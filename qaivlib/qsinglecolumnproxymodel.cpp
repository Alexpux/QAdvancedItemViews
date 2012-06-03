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
#include "stdafx.h"
#include "qsinglecolumnproxymodel.h"

class QSingleColumnProxyModelPrivate
{
public:
    QSingleColumnProxyModelPrivate(QSingleColumnProxyModel* pm);
    ~QSingleColumnProxyModelPrivate();

    int modelColumn;

    QSingleColumnProxyModel* m;
};

QSingleColumnProxyModelPrivate::QSingleColumnProxyModelPrivate(QSingleColumnProxyModel *pm)
{
    modelColumn = 0;
    m = pm;
}

QSingleColumnProxyModelPrivate::~QSingleColumnProxyModelPrivate()
{

}

QSingleColumnProxyModel::QSingleColumnProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent), d(new QSingleColumnProxyModelPrivate(this))
{
}

QSingleColumnProxyModel::~QSingleColumnProxyModel()
{
    delete d;
}

bool QSingleColumnProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    return source_column == d->modelColumn;
}

void QSingleColumnProxyModel::setSourceModelColumn(int column)
{
    d->modelColumn = column;
    invalidate();
}

int QSingleColumnProxyModel::sourceModelColumn() const
{
    return d->modelColumn;
}
