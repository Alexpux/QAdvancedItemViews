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
#include "qfilterview.h"

#include "qabstractfilter.h"
#include "qabstractfiltermodel.h"
#include "qadvancedheaderview.h"
#include "qfilterviewconnector.h"
#include "qfilterviewitemdelegate.h"

class QFilterViewPrivate
{
public:
    QFilterViewPrivate(QFilterView* fw);
    ~QFilterViewPrivate();

    QFilterViewConnector* filterViewConnector;

    int allowedFilterTypes;
    int defaultFilterType;
    bool filterVisible;
    int maxFilterVisible;

    QFilterView* v;
};

QFilterViewPrivate::QFilterViewPrivate(QFilterView *fw)
{
    allowedFilterTypes = 0xFFFF;
    defaultFilterType = QAbstractFilter::Type;
    filterVisible = true;
    maxFilterVisible = 3;
    filterViewConnector = new QFilterViewConnector(fw);

    v = fw;
}

QFilterViewPrivate::~QFilterViewPrivate()
{

}

QFilterView::QFilterView( QWidget* parent )
    : QTableView(parent), d(new QFilterViewPrivate(this))
{
    setItemDelegate(new QFilterViewItemDelegate(this));

    setHorizontalHeader(new QAdvancedHeaderView(Qt::Horizontal, this));
    setVerticalHeader(new QAdvancedHeaderView(Qt::Vertical, this));

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QAbstractButton* mCornerButton = qFindChild<QAbstractButton*>(this);
    if (mCornerButton){
        mCornerButton->disconnect();
        connect(mCornerButton, SIGNAL(clicked()), this, SIGNAL(cornerButtonClicked()));
    }
    updateGeometry();

}

QFilterView::~QFilterView()
{
    delete d;
}

void QFilterView::connectToView(QAbstractItemView* view)
{
    QTableView* tableView = qobject_cast<QTableView*>(view);
    if (tableView){
        d->filterViewConnector->setDataView(tableView);
    }
    QTreeView* treeView = qobject_cast<QTreeView*>(view);
    if (treeView){
        d->filterViewConnector->setDataView(treeView);
    }
}

void QFilterView::addFilter()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action){
        QModelIndex index = model()->index(action->data().toMap().value("row").toInt(), action->data().toMap().value("column").toInt());
        if (index.isValid()){
            model()->setData(index, action->data().toMap());
            edit(index);
        }
    }
}

void QFilterView::changeColor()
{
    QVariantMap properties = currentIndex().data(Qt::EditRole).toMap();
    if (!properties.isEmpty()){
        QColor color = qvariant_cast<QColor>(properties.value("highlightColor"));
        color = QColorDialog::getColor(color, this, tr("Select Highlight Color"));
        if (color.isValid()){
            properties["highlightColor"] = color;
            model()->setData(currentIndex(), properties);
        }
    }
}

void QFilterView::changeProperties()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action){
        QModelIndex index = model()->index(action->data().toMap().value("row").toInt(), action->data().toMap().value("column").toInt());
        if (index.isValid()){
            model()->setData(index, action->data().toMap());
        }
    }
}

void QFilterView::disableSelectedFilters()
{
    QVariantMap properties;
    Q_FOREACH(QModelIndex index, selectionModel()->selectedIndexes()){
        properties = index.data(Qt::EditRole).toMap();
        if (!properties.isEmpty()){
            properties["enabled"] = false;
            model()->setData(index, properties);
        }
    }
}

void QFilterView::enableSelectedFilters()
{
    QVariantMap properties;
    Q_FOREACH(QModelIndex index, selectionModel()->selectedIndexes()){
        properties = index.data(Qt::EditRole).toMap();
        if (!properties.isEmpty()){
            properties["enabled"] = true;
            model()->setData(index, properties);
        }
    }
}

void QFilterView::contextMenuEvent(QContextMenuEvent* event)
{
    QAbstractFilterModel* m = qobject_cast<QAbstractFilterModel*>(model());
    if (m == 0){
        event->ignore();
        return;
    }
    if (selectedIndexes().isEmpty()){
        event->ignore();
        return;
    }
    QAction* action = 0;
    QMenu* menu = new QMenu(this);

    if (selectedIndexes().size() > 1){
        menu->addAction(QIcon(":/qaiv/filter/enabled"), tr("Enable Filter"), this, SLOT(enableSelectedFilters()));
        menu->addAction(QIcon(":/qaiv/filter/disabled"), tr("Disable Filter"), this, SLOT(disableSelectedFilters()));
        menu->addSeparator();
        menu->addAction(QIcon(":/qaiv/filter/toggle"), tr("Toggle Filter"), this, SLOT(toggleSelectedFilters()));
    } else {
        QVariantMap properties = selectedIndexes().first().data(Qt::EditRole).toMap();
        if (properties.isEmpty()){
            properties["row"] = selectedIndexes().first().row();
            properties["column"] = selectedIndexes().first().column();
            properties["enabled"] = true;
            QVariantList mTypes = selectedIndexes().first().data(QAbstractFilterModel::ColumnFilterTypesRole).toList();
            Q_FOREACH(QAbstractFilterModel::FilterTypeEntry entry , m->registeredFilterTypes()){
                if (entry.type == QAbstractFilter::Type){
                    menu->addSeparator();
                } else{
                    if (mTypes.isEmpty() || mTypes.contains(entry.type)){
                        action = menu->addAction(entry.icon, entry.text, this, SLOT(addFilter()));
                        properties["type"] = entry.type;
                        action->setData(properties);
                        action->setToolTip(entry.tooltip);
                    }
                }
            }
        } else {
            m->filter(selectedIndexes().first())->addContextMenuActions(menu, this);
            if (!menu->isEmpty()){
                menu->addSeparator();
            }
            if (properties.value("enabled").toBool()){
                action = menu->addAction(QIcon(":/qaiv/filter/disabled"), tr("Disable Filter"), this, SLOT(changeProperties()));
                properties["enabled"] = false;
            } else {
                action = menu->addAction(QIcon(":/qaiv/filter/enabled"), tr("Enable Filter"), this, SLOT(changeProperties()));
                properties["enabled"] = true;
            }
            action->setData(properties);
            if (m->mode() == QAdvancedItemViews::HighlightMode){
                menu->addSeparator();
                QPixmap pixmap(22, 22);
                pixmap.fill(qvariant_cast<QColor>(properties.value("highlightColor")));
                action = menu->addAction(tr("Change Color..."), this, SLOT(changeColor()));
                action->setIcon(QIcon(pixmap));
                action->setData(properties);
            }
            menu->addSeparator();
            action = menu->addAction(tr("Remove filter"), this, SLOT(removeFilter()));
            action->setData(properties);
        }
    }

    //        QMenu* mMenu = mModel->createStandardContextMenu(selectionModel()->selectedIndexes(), this);
    menu->exec(viewport()->mapToGlobal(event->pos()));
    event->accept();
    delete menu;
}

void QFilterView::focusInEvent(QFocusEvent *event)
{
    QTableView::focusInEvent(event);
    emit focusReceived();
}

int QFilterView::defaultFilterType() const
{
    return d->defaultFilterType;
}

QFilterViewConnector* QFilterView::filterViewConnector() const
{
    return d->filterViewConnector;
}

bool QFilterView::filterVisible() const
{
    return d->filterVisible;
}

int QFilterView::maxVisibileFilterSets() const
{
    return d->maxFilterVisible;
}

void QFilterView::mousePressEvent( QMouseEvent* event )
{
    QModelIndex index = indexAt(event->pos());
    if (event->button() == Qt::LeftButton && index.isValid()){
        QRect r = visualRect(index);
        r.setWidth(16);
        if (r.contains(event->pos())){
            event->accept();
        } else {
            QTableView::mousePressEvent(event);
        }
    } else {
        QTableView::mousePressEvent(event);
    }
}

void QFilterView::hideFilter()
{
    setFilterVisible(false);
}

void QFilterView::setDefaultFilterType(int type)
{
    d->defaultFilterType = type;
}

void QFilterView::setMaxVisibileFilterSets(int rows)
{
    d->maxFilterVisible = rows;
    emit calcGeometryRequested();
}

void QFilterView::setModel(QAbstractItemModel* model)
{
    QTableView::setModel(model);
    connect(model, SIGNAL(modelReset()), this, SIGNAL(calcGeometryRequested()));
    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(calcGeometryRequested()));
    connect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(calcGeometryRequested()));
//    calcGeometry();
    updateGeometry();
}

void QFilterView::showFilter()
{
    setFilterVisible(true);
}

void QFilterView::updateGeometry()
{
    if (model() == 0){
        setFixedHeight(horizontalHeader()->sizeHint().height());
    } else {
        int rows = model()->rowCount();
        if (rows > maxVisibileFilterSets()){
            rows = maxVisibileFilterSets();
        }
        for (int iRow = 0; iRow < model()->rowCount(); iRow++){
            verticalHeader()->resizeSection(iRow, verticalHeader()->sizeHint().height());
        }
        int rowHeight = verticalHeader()->sizeHint().height();
        if (rowHeight == 0){
            rowHeight = horizontalHeader()->sizeHint().height();
        }
        int headerHeight = horizontalHeader()->sizeHint().height();
        if (headerHeight == 0){
            headerHeight = rowHeight;
        }
        if (filterVisible()){
            setFixedHeight(headerHeight + rowHeight * rows + (1 * rows) + 1);
        } else {
            setFixedHeight(horizontalHeader()->sizeHint().height());
        }
    }
}

void QFilterView::removeFilter()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action){
        QModelIndex index = model()->index(action->data().toMap().value("row").toInt(), action->data().toMap().value("column").toInt());
        if (index.isValid()){
            model()->setData(index, QVariantMap());
        }
    }
}

void QFilterView::setFilterVisible(bool visible)
{
    if (visible != d->filterVisible){
        d->filterVisible = visible;
        emit visibilityChanged(d->filterVisible);
    }
}

void QFilterView::toggleSelectedFilters()
{
    QVariantMap properties;
    Q_FOREACH(QModelIndex index, selectionModel()->selectedIndexes()){
        properties = index.data(Qt::EditRole).toMap();
        if (!properties.isEmpty()){
            properties["enabled"] = !properties.value("enabled").toBool();
            model()->setData(index, properties);
        }
    }
}
