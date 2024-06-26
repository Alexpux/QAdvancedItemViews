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

#include "qfixedrowstableview.h"

#include <QDebug>

class QFixedRowsTableViewPrivate {
public:
    explicit QFixedRowsTableViewPrivate(QFixedRowsTableView *tv);
    ~QFixedRowsTableViewPrivate() = default;

    QFixedRowsFilterProxyModel *filterProxy { nullptr };
    QFixedRowsDecorationProxyModel *decorationProxy { nullptr };
    QFixedRowsTableView *v { nullptr };
};

QFixedRowsTableViewPrivate::QFixedRowsTableViewPrivate(QFixedRowsTableView *tv) :
    v { tv }
{
}

class QFixedRowsDecorationProxyModelPrivate {
public:
    explicit QFixedRowsDecorationProxyModelPrivate(QFixedRowsDecorationProxyModel *pm) :
        m { pm } { }

    ~QFixedRowsDecorationProxyModelPrivate() = default;

    bool enabled { false };
    QFixedRowsDecorationProxyModel *m { nullptr };
    QPointer<QFixedRowsFilterProxyModel> filterProxy;
    // QList<QPersistentModelIndex> rows;
};

class QFixedRowsFilterProxyModelPrivate {
public:
    explicit QFixedRowsFilterProxyModelPrivate(QFixedRowsFilterProxyModel *pm) :
        m { pm } { }

    ~QFixedRowsFilterProxyModelPrivate() = default;

    bool enabled { false };
    QFixedRowsFilterProxyModel *m { nullptr };
    QList<QPersistentModelIndex> rows;
};

QFixedRowsFilterProxyModel::QFixedRowsFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    d(new QFixedRowsFilterProxyModelPrivate(this))
{
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
    if (!sourceModel() || !d->enabled) {
        return false;
    }
    return d->rows.contains(QPersistentModelIndex(sourceModel()->index(source_row, 0)));
}

QVariant QFixedRowsFilterProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DecorationRole) {
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
    if (sourceModel() == nullptr || !d->enabled) {
        return false;
    }
    return d->rows.contains(QPersistentModelIndex(sourceModel()->index(row, 0)));
}

void QFixedRowsFilterProxyModel::setEnabled(bool on)
{
    if (d->enabled != on) {
        d->enabled = on;
        invalidateFilter();
    }
}

void QFixedRowsFilterProxyModel::sourceModelReset()
{
    /**
     * @todo slot sourceModelReset() should not be public
     */
    d->rows.clear();
    invalidateFilter();
}

void QFixedRowsFilterProxyModel::toggleRow(const QModelIndex &index)
{
    QPersistentModelIndex i(index);
    if (!d->rows.contains(i)) {
        d->rows.append(i);
    } else {
        d->rows.removeAt(d->rows.indexOf(i));
    }
    invalidateFilter();
}

void QFixedRowsFilterProxyModel::setRowFixed(const QModelIndex &index, bool fixed)
{
    Q_UNUSED(index)
    if (fixed) {

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
    if (d->enabled && orientation == Qt::Vertical && role == Qt::DecorationRole) {
        if (d->filterProxy) {
            QIcon icon = qvariant_cast<QIcon>(QIdentityProxyModel::headerData(section, orientation, role));
            QIcon state;
            if (d->filterProxy->isRowPinned(section)) {
                state = QIcon(":/qaiv/pin/fixed");
            } else {
                state = QIcon(":/qaiv/pin/free");
            }
            if (icon.isNull()) {
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
    if (d->enabled != on) {
        emit layoutAboutToBeChanged();
        d->enabled = on;
        emit modelToggled(d->enabled);
        emit layoutChanged();
    }
}

void QFixedRowsDecorationProxyModel::toggleRow(const QModelIndex &index)
{
    QModelIndex i(index);
    const QAbstractProxyModel *p;
    while (i.model() != sourceModel() && (p = qobject_cast<const QAbstractProxyModel *>(i.model()))) {
        i = p->mapToSource(i);
    }
    d->filterProxy->toggleRow(i);
}

QFixedRowsTableView::QFixedRowsTableView(QWidget *parent) :
    QTableView(parent), d(new QFixedRowsTableViewPrivate(this))
{
    d->filterProxy = new QFixedRowsFilterProxyModel(this);
    d->decorationProxy = new QFixedRowsDecorationProxyModel(d->filterProxy, this);

    connect(verticalHeader(), &QHeaderView::sectionClicked, this, &QFixedRowsTableView::verticalHeaderSectionClicked);

    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    QTableView::setModel(d->filterProxy);

    connect(d->filterProxy, &QFixedRowsFilterProxyModel::layoutChanged, this, &QFixedRowsTableView::updateHeight);
    connect(d->filterProxy, &QFixedRowsFilterProxyModel::modelReset, this, &QFixedRowsTableView::updateHeight);
    connect(d->filterProxy, &QFixedRowsFilterProxyModel::rowsInserted, this, &QFixedRowsTableView::updateHeight);
    connect(d->filterProxy, &QFixedRowsFilterProxyModel::rowsRemoved, this, &QFixedRowsTableView::updateHeight);

    connect(d->decorationProxy, &QFixedRowsDecorationProxyModel::modelToggled, d->filterProxy, &QFixedRowsFilterProxyModel::setEnabled);

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

    connect(model, &QAbstractItemModel::modelReset, d->filterProxy, &QFixedRowsFilterProxyModel::sourceModelReset);
    updateHeight();
}

void QFixedRowsTableView::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    if (editor->parent()->parent() == this) {
        QTableView::closeEditor(editor, hint);
    } else {
        qDebug() << "closeEditor(...) ignored by" << this;
    }
}

bool QFixedRowsTableView::fixedRowsMode() const
{
    return d->filterProxy->isEnabled();
}

void QFixedRowsTableView::focusInEvent(QFocusEvent *event)
{
    QTableView::focusInEvent(event);
    emit focusReceived();
}

void QFixedRowsTableView::setFixedRowsMode(bool on)
{
    d->filterProxy->setEnabled(on);
}

void QFixedRowsTableView::updateHeight()
{
    int h = 0;
    if (!model() || model()->rowCount() == 0) {
        hide();
    } else {
        resizeRowsToContents();
        show();
        for (int i = 0; i < model()->rowCount(); i++) {
            h += rowHeight(i);
        }
        h += 2;
    }
    setFixedHeight(h);
}

void QFixedRowsTableView::verticalHeaderSectionClicked(int section)
{
    QModelIndex i = model()->index(section, 0);
    const QAbstractProxyModel *proxy;
    while ((proxy = qobject_cast<const QAbstractProxyModel *>(i.model()))) {
        i = proxy->mapToSource(i);
    }
    d->filterProxy->toggleRow(i);
}
