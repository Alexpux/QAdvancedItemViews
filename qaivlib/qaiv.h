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

#endif // QADVANCEDITEMVIEWS_H
