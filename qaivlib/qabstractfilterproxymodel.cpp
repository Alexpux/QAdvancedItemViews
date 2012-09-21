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
#include "qabstractfilterproxymodel.h"

#include "qfiltermodel.h"

class QAbstractFilterProxyModelPrivate
{
public:
    QAbstractFilterProxyModelPrivate(QAbstractFilterProxyModel* pm);
    ~QAbstractFilterProxyModelPrivate();

    QAbstractFilterModel* filterModel;
    int lastResultCount;
    QAbstractFilterProxyModel* m;
};


QAbstractFilterProxyModelPrivate::QAbstractFilterProxyModelPrivate(QAbstractFilterProxyModel *pm)
{
    m = pm;
    filterModel = 0;
    lastResultCount = -1;
}

QAbstractFilterProxyModelPrivate::~QAbstractFilterProxyModelPrivate()
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

QAbstractFilterModel* QAbstractFilterProxyModel::filterModel() const
{
    return d->filterModel;
}

void QAbstractFilterProxyModel::setFilterModel(QAbstractFilterModel* filterModel)
{
    if (d->filterModel){
        disconnect(d->filterModel);
    }
    d->filterModel = filterModel;
    connect(d->filterModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(updateResult()));
    connect(d->filterModel, SIGNAL(modelReset()), this, SLOT(updateResult()));
    connect(d->filterModel, SIGNAL(modeChanged(QAdvancedItemViews::FilterProxyMode)), this, SLOT(updateResult()));
    connect(d->filterModel, SIGNAL(matchModeChanged(QAdvancedItemViews::FilterMatchMode)), this, SLOT(updateResult()));
    d->filterModel->setSourceModel(sourceModel());
}

void QAbstractFilterProxyModel::setSourceModel(QAbstractItemModel* sourceModel)
{
	if (d->filterModel->sourceModel()){
		disconnect(d->filterModel->sourceModel, 0, this, 0);
	}
    QSortFilterProxyModel::setSourceModel(sourceModel);
    d->filterModel->setSourceModel(sourceModel);
	connect(d->filterModel->sourceModel(), SIGNAL(modelReset()), this, SLOT(updateResult()));
    emitResultCountChanged();
}

void QAbstractFilterProxyModel::updateResult()
{
    invalidate();
    emitResultCountChanged();
}

void QAbstractFilterProxyModel::emitResultCountChanged()
{
    if (rowCount() != d->lastResultCount){
        d->lastResultCount = rowCount();
        emit resultCountChanged(d->lastResultCount, d->filterModel->sourceModel()->rowCount());
    }
}
