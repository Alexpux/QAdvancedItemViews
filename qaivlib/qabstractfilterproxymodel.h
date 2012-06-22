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
#ifndef QABSTRACTFILTERPROXYMODEL_H
#define QABSTRACTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

#include <qaivlib_global.h>

class QAbstractFilterModel;
class QAbstractFilterProxyModelPrivate;

//! The QAbstractFilterProxyModel provides an abstract base class for filter models.
class QAIVLIBSHARED_EXPORT QAbstractFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
//    Q_PROPERTY(QAdvancedItemViews::FilterProxyModelMode mode READ mode WRITE setMode)
public:
    /**
      * Constructs a QAbstractFilterProxyModel with the given @p parent.
      */
    QAbstractFilterProxyModel(QObject* parent = 0);
    ~QAbstractFilterProxyModel();
    /**
      * Returns the filter model accosiated with proxy model.
      * @see setFilterModel()
      */
    QAbstractFilterModel* filterModel() const;
    /**
      * Sets the filter @p model providing the filter definitions.
      * @see filterModel()
      */
    void setFilterModel(QAbstractFilterModel* model);

    virtual void setSourceModel(QAbstractItemModel* sourceModel);
signals:
    /**
      * This signal is emitted whenever the number of rows in the filtered result set has changed.
      */
    void resultCountChanged(int filteredRows, int unfilteredRows);
protected:
    void emitResultCountChanged();
    virtual bool filterAcceptsRow( int source_row, const QModelIndex & source_parent ) const = 0;
private slots:
    void updateResult();
private:
    QAbstractFilterProxyModelPrivate* d;
};

#endif // QABSTRACTFILTERPROXYMODEL_H

