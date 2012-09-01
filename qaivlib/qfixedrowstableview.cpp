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
#include "qfixedrowstableview.h"

class QFixedRowsTableViewPrivate
{
public:
    QFixedRowsTableViewPrivate(QFixedRowsTableView* tv);
    ~QFixedRowsTableViewPrivate();

    QFixedRowsFilterProxyModel* filterProxy;
    QFixedRowsDecorationProxyModel* decorationProxy;

    QFixedRowsTableView* v;
};

QFixedRowsTableViewPrivate::QFixedRowsTableViewPrivate(QFixedRowsTableView *tv)
{
    decorationProxy = 0;
    filterProxy = 0;
    v = tv;
}

QFixedRowsTableViewPrivate::~QFixedRowsTableViewPrivate()
{
}

class QFixedRowsDecorationProxyModelPrivate
{
public:
    QFixedRowsDecorationProxyModelPrivate(QFixedRowsDecorationProxyModel* pm);
    ~QFixedRowsDecorationProxyModelPrivate();

    bool enabled;
    QPointer<QFixedRowsFilterProxyModel> filterProxy;
    QList<QPersistentModelIndex> rows;

    QFixedRowsDecorationProxyModel* m;
};

QFixedRowsDecorationProxyModelPrivate::QFixedRowsDecorationProxyModelPrivate(QFixedRowsDecorationProxyModel *pm)
{
    enabled = false;
    m = pm;
}

QFixedRowsDecorationProxyModelPrivate::~QFixedRowsDecorationProxyModelPrivate()
{

}

class QFixedRowsFilterProxyModelPrivate
{
public:
    QFixedRowsFilterProxyModelPrivate(QFixedRowsFilterProxyModel* pm);
    ~QFixedRowsFilterProxyModelPrivate();

    bool enabled;
    QList<QPersistentModelIndex> rows;

    QFixedRowsFilterProxyModel* m;
};

QFixedRowsFilterProxyModelPrivate::QFixedRowsFilterProxyModelPrivate(QFixedRowsFilterProxyModel *pm)
{
    m = pm;
}

QFixedRowsFilterProxyModelPrivate::~QFixedRowsFilterProxyModelPrivate()
{

}

QFixedRowsFilterProxyModel::QFixedRowsFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent), d(new QFixedRowsFilterProxyModelPrivate(this))
{
    d->enabled = false;
}

QFixedRowsFilterProxyModel::~QFixedRowsFilterProxyModel()
{
    delete d;
}

void QFixedRowsFilterProxyModel::clear()
{
    d->rows.clear();
    invalidateFilter();
}

bool QFixedRowsFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    if (sourceModel() == 0 || !d->enabled){
        return false;
    }
    return d->rows.contains(QPersistentModelIndex(sourceModel()->index(source_row, 0)));
}

QVariant QFixedRowsFilterProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DecorationRole){
        QIcon state;
        state = QIcon(":/qaiv/pin/fixed");
        return state;
    }
    return QSortFilterProxyModel::headerData(section, orientation, role);
}

bool QFixedRowsFilterProxyModel::isEnabled() const
{
    return d->enabled;
}

bool QFixedRowsFilterProxyModel::isRowPinned(int row) const
{
    if (sourceModel() == 0 || !d->enabled){
        return false;
    }
    return d->rows.contains(QPersistentModelIndex(sourceModel()->index(row, 0)));
}

void QFixedRowsFilterProxyModel::setEnabled(bool on)
{
    if (d->enabled != on){
        d->enabled = on;
        invalidateFilter();
    }
}

void QFixedRowsFilterProxyModel::sourceModelReset()
{
    d->rows.clear();
    invalidateFilter();
}

void QFixedRowsFilterProxyModel::toggleRow(const QModelIndex &index)
{
    QPersistentModelIndex i(index);
    if (!d->rows.contains(i)){
        d->rows.append(i);
    } else {
        d->rows.removeAt(d->rows.indexOf(i));
    }
    invalidateFilter();
}

void QFixedRowsFilterProxyModel::setRowFixed(const QModelIndex &index, bool fixed)
{
    if (fixed){

    } else {

    }
}

QFixedRowsDecorationProxyModel::QFixedRowsDecorationProxyModel(QFixedRowsFilterProxyModel *filterProxy, QObject *parent) :
    QIdentityProxyModel(parent), d(new QFixedRowsDecorationProxyModelPrivate(this))
{
    d->enabled = false;
    d->filterProxy = filterProxy;
}

QFixedRowsDecorationProxyModel::~QFixedRowsDecorationProxyModel()
{
    delete d;
}

QVariant QFixedRowsDecorationProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (d->enabled && orientation == Qt::Vertical && role == Qt::DecorationRole){
        if (d->filterProxy){
            QIcon icon = qvariant_cast<QIcon>(QIdentityProxyModel::headerData(section, orientation, role));
            QIcon state;
            if (d->filterProxy->isRowPinned(section)){
                state = QIcon(":/qaiv/pin/fixed");
            } else {
                state = QIcon(":/qaiv/pin/free");
            }
            if (icon.isNull()){
                return state;
            }
        }
    }
    return QIdentityProxyModel::headerData(section, orientation, role);
}

QSize QFixedRowsDecorationProxyModel::iconSize() const
{
	return QSize(24, 24);
}

bool QFixedRowsDecorationProxyModel::isEnabled() const
{
    return d->enabled;
}

void QFixedRowsDecorationProxyModel::setEnabled(bool on)
{
    if (d->enabled != on){
        emit layoutAboutToBeChanged();
        d->enabled = on;
        emit pinRowsEnabled(d->enabled);
        emit layoutChanged();
    }
}

void QFixedRowsDecorationProxyModel::toggleRow(const QModelIndex & index)
{
    //QModelIndex i = index;
    //QAbstractProxyModel* proxy;
    //while((proxy = qobject_cast<QAbstractProxyModel*>((QAbstractProxyModel*)i.model()))){
    //    i = proxy->mapToSource(i);
    //}
	QModelIndex i(index);
	QAbstractProxyModel* p;
	while(i.model() != sourceModel() && (p = qobject_cast<QAbstractProxyModel*>((QAbstractProxyModel*)i.model()))){
        i = p->mapToSource(i);
    }
    d->filterProxy->toggleRow(i);
}

QFixedRowsTableView::QFixedRowsTableView(QWidget *parent) :
    QTableView(parent), d(new QFixedRowsTableViewPrivate(this))
{
    d->filterProxy = new QFixedRowsFilterProxyModel(this);
    d->decorationProxy = new QFixedRowsDecorationProxyModel(d->filterProxy, this);

    connect(verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(verticalHeaderSectionClicked(int)));

    verticalHeader()->setResizeMode(QHeaderView::Fixed);
    QTableView::setModel(d->filterProxy);

    connect(d->filterProxy, SIGNAL(layoutChanged()), this, SLOT(updateHeight()));
    connect(d->filterProxy, SIGNAL(modelReset()), this, SLOT(updateHeight()));
    connect(d->filterProxy, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(updateHeight()));
    connect(d->filterProxy, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(updateHeight()));

    connect(d->decorationProxy, SIGNAL(pinRowsEnabled(bool)), d->filterProxy, SLOT(setEnabled(bool)));

    updateHeight();
}

QFixedRowsTableView::~QFixedRowsTableView()
{
    delete d;
}

QFixedRowsDecorationProxyModel *QFixedRowsTableView::decorationProxy() const
{
    return d->decorationProxy;
}

QFixedRowsFilterProxyModel *QFixedRowsTableView::filterProxy() const
{
    return d->filterProxy;
}

void QFixedRowsTableView::setModel(QAbstractItemModel *model)
{
    d->decorationProxy->setSourceModel(model);
    d->filterProxy->setSourceModel(model);

    connect(model, SIGNAL(modelReset()), d->filterProxy, SLOT(sourceModelReset()));
    updateHeight();
}

void QFixedRowsTableView::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    if (editor->parent()->parent() == this){
        QTableView::closeEditor(editor, hint);
    } else {
        qDebug() << "closeEditor(...) ignored by" << this;
    }
}

void QFixedRowsTableView::focusInEvent(QFocusEvent *event)
{
    QTableView::focusInEvent(event);
    emit focusReceived();
}

void QFixedRowsTableView::updateHeight()
{
    int h = 0;
    if (model() == 0 || model()->rowCount() == 0){
        hide();
    } else {
        resizeRowsToContents();
        show();
        for (int i = 0; i < model()->rowCount(); i++){
            h += rowHeight(i);
        }
        h += 2;
    }
    setFixedHeight(h);
}

void QFixedRowsTableView::verticalHeaderSectionClicked(int section)
{
    QModelIndex i = model()->index(section, 0);
    QAbstractProxyModel* proxy;
    while((proxy = qobject_cast<QAbstractProxyModel*>((QAbstractProxyModel*)i.model()))){
        i = proxy->mapToSource(i);
    }
    d->filterProxy->toggleRow(i);
}

