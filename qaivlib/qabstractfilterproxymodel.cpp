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

#include "qabstractfilterproxymodel.h"

#include "qfiltermodel.h"

class QAbstractFilterProxyModelPrivate {
public:
    explicit QAbstractFilterProxyModelPrivate(QAbstractFilterProxyModel *pm);
    ~QAbstractFilterProxyModelPrivate() = default;

    int lastResultCount { -1 };
    QAbstractFilterProxyModel *m { nullptr };
    QAbstractFilterModel *filterModel { nullptr };
};

QAbstractFilterProxyModelPrivate::QAbstractFilterProxyModelPrivate(QAbstractFilterProxyModel *pm) :
    m { pm }
{
}

QAbstractFilterProxyModel::QAbstractFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent), d(new QAbstractFilterProxyModelPrivate(this))
{
    d->filterModel = new QFilterModel(this);
}

QAbstractFilterProxyModel::~QAbstractFilterProxyModel()
{
    delete d;
}

QAbstractFilterModel *QAbstractFilterProxyModel::filterModel() const
{
    return d->filterModel;
}

QVariant QAbstractFilterProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical) {
        /*if (role == Qt::SizeHintRole) {
            QSize s = QSortFilterProxyModel::headerData(section, orientation, role).toSize();
            s.setHeight(30);
            return s;
        }*/
    }
    return QSortFilterProxyModel::headerData(section, orientation, role);
}

void QAbstractFilterProxyModel::setFilterModel(QAbstractFilterModel *filterModel)
{
    if (d->filterModel) {
        disconnect(d->filterModel);
    }
    d->filterModel = filterModel;
    connect(d->filterModel, &QAbstractFilterModel::dataChanged, this, &QAbstractFilterProxyModel::updateResult);
    connect(d->filterModel, &QAbstractFilterModel::modelReset, this, &QAbstractFilterProxyModel::updateResult);
    connect(d->filterModel, &QAbstractFilterModel::modeChanged, this, &QAbstractFilterProxyModel::updateResult);
    connect(d->filterModel, &QAbstractFilterModel::matchModeChanged, this, &QAbstractFilterProxyModel::updateResult);
    d->filterModel->setSourceModel(sourceModel());
}

QModelIndex QAbstractFilterProxyModel::getIndexForModel(const QAbstractItemModel *model, const QModelIndex &sourceIndex) const
{
    if (model) {
        const QAbstractProxyModel *p = qobject_cast<const QAbstractProxyModel *>(model);

        if (p) {
            const QAbstractItemModel *sModel = p->sourceModel();
            if (sModel == sourceIndex.model()) {
                return p->mapFromSource(sourceIndex);
            }
            return p->mapFromSource(getIndexForModel(sModel, sourceIndex));
        }
    }
    return QModelIndex();
}

/*QModelIndex QAbstractFilterProxyModel::mapFromSource(const QModelIndex &sourceIndex) const {
    QAbstractItemModel * sModel = sourceModel();

    QAbstractProxyModel* p = qobject_cast<QAbstractProxyModel*>(sModel);
    if (p) {
        return QSortFilterProxyModel::mapFromSource(p->mapFromSource(sourceIndex));
    }
    return QSortFilterProxyModel::mapFromSource(sourceIndex);
}*/

QModelIndex QAbstractFilterProxyModel::mapDeepFromSource(const QModelIndex &sourceIndex) const
{
    if (sourceIndex.isValid()) {
        return getIndexForModel(this, sourceIndex);
    }
    return QModelIndex();
}

void QAbstractFilterProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    if (d->filterModel->sourceModel()) {
        disconnect(d->filterModel->sourceModel(), nullptr, this, nullptr);
    }
    QSortFilterProxyModel::setSourceModel(sourceModel);
    d->filterModel->setSourceModel(sourceModel);
    connect(d->filterModel->sourceModel(), &QAbstractItemModel::modelReset, this, &QAbstractFilterProxyModel::updateResult);
    connect(d->filterModel->sourceModel(), &QAbstractItemModel::rowsInserted, this, &QAbstractFilterProxyModel::updateResult);
    connect(d->filterModel->sourceModel(), &QAbstractItemModel::rowsRemoved, this, &QAbstractFilterProxyModel::updateResult);
    emitResultCountChanged();
}

void QAbstractFilterProxyModel::updateResult()
{
    // reset last result count
    d->lastResultCount = -1;
    emit resultAboutToChange();
    // invalidate filter
    invalidateFilter();
    //
    emit resultChanged();
    // emit result count changed
    emitResultCountChanged();
}

void QAbstractFilterProxyModel::emitResultCountChanged()
{
    if (rowCount() != d->lastResultCount) {
        d->lastResultCount = rowCount();
        emit resultCountChanged(d->lastResultCount, d->filterModel->sourceModel()->rowCount());
    }
}
