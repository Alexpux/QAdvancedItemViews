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
#include "qaiv.h"


QAdvancedItemViews::QAdvancedItemViews()
{
}

QModelIndex qSourceIndex(const QModelIndex & index)
{
    QModelIndex i(index);
    QAbstractProxyModel* proxy;
    while((proxy = qobject_cast<QAbstractProxyModel*>((QAbstractProxyModel*)i.model()))){
        i = proxy->mapToSource(i);
    }
    return i;
}

const QAbstractItemModel* qSourceModel(const QModelIndex & index)
{
	QModelIndex i(index);
	QAbstractProxyModel* p;
	while((p = qobject_cast<QAbstractProxyModel*>((QAbstractProxyModel*)i.model()))){
        i = p->mapToSource(i);
    }
	return i.model();
}
