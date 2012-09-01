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
#ifndef QAUTOFILTER_H
#define QAUTOFILTER_H

#include <qabstractfilter.h>
#include <qaivlib_global.h>

//! The QAutoFilter class implements an auto filter.
class QAIVLIBSHARED_EXPORT QAutoFilter : public QAbstractFilter
{
public:
    enum {
        Type = 0
    };
    /**
      * Construtcs an auto filter with the given @p row and @p column.
      */
    QAutoFilter(int row, int column);
    /**
      * Returns the widget used to edit the filter specified by @p index for editing. The @p parent widget and style @p option are used to control how the editor widget appears.
      */
    QWidget* createEditor(QFilterViewItemDelegate* delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    QVariant data(int role = Qt::DisplayRole) const;

    bool matches(const QVariant & value, int type) const;

    void setEditorData(QWidget * editor, const QModelIndex & index);

    void setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index);

	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index);
};

#endif // QAUTOFILTER_H
