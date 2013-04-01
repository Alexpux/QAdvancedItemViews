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
#ifndef QSPECIALFILTER_H
#define QSPECIALFILTER_H

#include <qaivlib_global.h>
#include <qabstractfilter.h>

//! The QTextFilter class implements a conditonal filter.
class QAIVLIBSHARED_EXPORT QSpecialFilter : public QAbstractFilter
{
public:
    enum {
        Type = 7
    };
	//
    enum MatchFlag {
        MatchFlagInvalid = -1,
        Contains = 1,
        ContainsNot,
        EndsWith,
        EndsNotWith,
        IsBetween,
        IsEmpty,
        IsEqual,
        IsGreator,
        IsGreatorOrEqual,
        IsLessOrEqual,
        IsLess,
        IsNotBetween,
        IsNotEmpty,
        IsNotEqual,
        StartsWith,
        StartsNotWith
    };
    /**
      * Constructs a text filter for the given @p column.
      */
    QSpecialFilter(int row, int column);
    /**
      *  @reimp
      */
    QWidget* createEditor(QFilterViewItemDelegate* delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    void addContextMenuActions(QMenu* menu, QWidget* receiver);
    /**
      * Returns the data stored under the given @p role for this filter.
      * @see setData()
      * @remarks Returns an invalid QVariant if the no data exists for the given @p role.
      */
    QVariant data(int role = Qt::DisplayRole) const;

    bool matches(const QVariant & value, int type) const;

	QSpecialFilter::MatchFlag matchFlag() const;

    void setEditorData(QWidget * editor, const QModelIndex & index);

    void setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index);

	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index);
};

#endif // QSPECIALFILTER_H
