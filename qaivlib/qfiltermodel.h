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
#ifndef QFILTERMODEL_H
#define QFILTERMODEL_H

#include <QMenu>

#include <qabstractfiltermodel.h>
#include <qaivlib_global.h>

//! The QFilterModel class provides a default implementation of a filter model.
/**
  * The following filters are provided by QDefaultFilterModel:
  * @li QAutoFilter
  * @li QRangeFilter
  * @li QRegExpFilter
  * @li QSelectionListFilter
  * @li QTextFilter
  * @li QValueFilter
  */
class QAIVLIBSHARED_EXPORT QFilterModel : public QAbstractFilterModel
{
    Q_OBJECT
public:
    /**
      * Constructs a default filter model with the given @p parent.
      */
    QFilterModel(QObject* parent);
    /**
      * Destroys the default filter model.
      */
    ~QFilterModel();
protected:
    /**
      * @reimp QAbstractFilterModel::createFilter()
      */
    QAbstractFilter* createFilter(const QModelIndex & index, const QVariantMap & properties) const;
};

#endif // QFILTERMODEL_H
