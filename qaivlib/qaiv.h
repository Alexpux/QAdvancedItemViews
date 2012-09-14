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
#ifndef QAVI_H
#define QAVI_H

#include <qaivlib_global.h>

#include <QAbstractProxyModel>

class QAIVLIBSHARED_EXPORT QAdvancedItemViews
{
public:
    QAdvancedItemViews();

    enum FilterProxyMode {
        FilterMode,
        HighlightMode
    };
    enum FilterMatchMode {
        MatchNormal,
        MatchInverted
    };

    enum ItemDataRole {
        ValueFilterTypeRole = Qt::UserRole + 42
        ,DefaultFilterTypeRole
        ,ColumnFilterTypesRole
        ,ConditionalDecorationRole
		,SelectionListFilterDataRole
        ,IconSetsRole
    };

};

QAIVLIBSHARED_EXPORT QModelIndex qSourceIndex(const QModelIndex & index);
QAIVLIBSHARED_EXPORT const QAbstractItemModel* qSourceModel(const QModelIndex & index);

template <class T>
inline T qsourcemodel_cast(const QModelIndex & index)
{
	QModelIndex i(index);
	T s = qobject_cast<T>(i.model());
	if (s){
		return s;
	}
	QAbstractProxyModel* p;
	while((p = qobject_cast<QAbstractProxyModel*>((QAbstractProxyModel*)i.model()))){
		T s = qobject_cast<T>(p);
		if (s){
			return s;
		}
        i = p->mapToSource(i);
    }
	return 0;
}

template <class T>
inline T qsourcemodel_cast(QAbstractItemModel* model)
{
	QAbstractItemModel* m = model;
	T s = qobject_cast<T>(m);
	if (s){
		return s;
	}
	QAbstractProxyModel* p;
	while((p = qobject_cast<QAbstractProxyModel*>(m))){
		T s = qobject_cast<T>(p->sourceModel());
		if (s){
			return s;
		}
        m = p->sourceModel();
    }
	return 0;
}

#endif // QADVANCEDITEMVIEWS_H
