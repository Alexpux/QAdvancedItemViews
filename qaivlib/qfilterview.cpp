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

#include "qfilterview.h"

#include "qabstractfilter.h"
#include "qabstractfiltermodel.h"
#include "qadvancedheaderview.h"
#include "qfilterviewconnector.h"
#include "qfilterviewitemdelegate.h"

#include <QAbstractButton>
#include <QColorDialog>
#include <QContextMenuEvent>
#include <QTreeView>

class QFilterViewPrivate {
public:
    explicit QFilterViewPrivate(QFilterView *fw);
    QFilterViewPrivate(const QFilterViewPrivate &other);
    QFilterViewPrivate &operator=(const QFilterViewPrivate &other);

    ~QFilterViewPrivate() = default;

    bool filterVisible { true };
    int allowedFilterTypes { 0xFFFF };
    int defaultFilterType { QAbstractFilter::Type };
    int maxFilterVisible { 3 };

    QFilterView *v { nullptr };
    QFilterViewConnector *filterViewConnector { nullptr };
};

QFilterViewPrivate::QFilterViewPrivate(QFilterView *fw) :
    v { fw }
{
    filterViewConnector = new QFilterViewConnector(fw);
}

QFilterViewPrivate::QFilterViewPrivate(const QFilterViewPrivate &other)
{
    allowedFilterTypes = other.allowedFilterTypes;
    defaultFilterType = other.defaultFilterType;
    filterVisible = other.filterVisible;
    maxFilterVisible = other.maxFilterVisible;
    filterViewConnector = new QFilterViewConnector(other.v);

    v = other.v;
}

QFilterViewPrivate &QFilterViewPrivate::operator=(const QFilterViewPrivate &other)
{
    if (&other != this) {
        allowedFilterTypes = other.allowedFilterTypes;
        defaultFilterType = other.defaultFilterType;
        filterVisible = other.filterVisible;
        maxFilterVisible = other.maxFilterVisible;
        filterViewConnector = new QFilterViewConnector(other.v);

        v = other.v;
    }
    return *this;
}

/**
 * Constructs a QFilterView with the given @p parent.
 */
QFilterView::QFilterView(QWidget *parent) :
    QTableView(parent), d(new QFilterViewPrivate(this))
{
    setItemDelegate(new QFilterViewItemDelegate(this));

    setHorizontalHeader(new QAdvancedHeaderView(Qt::Horizontal, this));
    setVerticalHeader(new QAdvancedHeaderView(Qt::Vertical, this));

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QAbstractButton *cb = findChild<QAbstractButton *>();
    if (cb) {
        cb->disconnect();
        connect(cb, &QAbstractButton::clicked, this, &QFilterView::cornerButtonClicked);
    }
    updateGeometry();
}

/**
 * Destroys the filter view.
 */
QFilterView::~QFilterView()
{
    delete d;
}

void QFilterView::connectToView(QAbstractItemView *view)
{
    QTableView *tableView = qobject_cast<QTableView *>(view);
    if (tableView) {
        d->filterViewConnector->setDataView(tableView);
    }
    QTreeView *treeView = qobject_cast<QTreeView *>(view);
    if (treeView) {
        d->filterViewConnector->setDataView(treeView);
    }
}

void QFilterView::addFilter()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        QModelIndex index = model()->index(action->data().toMap().value("row").toInt(), action->data().toMap().value("column").toInt());
        if (index.isValid()) {
            model()->setData(index, action->data().toMap());
            edit(index);
        }
    }
}

void QFilterView::changeColor()
{
    QVariantMap properties = currentIndex().data(Qt::EditRole).toMap();
    if (!properties.isEmpty()) {
        QColor color = qvariant_cast<QColor>(properties.value("highlightColor"));
        color = QColorDialog::getColor(color, this, tr("Select Highlight Color"));
        if (color.isValid()) {
            properties["highlightColor"] = color;
            model()->setData(currentIndex(), properties);
        }
    }
}

void QFilterView::changeProperties()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        QModelIndex index = model()->index(action->data().toMap().value("row").toInt(), action->data().toMap().value("column").toInt());
        if (index.isValid()) {
            model()->setData(index, action->data().toMap());
        }
    }
}

/**
 * Disables the selected filter definitions.
 * @see enableSelectedFilters()
 */
void QFilterView::disableSelectedFilters()
{
    QVariantMap properties;
    QModelIndexList lst = selectionModel()->selectedIndexes();
    for (const QModelIndex &index : lst) {
        properties = index.data(Qt::EditRole).toMap();
        if (!properties.isEmpty()) {
            properties["enabled"] = false;
            model()->setData(index, properties);
        }
    }
}

/**
 * Enables the selected filter definitions.
 * @see disableSelectedFilters()
 */
void QFilterView::enableSelectedFilters()
{
    QVariantMap properties;
    QModelIndexList lst = selectionModel()->selectedIndexes();
    for (const QModelIndex &index : lst) {
        properties = index.data(Qt::EditRole).toMap();
        if (!properties.isEmpty()) {
            properties["enabled"] = true;
            model()->setData(index, properties);
        }
    }
}

void QFilterView::contextMenuEvent(QContextMenuEvent *event)
{
    const QAbstractFilterModel *m = qobject_cast<QAbstractFilterModel *>(model());
    if (!m) {
        event->ignore();
        return;
    }
    QModelIndexList selection = selectedIndexes();
    if (selection.isEmpty()) {
        event->ignore();
        return;
    }
    QMenu *menu = new QMenu(this);

    if (selection.size() > 1) {
        menu->addAction(QIcon(":/qaiv/filter/enabled"), tr("Enable Filter"), this, &QFilterView::enableSelectedFilters);
        menu->addAction(QIcon(":/qaiv/filter/disabled"), tr("Disable Filter"), this, &QFilterView::disableSelectedFilters);
        menu->addSeparator();
        menu->addAction(QIcon(":/qaiv/filter/toggle"), tr("Toggle Filter"), this, &QFilterView::toggleSelectedFilters);
    } else {
        QAction *action = nullptr;
        QVariantMap properties = selection.first().data(Qt::EditRole).toMap();
        if (properties.isEmpty()) {
            properties["row"] = selection.first().row();
            properties["column"] = selection.first().column();
            properties["enabled"] = true;
            QVariantList mTypes = selection.first().data(QAbstractFilterModel::ColumnFilterTypesRole).toList();
            QList<QAbstractFilterModel::FilterTypeEntry> fTypes = m->registeredFilterTypes();
            for (const QAbstractFilterModel::FilterTypeEntry &entry : fTypes) {
                if (entry.type == QAbstractFilter::Type) {
                    menu->addSeparator();
                } else {
                    if (mTypes.isEmpty() || mTypes.contains(entry.type)) {
                        action = menu->addAction(entry.icon, entry.text, this, &QFilterView::addFilter);
                        properties["type"] = entry.type;
                        action->setData(properties);
                        action->setToolTip(entry.tooltip);
                    }
                }
            }
        } else {
            m->filter(selection.first())->addContextMenuActions(menu, this);
            if (!menu->isEmpty()) {
                menu->addSeparator();
            }
            if (properties.value("enabled").toBool()) {
                action = menu->addAction(QIcon(":/qaiv/filter/disabled"), tr("Disable Filter"), this, &QFilterView::changeProperties);
                properties["enabled"] = false;
            } else {
                action = menu->addAction(QIcon(":/qaiv/filter/enabled"), tr("Enable Filter"), this, &QFilterView::changeProperties);
                properties["enabled"] = true;
            }
            action->setData(properties);
            if (m->mode() == QAdvancedItemViews::HighlightMode) {
                menu->addSeparator();
                QPixmap pixmap(22, 22);
                pixmap.fill(qvariant_cast<QColor>(properties.value("highlightColor")));
                action = menu->addAction(tr("Change Color..."), this, &QFilterView::changeColor);
                action->setIcon(QIcon(pixmap));
                action->setData(properties);
            }
            menu->addSeparator();
            action = menu->addAction(tr("Remove filter"), this, &QFilterView::removeFilter);
            action->setData(properties);
        }
    }

    // QMenu* mMenu = mModel->createStandardContextMenu(selectionModel()->selectedIndexes(), this);
    menu->exec(viewport()->mapToGlobal(event->pos()));
    event->accept();
    delete menu;
}

void QFilterView::focusInEvent(QFocusEvent *event)
{
    QTableView::focusInEvent(event);
    emit focusReceived();
}

/**
 * Returns the default filter type or -1 is no default type is set.
 * @see setDefaultFilterType()
 */
int QFilterView::defaultFilterType() const
{
    return d->defaultFilterType;
}

QFilterViewConnector *QFilterView::filterViewConnector() const
{
    return d->filterViewConnector;
}

bool QFilterView::filterVisible() const
{
    return d->filterVisible;
}

/**
 * Returns the maximum number of visible filter sets.
 * @see setMaxVisibileFilterSets()
 */
int QFilterView::maxVisibileFilterSets() const
{
    return d->maxFilterVisible;
}

void QFilterView::mousePressEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    if (event->button() == Qt::LeftButton && index.isValid()) {
        QRect r = visualRect(index);
        r.setWidth(16);
        if (r.contains(event->pos())) {
            toggleFilter(index);
        }
        QTableView::mousePressEvent(event);
    } else {
        QTableView::mousePressEvent(event);
    }
}

/**
 * Hides the filter definitions.
 * @see showFilter()
 */
void QFilterView::hideFilter()
{
    setFilterVisible(false);
}

void QFilterView::setDefaultFilterType(int type)
{
    d->defaultFilterType = type;
}

/**
 * Sets the maximum number of filter definition visible in this view to @p rows.
 * @see maxVisibileFilterSets()
 */
void QFilterView::setMaxVisibileFilterSets(int rows)
{
    d->maxFilterVisible = rows;
    emit calcGeometryRequested();
}

void QFilterView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
    connect(model, &QAbstractItemModel::modelReset, this, &QFilterView::calcGeometryRequested);
    connect(model, &QAbstractItemModel::rowsInserted, this, &QFilterView::calcGeometryRequested);
    connect(model, &QAbstractItemModel::rowsRemoved, this, &QFilterView::calcGeometryRequested);
    //    calcGeometry();
    updateGeometry();
}

/**
 * Shows the filter definitions.
 * @see hideFilter()
 */
void QFilterView::showFilter()
{
    setFilterVisible(true);
}

void QFilterView::updateGeometry()
{
    if (model() == nullptr) {
        setFixedHeight(horizontalHeader()->sizeHint().height());
    } else {
        int rows = model()->rowCount();
        if (rows > maxVisibileFilterSets()) {
            rows = maxVisibileFilterSets();
        }
        for (int iRow = 0; iRow < model()->rowCount(); iRow++) {
            verticalHeader()->resizeSection(iRow, verticalHeader()->sizeHint().height());
        }
        int rowHeight = verticalHeader()->sizeHint().height();
        if (rowHeight == 0) {
            rowHeight = horizontalHeader()->sizeHint().height();
        }
        int headerHeight = horizontalHeader()->sizeHint().height();
        if (headerHeight == 0) {
            headerHeight = rowHeight;
        }
        if (filterVisible()) {
            setFixedHeight(headerHeight + rowHeight * rows + (1 * rows) + 1);
        } else {
            setFixedHeight(horizontalHeader()->sizeHint().height());
        }
    }
}

void QFilterView::removeFilter()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        QModelIndex index = model()->index(action->data().toMap().value("row").toInt(), action->data().toMap().value("column").toInt());
        if (index.isValid()) {
            model()->setData(index, QVariantMap());
        }
    }
}

void QFilterView::setFilterVisible(bool visible)
{
    if (visible != d->filterVisible) {
        d->filterVisible = visible;
        emit visibilityChanged(d->filterVisible);
    }
}

/**
 * Toggles the state (enabled/disabled) of the filter specified by the given @p index.
 */
void QFilterView::toggleFilter(const QModelIndex &index)
{
    if (index.isValid()) {
        QVariantMap p;
        p = index.data(Qt::EditRole).toMap();
        if (!p.isEmpty()) {
            p["enabled"] = !p.value("enabled").toBool();
            model()->setData(index, p);
        }
    }
}

void QFilterView::toggleSelectedFilters()
{
    QVariantMap properties;
    QModelIndexList lst = selectionModel()->selectedIndexes();
    for (const QModelIndex &index : lst) {
        properties = index.data(Qt::EditRole).toMap();
        if (!properties.isEmpty()) {
            properties["enabled"] = !properties.value("enabled").toBool();
            model()->setData(index, properties);
        }
    }
}
