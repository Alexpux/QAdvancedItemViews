/******************************************************************************
** This file is part of qadvanceditemviews.
**
** Copyright (c) 2011-2012 Martin Hoppe martin@2x2hoppe.de
**
** qadvanceditemviews is free software: you can redistribute it
** and/or modify it under the terms of the GNU Lesser General
** Public License as published by the Free Software Foundation,
** either version 3 of the License, or (at your option) any
** later version.
**
** qadvanceditemviews is distributed in the hope that it will be
** useful, but WITHOUT ANY WARRANTY; without even the implied
** warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with qadvanceditemviews.
** If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "quniquevaluesproxymodel.h"

#include <QDebug>

#define MT QHash

class QUniqueValuesProxyModelPrivate {
public:
    explicit QUniqueValuesProxyModelPrivate(QUniqueValuesProxyModel *pm);
    ~QUniqueValuesProxyModelPrivate() = default;

    bool emptyValues { true };
    int modelColumn { 0 };
    QUniqueValuesProxyModel *m { nullptr };
    MT<QString, QList<int>> valueMap;
};

QUniqueValuesProxyModelPrivate::QUniqueValuesProxyModelPrivate(QUniqueValuesProxyModel *pm) :
    m { pm }
{
}

QUniqueValuesProxyModel::QUniqueValuesProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent), d(new QUniqueValuesProxyModelPrivate(this))
{
}

QUniqueValuesProxyModel::~QUniqueValuesProxyModel()
{
    delete d;
}

QVariant QUniqueValuesProxyModel::data(const QModelIndex &index, int role) const
{
    return mapToSource(index).data(role);
}

bool QUniqueValuesProxyModel::emptyItemsAllowed() const
{
    return d->emptyValues;
}

bool QUniqueValuesProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent)
    return isDuplicate(source_row);
}

bool QUniqueValuesProxyModel::insertRows(int row, int count, const QModelIndex &parent)
{
    return QSortFilterProxyModel::insertRows(row, count, parent);
}

int QUniqueValuesProxyModel::modelColumn() const
{
    return d->modelColumn;
}

void QUniqueValuesProxyModel::setModelColumn(int colum)
{
    beginResetModel();
    d->modelColumn = colum;
    buildMap();
    endResetModel();
}

void QUniqueValuesProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    connect(sourceModel, &QAbstractItemModel::layoutChanged, this, &QUniqueValuesProxyModel::buildMap);
    connect(sourceModel, &QAbstractItemModel::modelReset, this, &QUniqueValuesProxyModel::buildMap);
    connect(sourceModel, &QAbstractItemModel::dataChanged, this, &QUniqueValuesProxyModel::sourceModelDataChanged);
    QSortFilterProxyModel::setSourceModel(sourceModel);
    buildMap();
    invalidate();
}

bool QUniqueValuesProxyModel::isDuplicate(int row) const
{
    QVariant v = sourceModel()->index(row, d->modelColumn).data(filterRole());
    if (!d->emptyValues) {
        if (v.toString().isEmpty()) {
            return false;
        }
    }
    MT<QString, QList<int>>::ConstIterator mIt = d->valueMap.constFind(v.toString());
    if (mIt == d->valueMap.constEnd()) {
        return true;
    }
    if (mIt.value().first() == row) {
        return true;
    }
    return false;
}

void QUniqueValuesProxyModel::buildMap()
{
    beginResetModel();
    d->valueMap.clear();
    if (sourceModel() == nullptr) {
        return;
    }
    MT<QString, QList<int>>::Iterator it;
    int c = sourceModel()->rowCount();
    int max = 0;
    for (int iRow = 0; iRow < c; iRow++) {
        QVariant v = sourceModel()->index(iRow, d->modelColumn).data(filterRole());
        it = d->valueMap.find(v.toString());
        if (it == d->valueMap.end()) {
            QList<int> l;
            l << iRow;
            if (max < v.toString().size()) {
                max = v.toString().size();
            }
            d->valueMap[v.toString()] = l;
        } else {
            it.value().append(iRow);
        }
        emit progressChanged(iRow * 100 / c);
    }
    emit progressChanged(100);
    endResetModel();
    // invalidate();
}

void QUniqueValuesProxyModel::setEmptyItemsAllowed(bool on)
{
    if (d->emptyValues != on) {
        buildMap();
    }
}

void QUniqueValuesProxyModel::sourceModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_UNUSED(topLeft)
    Q_UNUSED(bottomRight)
    buildMap();
}
