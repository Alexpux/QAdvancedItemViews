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
#ifndef QFILTERMODELPROXY_H
#define QFILTERMODELPROXY_H

#include <qabstractfilterproxymodel.h>

#include <qaivlib_global.h>

class QAbstractFilterModel;

//! The QFilterModelProxy provides for filtering or highlighting data passed between a model and a view.
/**
 *
 */
class QAIVLIBSHARED_EXPORT QFilterModelProxy : public QAbstractFilterProxyModel
{
    Q_OBJECT
public:
	/**
	 * Constructs a filter model with the given @p parent.
	 */
    QFilterModelProxy(QObject* parent = 0);
	/**
	 * Destroys the filter model.
	 */
    ~QFilterModelProxy();

    QVariant data(const QModelIndex & index, int role) const;
protected:
	/**
	 * @reimp
	 */
    bool filterAcceptsRow( int source_row, const QModelIndex & source_parent ) const;
};

#endif // QFILTERMODELPROXY_H
