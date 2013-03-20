/******************************************************************************
** This file is part of qadvanceditemviews.
**
** Copyright (c) 2011-2013 Martin Hoppe martin@2x2hoppe.de
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

/**
 * @defgroup widgets Widgets
 * @defgroup views Views
 * @defgroup proxy Proxy Models
 * @defgroup filter Filter
 * @defgroup utils Utilities
 * @mainpage
 * The QAIV (Qt Advanced Item Views) library provides GUI and utility classes extending the capabilities of the Qt item views.
 * @image html main01.png Example of QAdvancedTableView with splitted views and fixed rows enabled
 * @section License
 * QAIV is distributed under the GNU Lesser General Public License version 3.
 * @section Screenshots
 * <ul>
 * <li>@ref screenshot1 "Conditional Decoration Proxy Model"</li>
 * <li>@ref screenshot2 "Grouping Proxy Model"</li>
 * <li>@ref screenshot3 "Unique Values Proxy Model"</li>
 * </li>
 * @section Download
 * You will find stable releases at <a href="http://qaiv.berlios.de">QAIV's project site</a>.
 * @section buildinstall Build and Install QAIV
 * @subsection requirements Requirements
 * QAIV requires Qt version 4.8.2 or greater.
 * @subsection platforms Platforms
 * QAIV shall build on all platforms supported by Qt. The current version is tested on Windows XP/7 only.
 * @subsection qmake Qt Creator / qmake
 * Use qaiv.pro to either build the library, the example application and the Qt designer plugin from the command line or open the qaiv.pro using Qt creator.
 * @subsection msvs Visual Studio
 * The repository contains the Visual Studio solution file to build the library, the example application, Qt designer plugin and test cases.
 * @subsection installer Installer
 * The current version does not provide an installer.
 * @page screenshot1 Conditional Decoration Proxy Model
 * The QConditionalDecorationProxyModel allows a user to decorate items in a table model by defining conditional decorations.
 * @image html conditional01.png "Conditional Decoration Proxy Model"
 * @page screenshot2 Grouping Proxy Model
 * The QGroupingProxyModel allows you to group items in a table model and display them in a tree view. The screenshot shows the source model grouped by 
 * the key words 'Qt's tools' and 'general software development' in column QtGroup.
 * Checking or unchecking 'Windows developers' and 'Unix developers' adds or removes the respective groups.
 * @image html grouping01.png "Grouping Proxy Model"
 * @page screenshot3 Unique Values Proxy Model
 * The QUniqueValuesProxyModel removes duplicate values from a source model. In the example the source model's column 'Duplicate Values' contains duplicate values for
 * A,B,C,... 
 * @image html uniquevalues01.png
 * 
 */
//! QAdvancedItemViews
class QAIVLIBSHARED_EXPORT QAdvancedItemViews
{
public:
    QAdvancedItemViews();
	//! This enum describes the modes of a filter proxy.
    enum FilterProxyMode {
        FilterMode,
        HighlightMode
    };
	//! This enumj describes the filter match mode.
    enum FilterMatchMode {
        MatchNormal, /*<! The filter matches if the condition is fullfilled */
        MatchInverted /*<! The filter matches if the condition is not fullfilled */
    };
	//! This enum decribes additional item data roles
    enum ItemDataRole {
		AutoFilterRole = Qt::UserRole + 42101,
        ValueFilterTypeRole,
        DefaultFilterTypeRole,
        ColumnFilterTypesRole,
        ConditionalDecorationRole,
		SelectionListFilterDataRole,
		SelectionFilterRole,
		ValueRole,
        IconSetsRole
    };

};

QAIVLIBSHARED_EXPORT QModelIndex qSourceIndex(const QModelIndex & index);
QAIVLIBSHARED_EXPORT const QAbstractItemModel* qSourceModel(const QModelIndex & index);
QAIVLIBSHARED_EXPORT QAbstractItemModel* qSourceModel(QAbstractItemModel* model);

/**
 * @ingroup utils
 * Returns the given @p model cast to type T if the model of the given @p index is of type T (or of a subclass); otherwise returns 0. If @p index is invalod then it will also return 0.
 */
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

/**
 * @ingroup utils
 * Returns the given @p model cast to type T if the @p model is of type T (or of a subclass); otherwise returns 0. If @p model is 0 then it will also return 0.
 */
template <class T> inline T qsourcemodel_cast(QAbstractItemModel* model)
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

/**
 * @ingroup utils
 * Returns the given @p model cast to type T if the @p model is of type T (or of a subclass); otherwise returns 0. If @p model is 0 then it will also return 0.
 */
template <class T>
inline T qsourcemodel_cast(const QAbstractItemModel* model)
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
