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
#include "qshareditemselectionmodel.h"

class QSharedItemSelectionModelPrivate
{
public:
    QSharedItemSelectionModelPrivate(QSharedItemSelectionModel* sm);
    ~QSharedItemSelectionModelPrivate();

    QItemSelectionModel* shared;

    QSharedItemSelectionModel* m;
};

QSharedItemSelectionModelPrivate::QSharedItemSelectionModelPrivate(QSharedItemSelectionModel *sm)
{
    m = sm;
}

QSharedItemSelectionModelPrivate::~QSharedItemSelectionModelPrivate()
{
}

QSharedItemSelectionModel::QSharedItemSelectionModel(QAbstractItemModel *model, QItemSelectionModel *shared, QObject *parent) :
    QItemSelectionModel(model, parent), d(new QSharedItemSelectionModelPrivate(this))
{
    d->shared = shared;
    connect(d->shared, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(sharedSelectionChanged(QItemSelection,QItemSelection)));
}

QSharedItemSelectionModel::~QSharedItemSelectionModel()
{
    delete d;
}

void QSharedItemSelectionModel::select(const QModelIndex &index, QItemSelectionModel::SelectionFlags command)
{
    QItemSelectionModel::select(index, command);
    d->shared->select(index, command);
}

void QSharedItemSelectionModel::select(const QItemSelection &selection, QItemSelectionModel::SelectionFlags command)
{
    QItemSelectionModel::select(selection, command);
    d->shared->select(selection, command);
}

void QSharedItemSelectionModel::sharedSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QItemSelectionModel::select(selected, QItemSelectionModel::Select);
    QItemSelectionModel::select(deselected, QItemSelectionModel::Deselect);
}

