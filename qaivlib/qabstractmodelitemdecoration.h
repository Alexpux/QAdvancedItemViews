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
#ifndef QABSTRACTITEMMODELDECORATION_H
#define QABSTRACTITEMMODELDECORATION_H

class QAbstractItemModelDecorationPrivate;

#include "qaivlib_global.h"

//! The QDecoration class is a base class for item model decorations.
class QAIVLIBSHARED_EXPORT QAbstractItemModelDecoration
{
public:
    /**
      * Constructs an empty QAbstractItemModelDecoration.
      */
    QAbstractItemModelDecoration();
    /**
      * Destroys the QAbstractItemModelDecoration.
      */
    ~QAbstractItemModelDecoration();

    virtual QVariant decorate(const QModelIndex & index, int role = Qt::DecorationRole) const = 0;
    /**
      *
      */
    QIcon defaultIcon() const;
    /**
      * Returns the value of the property specified by the given @p key.
      */
    QVariant property(const QString & key, const QVariant & defaultValue = QVariant()) const;
    /**
      * Returns the filter's properties.
      */
    QMap<QString,QVariant> properties() const;
	/**
	 * Restore the decoration's state and properties to the @p state specified.
	 * @sa saveState()
	 */
	bool restoreState(const QByteArray & state);
	/**
	 * Saves the decoration's state and properties.
	 * @sa restoreState()
	 */
	QByteArray saveState() const;
    /**
      * Sets the filter definition's property specified by @p key to the given @p value.
      */
    void setProperty(const QString & name, const QVariant & value);

    void setProperties(const QVariantMap & properties);
private:
    QAbstractItemModelDecorationPrivate* d;
};

#endif // QABSTRACTITEMMODELDECORATION_H
