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

