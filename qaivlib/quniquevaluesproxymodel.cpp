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
#include "stdafx.h"
#include "quniquevaluesproxymodel.h"

#include <QDebug>

class QUniqueValuesProxyModelPrivate
{
public:
    QUniqueValuesProxyModelPrivate(QUniqueValuesProxyModel* pm);
    ~QUniqueValuesProxyModelPrivate();

	bool emptyValues;
    int modelColumn;
    QMap<QString,QList<int> > valueMap;
    QUniqueValuesProxyModel* m;
};

QUniqueValuesProxyModelPrivate::QUniqueValuesProxyModelPrivate(QUniqueValuesProxyModel *pm)
{
	emptyValues = true;
    modelColumn = 0;
    m = pm;
}

QUniqueValuesProxyModelPrivate::~QUniqueValuesProxyModelPrivate()
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

QVariant QUniqueValuesProxyModel::data(const QModelIndex & index, int role) const
{
    return mapToSource(index).data(role);
}

bool QUniqueValuesProxyModel::emptyItemsAllowed() const
{
	return d->emptyValues;
}

bool QUniqueValuesProxyModel::filterAcceptsRow(int source_row, const QModelIndex & source_parent) const
{
    return isDuplicate(source_row);
}

bool QUniqueValuesProxyModel::insertRows(int row, int count, const QModelIndex & parent)
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

void QUniqueValuesProxyModel::setSourceModel(QAbstractItemModel* sourceModel)
{
    connect(sourceModel, SIGNAL(layoutChanged()), this, SLOT(buildMap()));
    connect(sourceModel, SIGNAL(modelReset()), this, SLOT(buildMap()));
    connect(sourceModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(sourceModelDataChanged(QModelIndex,QModelIndex)));
    QSortFilterProxyModel::setSourceModel(sourceModel);
    buildMap();
    invalidate();
}

bool QUniqueValuesProxyModel::isDuplicate(int row) const
{
	QVariant v = sourceModel()->index(row, d->modelColumn).data(filterRole());
	if (!d->emptyValues){
		if (v.toString().isEmpty()){
			return false;
		}
	}
    QMap<QString, QList<int> >::ConstIterator mIt = d->valueMap.constFind(v.toString());
    if (mIt == d->valueMap.constEnd()){
        return true;
    }
    if (mIt.value().first() == row){
        return true;
    }
    return false;
}

void QUniqueValuesProxyModel::buildMap()
{
    beginResetModel();
    d->valueMap.clear();
    if (sourceModel() == 0){
        return;
    }
    QMap<QString, QList<int> >::Iterator it;
    for(int iRow = 0; iRow < sourceModel()->rowCount(); iRow++){
		QVariant v = sourceModel()->index(iRow, d->modelColumn).data(filterRole());
		it = d->valueMap.find(v.toString());
		if (it == d->valueMap.end()){
			QList<int> l;
			l << iRow;
			d->valueMap[v.toString()] = l;
		} else {
			it.value().append(iRow);
		}
	}
    endResetModel();
    invalidate();
}

void QUniqueValuesProxyModel::setEmptyItemsAllowed(bool on)
{
	if (d->emptyValues != on){
		buildMap();
	}
}

void QUniqueValuesProxyModel::sourceModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    buildMap();
}
