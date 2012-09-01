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
#ifndef QVALUEFILTER_H
#define QVALUEFILTER_H

#include <qabstractfilter.h>
#include <qaivlib_global.h>

//! The QValueFilter class implements a value type aware filter.
class QAIVLIBSHARED_EXPORT QValueFilter : public QAbstractFilter
{
public:
    enum {
        Type = 5
    };
    /**
      * Constructs a value filter for the given @p column.
      */
    QValueFilter(int row, int column);
    /**
      * Destroys the value filter.
      */
    ~QValueFilter();

    QWidget* createEditor(QFilterViewItemDelegate* delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    QVariant data(int role = Qt::DisplayRole) const;

    bool matches(const QVariant & value, int type) const;

    void setEditorData(QWidget * editor, const QModelIndex & index);

    void setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index);

	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index);
};

QDebug operator<<(QDebug dbg, const QValueFilter & f);

#endif // QVALUEFILTER_H
