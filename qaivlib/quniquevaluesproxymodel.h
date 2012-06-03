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
#ifndef QUNIQUEVALUESPROXYMODEL_H
#define QUNIQUEVALUESPROXYMODEL_H

#include <QSortFilterProxyModel>

#include <qaivlib_global.h>

class QUniqueValuesProxyModelPrivate;

//! The QUniqueValuesProxyModel class
/**
  * @todo
  */
class QAIVLIBSHARED_EXPORT QUniqueValuesProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit QUniqueValuesProxyModel(QObject *parent = 0);
    
    ~QUniqueValuesProxyModel();

    QVariant data(const QModelIndex &index, int role) const;

    bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const;

    virtual bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());

    int modelColumn() const;

    void setModelColumn(int colum);

    void setSourceModel(QAbstractItemModel* sourceModel);
signals:
    
public slots:
private slots:
    bool isDuplicate(int row) const;
    void buildMap();
    void sourceModelDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
private:
    QUniqueValuesProxyModelPrivate* d;
};

#endif // QUNQIUEVALUESPROXYMODEL_H
