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
#ifndef QCONDITONALDECORATION_H
#define QCONDITONALDECORATION_H

#include <qabstractmodelitemdecoration.h>

#include "qaivlib_global.h"

class QConditionalDecorationProxyModel;
//! The QConditionalDecoration provides a conditional decorarion for model items.
class QAIVLIBSHARED_EXPORT QConditionalDecoration : public QAbstractItemModelDecoration
{
public:
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
      * Constructs an invalid QConditionalDecoration.
      */
    QConditionalDecoration();
    /**
      * Constructs a QConditionalDecoration with the specified @p column.
      */
    QConditionalDecoration(int column);

    void addCondition( QConditionalDecoration::MatchFlag matchFlag, const QVariant & value, const QString & set, const QString & name);
    /**
      * Returns the match flag for the given @p index or QModelItemDecoration::MatchFlagInvalid if @p index does not exists.
      */
    QConditionalDecoration::MatchFlag matchFlag(int index) const;
    /**
      * Returns the number of conditions.
      */
    int count() const;
	/**
	 * Returns the decoration for the given @p index.
	 */
    QVariant decorate(const QModelIndex & index, int role = Qt::DecorationRole) const;
    /**
      * Returns the icon name of the condition at the given @p index or QString::null if @p index does not exists.
      */
    QString iconName(int index) const;

    bool matches(const QModelIndex & index, const QVariantMap & properties) const;
    /**
      * Returns the properties of the condition at the given @p index.
      */
    QVariantMap properties(int index) const;
    /**
      * Sets the item @p role that is used to query the source model's data when highlighting items.
      */
    void setHighlightRole(int role);

    void setDefaultDecoration(const QString & set, const QString & name);
    /**
      * Returns the value at the given @p index.
      */
    QVariant value(int index) const;
private:
    bool equal(const QVariant & left, const QVariant & right) const;
    bool greaterOrEqualThan(const QVariant & left, const QVariant & right) const;
    bool greaterThan(const QVariant & left, const QVariant & right) const;
    bool lessOrEqualThan(const QVariant & left, const QVariant & right) const;
    bool lessThan(const QVariant & left, const QVariant & right) const;
};

#endif // QCONDITONALDECORATION_H
