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
#ifndef QSINGLECOLUMNPROXYMODEL_H
#define QSINGLECOLUMNPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <qaivlib_global.h>

class QSingleColumnProxyModelPrivate;

//! The QSingleColumnProxyModel class provides a proxy model which represent a single column of the source model.
/**
 * @ingroup proxy
 */
class QAIVLIBSHARED_EXPORT QSingleColumnProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    //! @property(sourceModelColumn)
    /**
	  * This property holds the column of the source model that is used.
      * By default, this property contains 0, indicating that the first column of the source model is used.
      */
    Q_PROPERTY(int sourceModelColumn READ sourceModelColumn WRITE setSourceModelColumn)
public:
    /**
      * Constructs a QSingleColumnProxyModel with the given @p parent.
      */
    explicit QSingleColumnProxyModel(QObject *parent = 0);
    /**
      * Destoys the QSingleColumnProxyModel.
      */
    ~QSingleColumnProxyModel();
    /**
      * Returns source model's column used by this proxy.
      */
    int sourceModelColumn() const;
public slots:
	/**
      * Sets the source model @p column used by this proxy.
      */
    void setSourceModelColumn(int column);
private:   
    bool filterAcceptsColumn(int source_column, const QModelIndex & source_parent) const;

    QSingleColumnProxyModelPrivate* d;
};

#endif // QSINGLECOLUMNPROXYMODEL_H
