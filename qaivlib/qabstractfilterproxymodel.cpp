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

QAbstractFilterProxyModel::QAbstractFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    cFilterModel = new QFilterModel(this);
}

QAbstractFilterModel* QAbstractFilterProxyModel::filterModel() const
{
    return cFilterModel;
}

void QAbstractFilterProxyModel::setFilterModel(QAbstractFilterModel* filterModel)
{
    if (cFilterModel){
        disconnect(cFilterModel);
    }
    cFilterModel = filterModel;
    connect(cFilterModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(invalidate()));
    connect(cFilterModel, SIGNAL(modelReset()), this, SLOT(invalidate()));
    connect(cFilterModel, SIGNAL(modeChanged(QAdvancedItemViews::FilterProxyMode)), this, SLOT(invalidate()));
    connect(cFilterModel, SIGNAL(matchModeChanged(QAdvancedItemViews::FilterMatchMode)), this, SLOT(invalidate()));
    cFilterModel->setSourceModel(sourceModel());
    cFilterModel->setProxyModel(this);
}

void QAbstractFilterProxyModel::setSourceModel(QAbstractItemModel* sourceModel)
{
    QSortFilterProxyModel::setSourceModel(sourceModel);
    cFilterModel->setSourceModel(sourceModel);
}
