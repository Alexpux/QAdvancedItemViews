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

#include "qadvancedtableview.h"

#include "qabstractfilterproxymodel.h"
#include "qadvancedheaderview.h"
#include "qadvancedmultilevelheaderview.h"
#include "qadvancedtableview_p.h"
#include "qadvancedtableviewproxy.h"
#include "qfiltermodel.h"
#include "qfiltermodelproxy.h"
#include "qfilterviewitemdelegate.h"
#include "qfixedrowstableview.h"
#include "qshareditemselectionmodel.h"
#include "ui_qadvancedtableview.h"

#include <QContextMenuEvent>
#include <QScrollBar>
#include <QTableView>
#include <QTimer>
#include <algorithm>

#define V_CALL(_m_)              \
    ui->dataTableView->_m_;      \
    ui->fixedRowsTableView->_m_; \
    ui->splittedDataTableView->_m_;

// SINGLE POINT OF SYNCHRONIZATION: This is the ONLY place where column
// properties are applied to all views. This eliminates code duplication
// and ensures perfect consistency.
void QAdvancedTableView::syncColumnProperties(int column, int size, bool hidden)
{
    if (column < 0) {
        return;
    }

    // Lambda to apply properties to a single header view
    auto applyToView = [=](QHeaderView *header, int columnIndex) {
        if (!header || columnIndex >= header->count()) {
            return;
        }
        header->resizeSection(columnIndex, size);
        header->setSectionHidden(columnIndex, hidden);
    };

    // Apply to all data views (same column index)
    applyToView(ui->dataTableView->horizontalHeader(), column);
    applyToView(ui->fixedRowsTableView->horizontalHeader(), column);
    applyToView(ui->splittedDataTableView->horizontalHeader(), column);

    if (d->summaryHeader) {
        applyToView(d->summaryHeader, column); // +1 offset for extra section
    }
}

// Synchronize ALL columns from headerTableView to other views
void QAdvancedTableView::syncAllColumns()
{
    const auto *header = ui->headerTableView->horizontalHeader();
    if (!header) {
        return;
    }

    const int columnCount = header->count();
    for (int i = 0; i < columnCount; ++i) {
        syncColumnProperties(i, header->sectionSize(i), header->isSectionHidden(i));
    }
}

// Capture current state of all columns
std::vector<ColumnState> QAdvancedTableView::captureColumnStates() const
{
    std::vector<ColumnState> states;
    const auto *header = ui->headerTableView->horizontalHeader();

    if (!header) {
        return states;
    }

    const int count = header->count();
    states.reserve(count);

    for (int i = 0; i < count; ++i) {
        ColumnState state;
        state.index = i;
        state.size = header->sectionSize(i);
        state.hidden = header->isSectionHidden(i);
        state.visualIndex = header->visualIndex(i);
        states.push_back(state);
    }

    return states;
}

// Apply captured column states
void QAdvancedTableView::applyColumnStates(const std::vector<ColumnState> &states)
{
    if (states.empty()) {
        return;
    }

    // Use RAII guard for batch operation
    BatchSyncGuard guard(this);

    auto *header = ui->headerTableView->horizontalHeader();
    if (!header) {
        return;
    }

    // Apply all states to header view first
    for (const auto &state : states) {
        if (!state.isValid() || state.index >= header->count()) {
            continue;
        }
        header->resizeSection(state.index, state.size);
        header->setSectionHidden(state.index, state.hidden);
    }

    // After guard destruction, sync to all views
}

//-----------------------------------------------
// class QAdvancedTableView
//-----------------------------------------------

QAdvancedTableView::QAdvancedTableView(QWidget *parent) :
    QWidget(parent),
    d(new QAdvancedTableViewPrivate(this)),
    ui(new Ui::QAdvancedTableView)
{
    ui->setupUi(this);

    ui->splittedDataTableView->hide();

    // Create header view (model) proxy
    setFilterModel(new QFilterModel(this));
    // d->filterModel = new QFilterModel(this);
    //  Create horizontal header view
    auto *hHeader = new QAdvancedMultiLevelHeaderView(Qt::Horizontal, this);
    hHeader->setRootLevelVisible(false);
    d->horizontalHeader = hHeader;
    ui->headerTableView->setHorizontalHeader(d->horizontalHeader);
    ui->headerTableView->setItemDelegate(new QFilterViewItemDelegate(this));
    ui->headerTableView->horizontalHeader()->setSectionsMovable(true);
    // Create vertical header views
    d->verticalHeader = new QAdvancedHeaderView(Qt::Vertical, this);
    ui->headerTableView->setVerticalHeader(d->verticalHeader);
    // ui->headerTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // ui->dataTableView->setVerticalHeader(new QFixedRowsHeaderView(Qt::Vertical, this));

    // Replace the current summaryView setup:
    d->summaryWidget = new QWidget(this);
    d->summaryWidget->setMinimumHeight(25);
    d->summaryWidget->setMaximumHeight(25);

    QHBoxLayout *summaryLayout = new QHBoxLayout(d->summaryWidget);
    summaryLayout->setContentsMargins(0, 0, 0, 0);
    summaryLayout->setSpacing(0);

    // Create the button
    d->summaryButton = new QPushButton(d->summaryWidget);
    d->summaryButton->setFixedWidth(verticalHeader()->width()); // Match vertical header width
    d->summaryButton->setText("Σ"); // Or use an icon
    d->summaryButton->setToolTip(tr("Summary actions"));
    // Control how the button behaves when parent resizes
    d->summaryButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    d->summaryButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    d->summaryButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(d->summaryButton, &QPushButton::clicked, this, &QAdvancedTableView::summaryButtonClicked);

    // Create the header for summary data
    d->summaryHeader = new QHeaderView(Qt::Horizontal, d->summaryWidget);
    d->summaryHeader->setSectionResizeMode(QHeaderView::Fixed);
    QFont fnt = d->summaryHeader->font();
    fnt.setBold(true);
    d->summaryHeader->setFont(fnt);

    summaryLayout->addWidget(d->summaryButton);
    summaryLayout->addWidget(d->summaryHeader);

    ui->verticalLayout->addWidget(d->summaryWidget);
    d->summaryWidget->setVisible(false);

    // set models
    d->dataViewProxy->setSourceModel(ui->fixedRowsTableView->decorationProxy());
    ui->dataTableView->setModel(d->dataViewProxy);
    ui->splittedDataTableView->setModel(d->dataViewProxy);

    // set selection models
    ui->splittedDataTableView->setSelectionModel(new QSharedItemSelectionModel(ui->splittedDataTableView->model(), ui->dataTableView->selectionModel(), this));
    ui->fixedRowsTableView->setSelectionModel(new QSharedItemSelectionModel(ui->fixedRowsTableView->model(), ui->dataTableView->selectionModel(), this));

    // data table view
    connect(ui->dataTableView->verticalHeader(), &QHeaderView::sectionClicked, this, &QAdvancedTableView::verticalHeaderSectionClicked);
    connect(ui->dataTableView->horizontalScrollBar(), &QScrollBar::sliderMoved, this, &QAdvancedTableView::dataViewHorizontalScrollBarSilderMoved);
    connect(ui->dataTableView->horizontalScrollBar(), &QScrollBar::rangeChanged, this, &QAdvancedTableView::dataViewHorizontalScrollBarRangeChanged);
    connect(ui->dataTableView->horizontalScrollBar(), &QScrollBar::valueChanged, this, &QAdvancedTableView::dataViewHorizontalScrollBarValueChanged);
    connect(ui->dataTableView->verticalScrollBar(), &QScrollBar::rangeChanged, this, &QAdvancedTableView::updateHeaderViewVerticalScrollBar);
    connect(ui->dataTableView->verticalHeader(), &QHeaderView::sectionResized, this, &QAdvancedTableView::verticalHeaderSectionResized);
    connect(ui->dataTableView, &QAdvancedTableViewProxy::focusReceived, this, &QAdvancedTableView::subviewReceivedFocus);

    // splitted data table view view
    connect(ui->splittedDataTableView, &QAdvancedTableViewProxy::focusReceived, this, &QAdvancedTableView::subviewReceivedFocus);
    connect(ui->splittedDataTableView->verticalHeader(), &QHeaderView::sectionResized, this, &QAdvancedTableView::verticalHeaderSectionResized);
    connect(ui->splittedDataTableView->verticalHeader(), &QHeaderView::sectionClicked, this, &QAdvancedTableView::verticalHeaderSectionClicked);

    // fixed rows view
    connect(ui->fixedRowsTableView, &QFixedRowsTableView::focusReceived, this, &QAdvancedTableView::subviewReceivedFocus);

    // filter model
    connect(d->filterModel, &QAbstractFilterModel::modelReset, this, &QAdvancedTableView::updateHeaderViewGeometries);
    connect(d->filterModel, &QAbstractFilterModel::rowsRemoved, this, &QAdvancedTableView::updateHeaderViewGeometries);

    // data view proxy
    connect(d->dataViewProxy, &QAbstractFilterProxyModel::modelReset, this, &QAdvancedTableView::modelReset);
    connect(d->dataViewProxy, &QAbstractFilterProxyModel::layoutChanged, this, &QAdvancedTableView::dataModelLayoutChanged);

    // header table view
    connect(ui->headerTableView, &QFilterView::cornerButtonClicked, this, &QAdvancedTableView::selectAll);
    connect(ui->headerTableView, &QFilterView::calcGeometryRequested, this, &QAdvancedTableView::updateHeaderViewGeometries);
    connect(ui->headerTableView, &QFilterView::visibilityChanged, this, &QAdvancedTableView::updateHeaderViewGeometries);
    connect(ui->headerTableView, &QFilterView::focusReceived, this, &QAdvancedTableView::subviewReceivedFocus);
    //
    connect(ui->headerTableView->model(), &QAbstractItemModel::rowsInserted, this, &QAdvancedTableView::filterAdded);
    connect(ui->headerTableView->horizontalHeader(), &QHeaderView::sectionResized, this, &QAdvancedTableView::headerViewSectionResized);
    connect(ui->headerTableView->horizontalHeader(), &QHeaderView::sectionMoved, this, &QAdvancedTableView::horizontalHeaderViewSectionMoved);
    connect(ui->headerTableView->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, &QAdvancedTableView::horizontalHeaderSortIndicatorChanged);
    connect(ui->headerTableView->horizontalScrollBar(), &QScrollBar::rangeChanged, this, &QAdvancedTableView::headerViewHorizontalScrollBarRangeChanged);
    connect(ui->headerTableView->horizontalScrollBar(), &QScrollBar::sliderMoved, this, &QAdvancedTableView::headerViewHorizontalScrollBarSilderMoved);
    connect(ui->headerTableView->horizontalScrollBar(), &QScrollBar::valueChanged, this, &QAdvancedTableView::headerViewHorizontalScrollBarValueChanged);

    // connect(ui->headerTableView, &QFilterView::verticalHeaderWidthChangeRequested, this, &QAdvancedTableView::verticalHeaderWidthChangeRequested);
    //  Forward data view signals
    connect(ui->dataTableView, &QAdvancedTableViewProxy::activated, this, &QAdvancedTableView::viewActivated);
    connect(ui->dataTableView, &QAdvancedTableViewProxy::clicked, this, &QAdvancedTableView::viewClicked);
    connect(ui->dataTableView, &QAdvancedTableViewProxy::customContextMenuRequested, this, &QAdvancedTableView::customContextMenuRequested);
    connect(ui->dataTableView, &QAdvancedTableViewProxy::doubleClicked, this, &QAdvancedTableView::viewDoubleClicked);
    connect(ui->dataTableView, &QAdvancedTableViewProxy::entered, this, &QAdvancedTableView::viewEntered);
    connect(ui->dataTableView, &QAdvancedTableViewProxy::pressed, this, &QAdvancedTableView::viewPressed);
    connect(ui->dataTableView, &QAdvancedTableViewProxy::viewportEntered, this, &QAdvancedTableView::viewportEntered);

    // Forward fixed rows view signals
    connect(ui->fixedRowsTableView, &QFixedRowsTableView::activated, this, &QAdvancedTableView::viewActivated);
    connect(ui->fixedRowsTableView, &QFixedRowsTableView::clicked, this, &QAdvancedTableView::viewClicked);
    connect(ui->fixedRowsTableView, &QFixedRowsTableView::customContextMenuRequested, this, &QAdvancedTableView::customContextMenuRequested);
    connect(ui->fixedRowsTableView, &QFixedRowsTableView::doubleClicked, this, &QAdvancedTableView::viewDoubleClicked);
    connect(ui->fixedRowsTableView, &QFixedRowsTableView::entered, this, &QAdvancedTableView::viewEntered);
    connect(ui->fixedRowsTableView, &QFixedRowsTableView::pressed, this, &QAdvancedTableView::viewPressed);
    connect(ui->fixedRowsTableView, &QFixedRowsTableView::viewportEntered, this, &QAdvancedTableView::viewportEntered);

    // Forward splitted data table view signals
    connect(ui->splittedDataTableView, &QAdvancedTableViewProxy::activated, this, &QAdvancedTableView::viewActivated);
    connect(ui->splittedDataTableView, &QAdvancedTableViewProxy::clicked, this, &QAdvancedTableView::viewClicked);
    connect(ui->splittedDataTableView, &QAdvancedTableViewProxy::customContextMenuRequested, this, &QAdvancedTableView::customContextMenuRequested);
    connect(ui->splittedDataTableView, &QAdvancedTableViewProxy::doubleClicked, this, &QAdvancedTableView::viewDoubleClicked);
    connect(ui->splittedDataTableView, &QAdvancedTableViewProxy::entered, this, &QAdvancedTableView::viewEntered);
    connect(ui->splittedDataTableView, &QAdvancedTableViewProxy::pressed, this, &QAdvancedTableView::viewPressed);
    connect(ui->splittedDataTableView, &QAdvancedTableViewProxy::viewportEntered, this, &QAdvancedTableView::viewportEntered);

    connect(d->horizontalHeader, &QAdvancedHeaderView::visibleSectionsCountChanged, this, &QAdvancedTableView::viewLayoutChangedImpl);
    // Install event filter
    ui->dataTableView->verticalHeader()->installEventFilter(this);
    updateHeaderViewGeometries();
    //
    setFocusProxy(ui->dataTableView);
}

QAdvancedTableView::~QAdvancedTableView()
{
    delete d;
    delete ui;
}

void QAdvancedTableView::addFilterGroup()
{
    d->filterModel->insertRows(d->filterModel->rowCount(), 1);
}

bool QAdvancedTableView::alternatingRowColors() const
{
    return ui->dataTableView->alternatingRowColors();
}

int QAdvancedTableView::columnAt(int x) const
{
    return ui->dataTableView->columnAt(x);
}

QVariantList QAdvancedTableView::columnsFilterTypes(int column) const
{
    return d->filterModel->index(0, column).data(QAbstractFilterModel::ColumnFilterTypesRole).toList();
}

int QAdvancedTableView::columnWidth(int column) const
{
    return ui->headerTableView->columnWidth(column);
}

void QAdvancedTableView::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex i;
    if (ui->dataTableView->viewport()->rect().contains(event->pos())) {
        i = ui->dataTableView->indexAt(ui->dataTableView->viewport()->mapFromParent(event->pos()));
    } else if (ui->fixedRowsTableView->viewport()->rect().contains(event->pos())) {
        i = ui->fixedRowsTableView->indexAt(event->pos());
    }
    QMenu *m = createStandardContextMenu(i);

    m->exec(event->globalPos());
    delete m;
}

QMenu *QAdvancedTableView::createStandardContextMenu(const QModelIndex &index)
{
    Q_UNUSED(index)
    QAction *a { nullptr };
    QMenu *m = new QMenu();
    m = m->addMenu(tr("View"));
    a = m->addAction(tr("Splitted View"));
    a->setCheckable(true);
    a->setChecked(viewSplitted());
    connect(a, &QAction::toggled, this, &QAdvancedTableView::splitView);

    a = m->addAction(tr("Fixed Rows"));
    a->setCheckable(true);
    a->setChecked(showFixedRows());
    connect(a, &QAction::toggled, this, &QAdvancedTableView::setShowFixedRows);

    m->addSeparator();

    a = m->addAction(tr("Show Filter"));
    a->setCheckable(true);
    a->setChecked(showFilter());
    connect(a, &QAction::toggled, this, &QAdvancedTableView::setShowFilter);

    m->addSeparator();

    a = m->addAction(tr("Show Grid"));
    a->setCheckable(true);
    a->setChecked(showGrid());

    connect(a, &QAction::toggled, this, &QAdvancedTableView::setShowGrid);

    return m;
}

void QAdvancedTableView::clearSelection()
{
    ui->dataTableView->clearSelection();
}

QModelIndex QAdvancedTableView::currentIndex() const
{
    if (ui->fixedRowsTableView->hasFocus()) {
        return mapToSource(ui->fixedRowsTableView->currentIndex());
    }
    if (ui->splittedDataTableView->hasFocus()) {
        return mapToSource(ui->splittedDataTableView->currentIndex());
    }
    return mapToSource(ui->dataTableView->currentIndex());
}

int QAdvancedTableView::defaultFilterType(int column) const
{
    return d->filterModel->index(0, column).data(QAbstractFilterModel::DefaultFilterTypeRole).toInt();
}

void QAdvancedTableView::dataViewHorizontalScrollBarRangeChanged(int minimum, int maximum)
{
    ui->headerTableView->horizontalScrollBar()->setRange(minimum, maximum);
    ui->splittedDataTableView->horizontalScrollBar()->setRange(minimum, maximum);
    ui->fixedRowsTableView->horizontalScrollBar()->setRange(minimum, maximum);
}

void QAdvancedTableView::dataViewHorizontalScrollBarSilderMoved(int value)
{
    ui->headerTableView->horizontalScrollBar()->setValue(value);
    ui->splittedDataTableView->horizontalScrollBar()->setValue(value);
    ui->fixedRowsTableView->horizontalScrollBar()->setValue(value);
}

void QAdvancedTableView::dataViewHorizontalScrollBarValueChanged(int value)
{
    ui->headerTableView->horizontalScrollBar()->setValue(value);
    ui->splittedDataTableView->horizontalScrollBar()->setValue(value);
    ui->fixedRowsTableView->horizontalScrollBar()->setValue(value);
}

bool QAdvancedTableView::dragEnabled() const
{
    return ui->dataTableView->dragEnabled();
}

void QAdvancedTableView::edit(const QModelIndex &index)
{
    if (ui->fixedRowsTableView->hasFocus()) {
        ui->fixedRowsTableView->edit(index);
    } else if (ui->splittedDataTableView->hasFocus()) {
        ui->splittedDataTableView->edit(index);
    } else {
        ui->dataTableView->edit(index);
    }
}

QAbstractItemView::EditTriggers QAdvancedTableView::editTriggers() const
{
    return ui->dataTableView->editTriggers();
}

bool QAdvancedTableView::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Hide) {
        if (obj == ui->dataTableView->verticalHeader()) {
            ui->fixedRowsTableView->verticalHeader()->setVisible(false);
            ui->headerTableView->verticalHeader()->setVisible(false);
            ui->splittedDataTableView->verticalHeader()->setVisible(false);
        }
    }
    return QObject::eventFilter(obj, event);
}

QAbstractFilter *QAdvancedTableView::filterAt(int row, int col) const
{
    return d->filterModel->filter(d->filterModel->index(row, col));
}

void QAdvancedTableView::filterAdded(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);
    updateHeaderViewGeometries();
}

QIcon QAdvancedTableView::filterDisabledIcon() const
{
    return d->filterModel->filterDisabledIcon();
}

QIcon QAdvancedTableView::filterEnabledIcon() const
{
    return d->filterModel->filterEnabledIcon();
}

void QAdvancedTableView::headerViewHorizontalScrollBarRangeChanged(int min, int max)
{
    Q_UNUSED(min);
    Q_UNUSED(max);
    // ui->dataTableView->horizontalScrollBar()->setRange(min, max);
}

QAbstractFilterModel *QAdvancedTableView::filterModel() const
{
    return d->filterModel;
}

QAbstractFilterProxyModel *QAdvancedTableView::filterProxyModel() const
{
    return d->dataViewProxy;
}

bool QAdvancedTableView::showFilter() const
{
    return ui->headerTableView->filterVisible();
}

QModelIndexList QAdvancedTableView::fixedRows(int column) const
{
    QModelIndexList l;
    for (int i = 0; i < ui->fixedRowsTableView->model()->rowCount(); i++) {
        l << qSourceIndex(ui->fixedRowsTableView->model()->index(i, column));
    }
    return l;
}

Qt::PenStyle QAdvancedTableView::gridStyle() const
{
    return ui->dataTableView->gridStyle();
}

QSize QAdvancedTableView::iconSize() const
{
    return ui->dataTableView->iconSize();
}

bool QAdvancedTableView::hasAutoScroll() const
{
    return ui->dataTableView->hasAutoScroll();
}

void QAdvancedTableView::headerViewHorizontalScrollBarSilderMoved(int value)
{
    ui->dataTableView->horizontalScrollBar()->setValue(value);
    ui->fixedRowsTableView->horizontalScrollBar()->setValue(value);
    ui->splittedDataTableView->horizontalScrollBar()->setValue(value);
}

void QAdvancedTableView::headerViewHorizontalScrollBarValueChanged(int value)
{
    ui->dataTableView->horizontalScrollBar()->setValue(value);
    ui->fixedRowsTableView->horizontalScrollBar()->setValue(value);
    ui->splittedDataTableView->horizontalScrollBar()->setValue(value);
}

void QAdvancedTableView::headerViewSectionResized(int logicalIndex, int oldSize, int newSize)
{
    // Skip synchronization during batch operations
    if (d->isSyncSuppressed()) {
        return;
    }

    // Use unified synchronization for individual column changes
    const bool hidden = (newSize == 0);
    const int size = (newSize == 0 && oldSize > 0) ? 0 : newSize;

    syncColumnProperties(logicalIndex, size, hidden);
}

void QAdvancedTableView::hideColumn(int column)
{
    ui->headerTableView->horizontalHeader()->hideSection(ui->headerTableView->horizontalHeader()->logicalIndex(column));
}

void QAdvancedTableView::hideColumns(const QStringList &names)
{
    for (int i = 0; i < horizontalHeader()->count(); i++) {
        horizontalHeader()->setSectionHidden(i, names.contains(horizontalHeader()->model()->headerData(i, Qt::Horizontal).toString()));
    }
}

void QAdvancedTableView::hideFilterView()
{
    setShowFilter(false);
}

void QAdvancedTableView::hideRow(int row) {
    V_CALL(hideRow(row))
}

QHeaderView *QAdvancedTableView::horizontalHeader() const
{
    return ui->headerTableView->horizontalHeader();
}

void QAdvancedTableView::horizontalHeaderViewSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    Q_UNUSED(logicalIndex);
    ui->dataTableView->horizontalHeader()->moveSection(oldVisualIndex, newVisualIndex);
    ui->fixedRowsTableView->horizontalHeader()->moveSection(oldVisualIndex, newVisualIndex);
    ui->splittedDataTableView->horizontalHeader()->moveSection(oldVisualIndex, newVisualIndex);
    if (d->summaryHeader->isVisible()) {
        d->summaryHeader->moveSection(oldVisualIndex, newVisualIndex);
    }
}

void QAdvancedTableView::horizontalHeaderSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    ui->dataTableView->sortByColumn(logicalIndex, order);
    ui->fixedRowsTableView->sortByColumn(logicalIndex, order);
    ui->splittedDataTableView->sortByColumn(logicalIndex, order);
}

QModelIndex QAdvancedTableView::indexAt(const QPoint &point) const
{
    if (focusProxy() == ui->splittedDataTableView) {
        return mapToSource(ui->splittedDataTableView->indexAt(point));
    }
    if (focusProxy() == ui->fixedRowsTableView) {
        return mapToSource(ui->fixedRowsTableView->indexAt(point));
    }
    return mapToSource(ui->dataTableView->indexAt(point));
}

bool QAdvancedTableView::isColumnHidden(int column) const
{
    return ui->dataTableView->isColumnHidden(column);
}

bool QAdvancedTableView::isRowHidden(int row) const
{
    return ui->dataTableView->isRowHidden(row);
}

void QAdvancedTableView::setRowHidden(int row, bool hide)
{
    if (focusProxy() == ui->splittedDataTableView) {
        ui->splittedDataTableView->setRowHidden(row, hide);
    } else if (focusProxy() == ui->fixedRowsTableView) {
        ui->fixedRowsTableView->setRowHidden(row, hide);
    } else {
        ui->dataTableView->setRowHidden(row, hide);
    }
}

bool QAdvancedTableView::isSortIndicatorShown() const
{
    return ui->headerTableView->horizontalHeader()->isSortIndicatorShown();
}

bool QAdvancedTableView::isSortingEnabled() const
{
    return ui->dataTableView->isSortingEnabled();
}

bool QAdvancedTableView::viewSplitted() const
{
    return ui->splittedDataTableView->isVisible();
}

QAbstractItemDelegate *QAdvancedTableView::itemDelegate() const
{
    return ui->dataTableView->itemDelegate();
}

QAbstractItemDelegate *QAdvancedTableView::itemDelegateForColumn(int column) const
{
    return ui->dataTableView->itemDelegateForColumn(column);
}

QAbstractItemDelegate *QAdvancedTableView::itemDelegateForRow(int row) const
{
    return ui->dataTableView->itemDelegateForRow(row);
}

QModelIndex QAdvancedTableView::mapToSource(const QModelIndex &index) const
{
    QModelIndex i(index);
    const QAbstractProxyModel *p { nullptr };
    while (i.model() != d->model && (p = qobject_cast<const QAbstractProxyModel *>(i.model()))) {
        i = p->mapToSource(i);
    }
    return i;
}

int QAdvancedTableView::getHeaderSectionWidth(QHeaderView *header, int column)
{
    if (!header || !header->model() || column >= header->model()->columnCount()) {
        return 0;
    }

    QString headerText = header->model()->headerData(column, header->orientation(), Qt::DisplayRole).toString();

    QVariant decoration = header->model()->headerData(column, header->orientation(), Qt::DecorationRole);

    int textWidth = 0;
    int iconWidth = 0;
    int spacing = 0;
    QFontMetrics fm = header->fontMetrics();

    QStringList lines = headerText.split('\n');
    if (!headerText.isEmpty()) {
        for (const QString &line : lines) {
            int lineWidth = fm.horizontalAdvance(line);
            textWidth = std::max(lineWidth, textWidth);
        }
    }

    if (decoration.isValid()) {
        QSize icon_size;

        if (decoration.canConvert<QIcon>()) {
            QIcon icon = decoration.value<QIcon>();
            int iconExtent = style()->pixelMetric(QStyle::PM_SmallIconSize);
            icon_size = icon.actualSize(QSize(iconExtent, iconExtent));
        } else if (decoration.canConvert<QPixmap>()) {
            QPixmap pixmap = decoration.value<QPixmap>();
            icon_size = pixmap.size();
        } else if (decoration.canConvert<QImage>()) {
            QImage image = decoration.value<QImage>();
            icon_size = image.size();
        }

        iconWidth = icon_size.width();
    }

    // Spacing between text and icon
    if (textWidth > 0 && iconWidth > 0) {
        spacing = style()->pixelMetric(QStyle::PM_HeaderMargin);
    }

    int padding = 0;

    // Base header margin (applied to both sides)
    padding += 2 * style()->pixelMetric(QStyle::PM_HeaderMargin);

    // Frame width
    padding += 2 * style()->pixelMetric(QStyle::PM_DefaultFrameWidth);

    // Sort indicator space
    if (header->isSortIndicatorShown()) {
        padding += style()->pixelMetric(QStyle::PM_HeaderMarkSize);
    }

    return textWidth + spacing + iconWidth + padding;
}

QSize QAdvancedTableView::minimumSizeHint() const
{
    return QSize(QWidget::minimumSizeHint().width(), ui->headerTableView->height() * 2);
}

QAbstractItemModel *QAdvancedTableView::model() const
{
    return d->model;
}

bool QAdvancedTableView::showFixedRows() const
{
    return ui->fixedRowsTableView->decorationProxy()->isEnabled();
}

void QAdvancedTableView::modelReset()
{
    if (d->autoResizeRowsToContents) {
        ui->dataTableView->resizeRowsToContents();
        ui->fixedRowsTableView->resizeRowsToContents();
        ui->splittedDataTableView->resizeRowsToContents();
    }
}

bool QAdvancedTableView::restoreFilter(const QByteArray &data)
{
    QByteArray mData(data);
    QDataStream mStream(&mData, QIODevice::ReadOnly);
    if (mStream.atEnd()) {
        return false;
    }
    qint32 mCol { 0 };
    qint32 mRow { 0 };
    qint32 mRows { 0 };
    QString mName {};
    QVariantMap mProperties;
    // Clear current filter model
    d->filterModel->removeRows(0, d->filterModel->rowCount());
    mStream >> mRows;
    d->filterModel->insertRows(0, mRows);
    for (int iRow = 0; iRow < mRows; iRow++) {
        mStream >> mName;
        d->filterModel->setHeaderData(iRow, Qt::Vertical, mName);
    }
    while (!mStream.atEnd()) {
        mStream >> mRow >> mCol >> mProperties;
        if (d->filterModel->rowCount() < mRow) {
            d->filterModel->insertRows(d->filterModel->rowCount(), 1);
        }
        d->filterModel->setData(d->filterModel->index(mRow, mCol), mProperties);
    }
    return true;
}

bool QAdvancedTableView::restoreState(const QByteArray &data)
{
    QByteArray stateArr;
    QByteArray ds(data);
    bool splitted { false };
    bool fixed { false };
    bool filter { false };
    bool grid { false };
    QDataStream stream(&ds, QIODevice::ReadOnly);
    if (stream.atEnd()) {
        return false;
    }
    stream >> stateArr >> splitted >> fixed >> filter >> grid;
    if (ui->headerTableView->horizontalHeader()->restoreState(stateArr)) {
        setShowFixedRows(fixed);
        setShowFilter(filter);
        setShowGrid(grid);
        syncAllColumns(); // UNIFIED SYNC: Use unified synchronization
        ui->headerTableView->viewport()->update();
        ui->dataTableView->horizontalHeader()->restoreState(stateArr);
        ui->fixedRowsTableView->horizontalHeader()->restoreState(stateArr);
        ui->splittedDataTableView->horizontalHeader()->restoreState(stateArr);
    }
    return false;
}

QModelIndex QAdvancedTableView::rootIndex() const
{
    return ui->dataTableView->rootIndex();
}

int QAdvancedTableView::rowAt(int y) const
{
    if (ui->splittedDataTableView->isVisible()) {
        int r = ui->splittedDataTableView->rowAt(y);
        if (r > -1) {
            return r;
        }
    }
    return ui->dataTableView->rowAt(y);
}

int QAdvancedTableView::rowHeight(int row) const
{
    return ui->dataTableView->rowHeight(row);
}

void QAdvancedTableView::reset()
{
    V_CALL(reset())
}

void QAdvancedTableView::resizeColumnToContents(int column)
{
    ui->headerTableView->resizeColumnToContents(column);
}

void QAdvancedTableView::resizeColumnsToContents()
{
    ui->headerTableView->resizeColumnsToContents();
}

void QAdvancedTableView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    viewLayoutChanged();
}

void QAdvancedTableView::resizeRowToContents(int row)
{
    ui->dataTableView->resizeRowToContents(row);
    ui->fixedRowsTableView->resizeRowToContents(row);
    ui->splittedDataTableView->resizeRowToContents(row);
}

void QAdvancedTableView::resizeRowsToContents()
{
    ui->dataTableView->resizeRowsToContents();
    ui->fixedRowsTableView->resizeRowsToContents();
    ui->splittedDataTableView->resizeRowsToContents();
}

QByteArray QAdvancedTableView::saveFilter() const
{
    QByteArray mData;
    QDataStream mStream(&mData, QIODevice::WriteOnly);
    QVariantMap mProperties;

    mStream << qint32(d->filterModel->rowCount());
    for (int iRow = 0; iRow < d->filterModel->rowCount(); iRow++) {
        mStream << d->filterModel->headerData(iRow, Qt::Vertical).toString();
    }
    for (int iRow = 0; iRow < d->filterModel->rowCount(); iRow++) {
        for (int iCol = 0; iCol < d->filterModel->columnCount(); iCol++) {
            mProperties = d->filterModel->index(iRow, iCol).data(Qt::EditRole).toMap();
            if (!mProperties.isEmpty()) {
                mStream << qint32(iRow) << qint32(iCol) << mProperties;
            }
        }
    }
    return mData;
}

QByteArray QAdvancedTableView::saveState()
{
    QByteArray stateArr;
    QDataStream stream(&stateArr, QIODevice::WriteOnly);

    stream << ui->headerTableView->horizontalHeader()->saveState();
    stream << viewSplitted();
    stream << showFixedRows();
    stream << showFilter();
    stream << showGrid();
    return stateArr;
}

void QAdvancedTableView::scrollToBottom()
{
    ui->dataTableView->scrollToBottom();
}

void QAdvancedTableView::scrollToTop()
{
    ui->dataTableView->scrollToTop();
}

void QAdvancedTableView::selectAll()
{
    ui->dataTableView->selectAll();
}

void QAdvancedTableView::selectColumn(int colum)
{
    ui->dataTableView->selectColumn(colum);
}

void QAdvancedTableView::selectRow(int row)
{
    ui->dataTableView->selectRow(row);
}

QAbstractItemView::SelectionBehavior QAdvancedTableView::selectionBehavior() const
{
    return ui->dataTableView->selectionBehavior();
}

QAbstractItemView::SelectionMode QAdvancedTableView::selectionMode() const
{
    return ui->dataTableView->selectionMode();
}

QItemSelectionModel *QAdvancedTableView::selectionModel() const
{
    return ui->dataTableView->selectionModel();
}

void QAdvancedTableView::setAlternatingRowColors(bool enable)
{
    V_CALL(setAlternatingRowColors(enable))
}

void QAdvancedTableView::setAutoScroll(bool enable)
{
    V_CALL(setAutoScroll(enable))
}

void QAdvancedTableView::setColumnWidth(int column, int width)
{
    ui->headerTableView->setColumnWidth(column, width);
}

void QAdvancedTableView::setContextMenuPolicy(Qt::ContextMenuPolicy policy)
{
    V_CALL(setContextMenuPolicy(policy))
}

void QAdvancedTableView::setCurrentIndex(const QModelIndex &index)
{
    ui->dataTableView->setCurrentIndex(index);
}

void QAdvancedTableView::setDefaultFilterType(int column, int type)
{
    d->filterModel->setData(d->filterModel->index(0, column), type, QAbstractFilterModel::DefaultFilterTypeRole);
}

void QAdvancedTableView::setDefaultFilterType(const QString &name, int type)
{
    for (int i = 0; i < ui->headerTableView->model()->columnCount(); i++) {
        if (ui->headerTableView->model()->headerData(i, Qt::Horizontal) == name) {
            setDefaultFilterType(i, type);
        }
    }
}

void QAdvancedTableView::setDragEnabled(bool enable)
{
    V_CALL(setDragEnabled(enable))
}

void QAdvancedTableView::setEditTriggers(QAbstractItemView::EditTriggers triggers)
{
    V_CALL(setEditTriggers(triggers))
}

void QAdvancedTableView::setColumnFilterTypes(int column, const QVariantList &types)
{
    d->filterModel->setData(d->filterModel->index(0, column), types, QAbstractFilterModel::ColumnFilterTypesRole);
}

void QAdvancedTableView::setFilterDisabledIcon(const QIcon &icon)
{
    d->filterModel->setFilterDisabledIcon(icon);
}

void QAdvancedTableView::setFilterEnabledIcon(const QIcon &icon)
{
    d->filterModel->setFilterEnabledIcon(icon);
}

void QAdvancedTableView::setFilterEnabled(int row, int column, bool enable)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(enable);
    // d->filterModel->setFilterEnabled(row, column, enable);
}

void QAdvancedTableView::setItemDelegate(QAbstractItemDelegate *delegate)
{
    V_CALL(setItemDelegate(delegate))
}

void QAdvancedTableView::setItemDelegateForColumn(int column, QAbstractItemDelegate *delegate)
{
    V_CALL(setItemDelegateForColumn(column, delegate))
}

void QAdvancedTableView::setItemDelegateForRow(int row, QAbstractItemDelegate *delegate)
{
    V_CALL(setItemDelegateForRow(row, delegate))
}

void QAdvancedTableView::setGridStyle(Qt::PenStyle style)
{
    ui->headerTableView->setGridStyle(style);
    V_CALL(setGridStyle(style))
}

void QAdvancedTableView::setIconSize(const QSize size) {
    V_CALL(setIconSize(size))
}

QWidget *QAdvancedTableView::indexWidget(const QModelIndex &index) const
{
    if (focusProxy() == ui->splittedDataTableView) {
        return ui->splittedDataTableView->indexWidget(index);
    }
    if (focusProxy() == ui->fixedRowsTableView) {
        return ui->fixedRowsTableView->indexWidget(index);
    }
    return ui->dataTableView->indexWidget(index);
}

void QAdvancedTableView::setIndexWidget(const QModelIndex &index, QWidget *widget)
{
    if (focusProxy() == ui->splittedDataTableView) {
        ui->splittedDataTableView->setIndexWidget(index, widget);
    } else if (focusProxy() == ui->fixedRowsTableView) {
        ui->fixedRowsTableView->setIndexWidget(index, widget);
    } else {
        ui->dataTableView->setIndexWidget(index, widget);
    }
}

void QAdvancedTableView::setModel(QAbstractItemModel *model)
{
    d->model = model;
    connect(d->model, &QAbstractItemModel::layoutChanged, this, &QAdvancedTableView::viewLayoutChanged);
    ui->fixedRowsTableView->setModel(d->model);
    d->filterModel->setSourceModel(d->model);

    d->summaryHeader->setModel(new QStandardItemModel(1, d->model->columnCount()));
    for (int i = 0; i < d->model->columnCount() + 1; i++) {
        d->summaryHeader->model()->setHeaderData(i, Qt::Horizontal, "");
    }

    horizontalHeader()->setModel(d->model);

    for (int iCol = 0; iCol < d->horizontalHeader->count(); iCol++) {
        ui->dataTableView->horizontalHeader()->resizeSection(iCol, d->horizontalHeader->sectionSize(iCol));
        ui->dataTableView->horizontalHeader()->moveSection(ui->dataTableView->horizontalHeader()->visualIndex(iCol), d->horizontalHeader->visualIndex(iCol));

        // d->summaryView->resizeSection(iCol, d->horizontalHeader->sectionSize(iCol));
        // d->summaryHeader->moveSection(ui->dataTableView->horizontalHeader()->visualIndex(iCol), d->horizontalHeader->visualIndex(iCol));
    }
    if (horizontalHeader()->stretchLastSection()) {
        if (horizontalHeader()->count() > 0) {
            horizontalHeader()->setSectionResizeMode(horizontalHeader()->count() - 1, QHeaderView::Stretch);
            d->summaryHeader->setSectionResizeMode(d->summaryHeader->count() - 1, QHeaderView::Stretch);
        }
    }

    updateSummary();
    updateHeaderViewGeometries();
}

void QAdvancedTableView::setRowHeight(int row, int height)
{
    ui->dataTableView->setRowHeight(row, height);
}

void QAdvancedTableView::setRootIndex(const QModelIndex &index)
{
    V_CALL(setRootIndex(index))
}

void QAdvancedTableView::showColumn(int column)
{
    horizontalHeader()->showSection(column);
}

void QAdvancedTableView::showEvent(QShowEvent *event)
{
    ui->fixedRowsTableView->verticalHeader()->setVisible(ui->dataTableView->verticalHeader()->isVisible());
    ui->splittedDataTableView->verticalHeader()->setVisible(ui->dataTableView->verticalHeader()->isVisible());
    d->verticalHeader->setVisible(ui->dataTableView->verticalHeader()->isVisible());
    viewLayoutChanged();
    QWidget::showEvent(event);
}

void QAdvancedTableView::showRow(int row)
{
    V_CALL(showRow(row))
}

void QAdvancedTableView::setColumnsAutoFitParams(const QMap<int, int> &colSpareWidthParts, bool forceFitSize)
{
    d->columnSpareWidthParts.clear();
    d->columnSpareWidthParts = colSpareWidthParts;
    d->autoResizeColumnsToFitView = forceFitSize;
    autoResizeColumnsToContent();
}

void QAdvancedTableView::autoResizeColumnsToContent()
{
    if (!model() || !isVisible()) {
        return;
    }

    const int columnsCnt = model()->columnCount();
    if (columnsCnt == 0) {
        return;
    }

    // Use RAII guard for batch operations
    BatchSyncGuard guard(this);

    // Calculate available width, accounting for scrollbar
    int availableWidth = viewport()->width();

    if (ui->dataTableView->verticalScrollBar()->isVisible() && ui->dataTableView->verticalScrollBarPolicy() != Qt::ScrollBarAlwaysOff) {
        const int scrollBarWidth = style()->pixelMetric(QStyle::PM_ScrollBarExtent);
        availableWidth -= scrollBarWidth;
    }

    resizeColumnsToContents();

    ui->dataTableView->resizeColumnsToContents();

    /*horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    if (d->summaryView) {
        d->summaryView->resizeSections(QHeaderView::ResizeToContents);
    }*/

    struct ColumnInfo {
        int index { 0 };
        int top_header { 0 };
        int content { 0 };
        int extra_header { 0 };
        int max_width { 0 };
        int min_width { 0 };
    };

    std::map<int, ColumnInfo> col_info;
    int resizedWidth = 0;
    for (int i = 0; i < columnsCnt; ++i) {
        if (!isColumnHidden(i)) {
            ColumnInfo item;
            item.index = i;

            item.top_header = getHeaderSectionWidth(horizontalHeader(), i);
            item.extra_header = d->summaryHeader ? getHeaderSectionWidth(d->summaryHeader, i) : 0;
            // item.top_header = horizontalHeader()->sectionSize(i);
            // item.extra_header = d->summaryView ? d->summaryView->sectionSize(i + 1) : 0;
            item.content = ui->dataTableView->columnWidth(i);
            item.max_width = std::max({ item.top_header, item.content, item.extra_header });
            item.min_width = std::max({ item.top_header, item.extra_header });

            resizedWidth += item.max_width;

            col_info[i] = item;
        }
    }

    // Distribute remaining width to spare columns
    int remainingWidth = availableWidth - resizedWidth;
    if (remainingWidth < 0 && d->autoResizeColumnsToFitView) {
        int widthToReduce = -remainingWidth;
        // Handle case where content width exceeds viewport and force-fit is enabled

        std::map<int, int> shrinkableSpareColumns;
        int totalSpareShrinkable = 0;
        for (auto it = d->columnSpareWidthParts.constBegin(); it != d->columnSpareWidthParts.constEnd(); ++it) {
            int col = it.key();
            if (col >= 0 && col < columnsCnt && !isColumnHidden(col)) {
                int minWidth = std::max({ col_info[col].min_width, 20 });

                int shrinkable = std::max(0, col_info[col].max_width - minWidth);

                if (shrinkable > 0) {
                    shrinkableSpareColumns[col] = shrinkable;
                    totalSpareShrinkable += shrinkable;
                }
            }
        }

        // Stage 1: Try to satisfy the reduction from spare columns only
        if (totalSpareShrinkable && (totalSpareShrinkable >= widthToReduce)) {
            for (const auto &pair : shrinkableSpareColumns) {
                int reduction = (widthToReduce * pair.second) / totalSpareShrinkable;
                col_info[pair.first].max_width -= reduction;
            }
        } else {
            // Stage 2: Shrink spare columns to their minimum first
            for (const auto &pair : shrinkableSpareColumns) {
                int minWidth = std::max({ col_info[pair.first].min_width, 20 });

                int reduction = col_info[pair.first].max_width - minWidth;

                col_info[pair.first].max_width = minWidth;
                widthToReduce -= reduction;
                resizedWidth -= reduction;
            }

            // Satge 3: Still need more reduction - shrink all columns proportionally
            if (widthToReduce > 0) {
                double scaleFactor = static_cast<double>(availableWidth) / resizedWidth;

                for (auto &pair : col_info) {
                    int minWidth = std::max({ pair.second.top_header, pair.second.extra_header, 20 });
                    int scaledWidth = static_cast<int>(pair.second.max_width * scaleFactor);

                    pair.second.max_width = std::max(minWidth, scaledWidth);
                }
            }
        }
    } else {
        // Calculate total parts for spare with distribution
        int partCount = 0;
        std::map<int, int> validSpareWidthParts;
        for (auto it = d->columnSpareWidthParts.constBegin(); it != d->columnSpareWidthParts.constEnd(); ++it) {
            if (it.key() >= 0 && it.key() < columnsCnt && it.value() > 0) {
                if (!isColumnHidden(it.key())) {
                    validSpareWidthParts[it.key()] = it.value();
                    partCount += it.value();
                }
            }
        }

        if (partCount == 0 && d->autoResizeColumnsToFitView) {
            for (int i = 0; i < columnsCnt; ++i) {
                if (!isColumnHidden(i)) {
                    validSpareWidthParts[i] = 1;
                }
            }
            partCount = validSpareWidthParts.size();
        }

        // Handle spare width distribution
        if (partCount > 0) {
            for (const auto &pair : validSpareWidthParts) {
                int partWidth = (remainingWidth * pair.second) / partCount;
                col_info[pair.first].max_width += partWidth;
            }
        }
    }

    if (d->summaryHeader->isVisible()) {
        // Synchronize section 0 (vertical header placeholder) with actual vertical header
        const bool vHeaderVisible = verticalHeader()->isVisible() && !verticalHeader()->sectionsHidden();
        const int vHeaderWidth = vHeaderVisible ? verticalHeader()->width() : 0;

        d->summaryButton->setFixedWidth(vHeaderWidth);
        d->summaryButton->setVisible(vHeaderVisible);
    }
    // Resize all visible columns in the header view
    for (const auto &pair : col_info) {
        if (!isColumnHidden(pair.first)) {
            ui->headerTableView->horizontalHeader()->resizeSection(pair.first, pair.second.max_width);
        }
    }

    // Guard destructor will trigger syncAllColumns() automatically
    // when batch operations complete

    // Adjust row heights after column resize
    ui->dataTableView->resizeRowsToContents();

    emit sectionSizeChanged();
}

void QAdvancedTableView::setSelectionBehavior(QAbstractItemView::SelectionBehavior behavior)
{
    V_CALL(setSelectionBehavior(behavior))
}

void QAdvancedTableView::setSelectionMode(QAbstractItemView::SelectionMode mode)
{
    V_CALL(setSelectionMode(mode))
}

void QAdvancedTableView::setFilterModel(QAbstractFilterModel *model)
{
    if (d->filterModel) {
        disconnect(d->filterModel);
    }
    d->filterModel = model;
    d->dataViewProxy->setFilterModel(d->filterModel);
    ui->headerTableView->setModel(d->filterModel);
    // filter model
    connect(d->filterModel, &QAbstractFilterModel::modelReset, this, &QAdvancedTableView::updateHeaderViewGeometries);
    connect(d->filterModel, &QAbstractFilterModel::rowsRemoved, this, &QAdvancedTableView::updateHeaderViewGeometries);
}

void QAdvancedTableView::setFilterProxyModel(QAbstractFilterProxyModel *proxy)
{
    if (d->dataViewProxy) {
        disconnect(d->dataViewProxy);
    }
    d->dataViewProxy = proxy;
    d->dataViewProxy->setFilterModel(d->filterModel);
    d->dataViewProxy->setSourceModel(ui->fixedRowsTableView->decorationProxy());
    ui->dataTableView->setModel(d->dataViewProxy);
    ui->splittedDataTableView->setModel(d->dataViewProxy);

    connect(d->dataViewProxy, &QAbstractFilterProxyModel::modelReset, this, &QAdvancedTableView::modelReset);
    connect(d->dataViewProxy, &QAbstractFilterProxyModel::layoutChanged, this, &QAdvancedTableView::dataModelLayoutChanged);
    dataModelLayoutChanged();
}

bool QAdvancedTableView::setFilterType(int type, int column, int row)
{
    QVariantMap mProperties;
    mProperties["type"] = type;
    if (d->filterModel->createFilter(d->filterModel->index(row, column), mProperties) != nullptr) {
        return true;
    }
    return false;
}

void QAdvancedTableView::setShowFixedRows(bool show)
{
    ui->fixedRowsTableView->decorationProxy()->setEnabled(show);
}

void QAdvancedTableView::setShowGrid(bool show)
{
    ui->headerTableView->setShowGrid(show);
    V_CALL(setShowGrid(show))
}

void QAdvancedTableView::setSortIndicatorShown(bool show)
{
    ui->headerTableView->horizontalHeader()->setSortIndicatorShown(show);
}

void QAdvancedTableView::setSortingEnabled(bool enable)
{
    V_CALL(setSortingEnabled(enable))
}

void QAdvancedTableView::setSpan(int row, int column, int rowSpanCount, int columnSpanCount)
{
    if (focusProxy() == ui->splittedDataTableView) {
        ui->splittedDataTableView->setSpan(row, column, rowSpanCount, columnSpanCount);
    } else if (focusProxy() == ui->fixedRowsTableView) {
        ui->fixedRowsTableView->setSpan(row, column, rowSpanCount, columnSpanCount);
    } else {
        ui->dataTableView->setSpan(row, column, rowSpanCount, columnSpanCount);
    }
}

void QAdvancedTableView::setTextElideMode(Qt::TextElideMode mode)
{
    V_CALL(setTextElideMode(mode))
}

void QAdvancedTableView::setShowFilter(bool show)
{
    ui->headerTableView->setFilterVisible(show);
}

void QAdvancedTableView::setWordWrap(bool wrap)
{
    ui->dataTableView->setWordWrap(wrap);
    ui->fixedRowsTableView->setWordWrap(wrap);
}

bool QAdvancedTableView::showGrid() const
{
    return ui->dataTableView->showGrid();
}

void QAdvancedTableView::subviewReceivedFocus()
{
    QWidget *w = qobject_cast<QWidget *>(sender());
    if (w) {
        setFocusProxy(w);
    }
}

QSize QAdvancedTableView::sizeHint() const
{
    return QSize(QWidget::sizeHint().width(), ui->headerTableView->height() * 2);
}

void QAdvancedTableView::splitView(bool split)
{
    if (split == ui->splittedDataTableView->isVisible()) {
        return;
    }
    if (split) {
        ui->splittedDataTableView->show();
        if (d->autoResizeRowsToContents) {
            ui->splittedDataTableView->resizeRowsToContents();
        }
    } else {
        ui->splittedDataTableView->hide();
    }
    ui->splittedDataTableView->setVerticalScrollBarPolicy(ui->dataTableView->verticalScrollBarPolicy());
}

void QAdvancedTableView::sortByColumn(int column, Qt::SortOrder order)
{
    ui->headerTableView->sortByColumn(column, order);
}

Qt::TextElideMode QAdvancedTableView::textElideMode() const
{
    return ui->dataTableView->textElideMode();
}

void QAdvancedTableView::update(const QModelIndex &index)
{
    ui->dataTableView->update(index);
}

void QAdvancedTableView::dataModelLayoutChanged()
{
    if (d->autoResizeRowsToContents) {
        ui->dataTableView->resizeRowsToContents();
        ui->fixedRowsTableView->resizeRowsToContents();
        ui->splittedDataTableView->resizeRowsToContents();
    }
    for (int i = 0; i < ui->headerTableView->horizontalHeader()->count(); i++) {
        bool isHidden = ui->headerTableView->horizontalHeader()->isSectionHidden(i);
        ui->dataTableView->horizontalHeader()->setSectionHidden(i, isHidden);
        ui->fixedRowsTableView->horizontalHeader()->setSectionHidden(i, isHidden);
        ui->splittedDataTableView->horizontalHeader()->setSectionHidden(i, isHidden);
        d->summaryHeader->setSectionHidden(i, isHidden);
    }
    updateSummary();
}

void QAdvancedTableView::updateHeaderViewGeometries()
{
    int rows = ui->headerTableView->model()->rowCount();
    if (rows > ui->headerTableView->maxVisibileFilterSets()) {
        rows = ui->headerTableView->maxVisibileFilterSets();
    }

    for (int iRow = 0; iRow < ui->headerTableView->model()->rowCount(); iRow++) {
        ui->headerTableView->verticalHeader()->resizeSection(iRow, ui->headerTableView->verticalHeader()->sizeHint().height());
    }

    int mRowHeight = ui->headerTableView->verticalHeader()->sizeHint().height();
    if (mRowHeight == 0) {
        mRowHeight = ui->headerTableView->horizontalHeader()->sizeHint().height();
    }
    int mHeaderHeight = ui->headerTableView->horizontalHeader()->sizeHint().height();
    if (mHeaderHeight == 0) {
        mHeaderHeight = mRowHeight;
    }

    if (ui->headerTableView->filterVisible()) {
        ui->headerTableView->setFixedHeight(mHeaderHeight + mRowHeight * rows + (1 * rows) + 1);
    } else {
        ui->headerTableView->setFixedHeight(ui->headerTableView->horizontalHeader()->sizeHint().height());
    }

    if (ui->headerTableView->model()->rowCount() > ui->headerTableView->maxVisibileFilterSets()) {
        ui->headerTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->dataTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->splittedDataTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }

    // Qt6 Fix: Use sizeHint().width() instead of width() when header has no sections.
    // In Qt6, width() returns the geometric width from the layout system even when
    // the header has no sections (count() == 0). The sizeHint() correctly returns 0
    // for headers without content, which is what we want for comparison.
    auto getContentWidth = [](QHeaderView *header) -> int {
        // If header has no sections, use sizeHint which returns content-based width
        if (header->count() == 0) {
            return header->sizeHint().width();
        }
        // Otherwise use actual width
        return header->width();
    };

    int vertHWidth = getContentWidth(ui->headerTableView->verticalHeader());
    int dataVHWidth = getContentWidth(ui->dataTableView->verticalHeader());

    if (vertHWidth > dataVHWidth) {
        ui->dataTableView->verticalHeader()->setFixedWidth(vertHWidth);
        ui->fixedRowsTableView->verticalHeader()->setFixedWidth(vertHWidth);
        ui->splittedDataTableView->verticalHeader()->setFixedWidth(vertHWidth);
    } else {
        ui->headerTableView->verticalHeader()->setFixedWidth(dataVHWidth);
    }

    if (d->summaryHeader->isVisible()) {
        // Synchronize section 0 (vertical header placeholder) with actual vertical header
        const bool vHeaderVisible = verticalHeader()->isVisible() && !verticalHeader()->sectionsHidden();
        const int vHeaderWidth = vHeaderVisible ? verticalHeader()->width() : 0;

        d->summaryButton->setFixedWidth(vHeaderWidth);
        d->summaryButton->setVisible(vHeaderVisible);
    }
}

void QAdvancedTableView::updateHeaderViewVerticalScrollBar(int min, int max)
{
    Q_UNUSED(min);
    Q_UNUSED(max);
    if (ui->dataTableView->verticalScrollBar()->maximum() == 0) {
        ui->headerTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->fixedRowsTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    } else {
        ui->headerTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->fixedRowsTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
}

void QAdvancedTableView::verticalHeaderSectionClicked(int section)
{
    QPoint p = mapFromGlobal(QCursor::pos());
    if (ui->fixedRowsTableView->decorationProxy()->isEnabled()) {
        if (p.x() < ui->fixedRowsTableView->decorationProxy()->iconSize().width()) {
            ui->fixedRowsTableView->decorationProxy()->toggleRow(ui->dataTableView->model()->index(section, 0));
        }
    }
}

void QAdvancedTableView::verticalHeaderWidthChangeRequested(int width)
{
    if (sender() == ui->headerTableView) {
        if (ui->dataTableView->verticalHeader()->sectionSize(0) < width) {
            int w = width + 12;
            if (showFixedRows()) {
                w += 22;
            }
            ui->dataTableView->verticalHeader()->setFixedWidth(w);
            ui->headerTableView->verticalHeader()->setFixedWidth(w);
            ui->fixedRowsTableView->verticalHeader()->setFixedWidth(w);
            ui->splittedDataTableView->verticalHeader()->setFixedWidth(w);
            updateHeaderViewGeometries();
        }
    }
}

void QAdvancedTableView::verticalHeaderSectionResized(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(oldSize);
    if (sender() == ui->splittedDataTableView->verticalHeader()) {
        ui->dataTableView->verticalHeader()->resizeSection(logicalIndex, newSize);
    } else if (sender() == ui->dataTableView->verticalHeader()) {
        ui->splittedDataTableView->verticalHeader()->resizeSection(logicalIndex, newSize);
    }
    adjustSummaryOffset();
}

QHeaderView *QAdvancedTableView::verticalHeader() const
{
    return ui->dataTableView->verticalHeader();
}

void QAdvancedTableView::viewActivated(const QModelIndex &index)
{
    emit activated(mapToSource(index));
}

void QAdvancedTableView::viewClicked(const QModelIndex &index)
{
    emit clicked(mapToSource(index));
}

void QAdvancedTableView::viewDoubleClicked(const QModelIndex &index)
{
    emit doubleClicked(mapToSource(index));
}

void QAdvancedTableView::viewEntered(const QModelIndex &index)
{
    emit entered(mapToSource(index));
}

void QAdvancedTableView::viewLayoutChanged()
{
    QTimer::singleShot(0, this, &QAdvancedTableView::viewLayoutChangedImpl);
}

void QAdvancedTableView::viewLayoutChangedImpl()
{
    autoResizeColumnsToContent();
}

void QAdvancedTableView::viewPressed(const QModelIndex &index)
{
    emit pressed(mapToSource(index));
}

QWidget *QAdvancedTableView::viewport() const
{
    if (focusProxy() == ui->splittedDataTableView) {
        return ui->splittedDataTableView->viewport();
    }
    if (focusProxy() == ui->fixedRowsTableView) {
        return ui->fixedRowsTableView->viewport();
    }
    return ui->dataTableView->viewport();
}

bool QAdvancedTableView::wordWrap() const
{
    return ui->dataTableView->wordWrap();
}

void QAdvancedTableView::setSummaryType(int column, advSummaryFunc type)
{
    d->columnsSummaryTypes[column] = type;
    updateSummary();
}

void QAdvancedTableView::setSummaryTypes(const QMap<int, advSummaryFunc> &columnMap)
{
    d->columnsSummaryTypes.clear();
    d->columnsSummaryTypes = columnMap;
    updateSummary();
}

void QAdvancedTableView::updateSummary()
{
    if (d->model && d->columnsSummaryTypes.count() > 0) {
        d->summaryWidget->setVisible(true);
        QMap<int, advSummaryFunc>::iterator iter;
        for (iter = d->columnsSummaryTypes.begin(); iter != d->columnsSummaryTypes.end(); ++iter) {
            advSummaryFunc fn = iter.value();
            if (fn) {
                QVariant result = fn(ui->dataTableView->model(), iter.key());
                d->summaryHeader->model()->setHeaderData(iter.key(), Qt::Horizontal, result);
            }
        }
    } else {
        d->summaryWidget->setVisible(false);
    }
}

void QAdvancedTableView::adjustSummaryOffset()
{
    updateSummary();
    // int offset = ui->headerTableView->horizontalHeader()->offset() + (ui->dataTableView->verticalHeader()->isVisible() ? ui->dataTableView->verticalHeader()->width() : 0);
    // d->summaryView->setStyleSheet(QString("margin-left: %1px;").arg(offset));
}

void QAdvancedTableView::sectionsResize()
{
    adjustSummaryOffset();
    if (ui->dataTableView->model()) {
        for (int i = 0; i < ui->dataTableView->model()->columnCount(); i++) {
            d->summaryHeader->resizeSection(i, ui->dataTableView->columnWidth(i));
        }
    }
}

void QAdvancedTableView::summaryButtonClicked()
{
}
