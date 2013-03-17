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
#ifndef QREGEXPFILTER_H
#define QREGEXPFILTER_H

#include <qaivlib_global.h>
#include <qabstractfilter.h>

//! The QRegExpFilter class implements a regular expression filter.
/**
 * @ingroup filter
 */
class QAIVLIBSHARED_EXPORT QRegExpFilter : public QAbstractFilter
{
public:
    enum {
        Type = 3
    };
	/**
	 * Constructs a QRegExpFilter with the given @p row and @p column.
	 */
    QRegExpFilter(int row, int column);

    QWidget* createEditor(QFilterViewItemDelegate* delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    /**
      * Returns the data stored under the given @p role for this filter.
      * @see setData()
      * @remarks Returns an invalid QVariant if the no data exists for the given @p role.
      */
    QVariant data(int role = Qt::DisplayRole) const;

	bool matches(const QVariant & value, int type) const;
	/**
	 * Returns the regular expression used by this filter.
	 */
    QRegExp regExp() const;
	/**
	 * Sets the contents of the given @p editor to the data for the filter at the given @p index. Note that the @p index contains information about the filter model being used.
	 */
    void setEditorData(QWidget * editor, const QModelIndex & index);
	/**
	 * Sets the data for the filter at the given @p index in the filter @p model to the contents of the given @p editor.
	 */
    void setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index);
	/**
	 * Updates the filter's @p editor geometry specified by index according to the style option given.
	 */
	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index);
};

QDebug operator<<(QDebug dbg, const QRegExpFilter & f);

#endif // QREGEXPFILTER_H
