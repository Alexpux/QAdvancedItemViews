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
#include "qadvancedtableview.h"
#include "ui_qadvancedtableview.h"

#include "qadvancedheaderview.h"
#include "qadvancedtableview_p.h"

#include <qfiltermodelproxy.h>
#include <qfiltermodel.h>
#include <qfilterviewitemdelegate.h>
#include <qfixedrowstableview.h>
#include <qshareditemselectionmodel.h>

#define V_CALL(_m_) ui->dataTableView->_m_; \
    ui->fixedRowsTableView->_m_; \
    ui->splittedDataTableView->_m_;


class QAdvancedTableViewPrivate
{
public:
    QAdvancedTableViewPrivate(QAdvancedTableView* tv);
    ~QAdvancedTableViewPrivate();

    bool autoResizeRowsToContents;
    int defaultFilterType;
    QFilterModelProxy* dataViewProxy;
    QAbstractFilterModel* filterModel;
    QAbstractItemModel* model;
    QFilterTableViewSettingsDialog* settingsDialog;

    QAdvancedHeaderView* horizontalHeader;
    Qt::ScrollBarPolicy horizontalScrollBarPolicy;
    QAdvancedHeaderView* verticalHeader;
    QItemSelectionModel* selectionModel;
    QSharedItemSelectionModel* splittedViewSelectionModel;

    QAdvancedTableView* v;
};

QAdvancedTableViewPrivate::QAdvancedTableViewPrivate(QAdvancedTableView *tv)
{
    dataViewProxy = new QFilterModelProxy(tv);
    horizontalScrollBarPolicy = Qt::ScrollBarAsNeeded;
    selectionModel = 0;
    v = tv;
}

QAdvancedTableViewPrivate::~QAdvancedTableViewPrivate()
{

}

//-----------------------------------------------
// class QFilterTableViewSettingsDialog
//-----------------------------------------------

QFilterTableViewSettingsDialog::QFilterTableViewSettingsDialog(QAbstractFilterModel *model, QWidget* parent) :
QDialog(parent)
{
    setup();
    cModel = model;
}

QFilterTableViewSettingsDialog::~QFilterTableViewSettingsDialog()
{
}

void QFilterTableViewSettingsDialog::setup()
{
    if (objectName().isEmpty()){
        setObjectName(QString::fromUtf8("Dialog"));
    }
    resize(400, 300);
    verticalLayout_2 = new QVBoxLayout(this);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));

    verticalLayout_2->addLayout(horizontalLayout_2);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    label_2 = new QLabel(this);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    horizontalLayout->addWidget(label_2);

    nameLineEdit = new QLineEdit(this);
    nameLineEdit->setObjectName(QString::fromUtf8("nameLineEdit"));

    horizontalLayout->addWidget(nameLineEdit);

    renamePushButton = new QPushButton(this);
    renamePushButton->setObjectName(QString::fromUtf8("renamePushButton"));

    horizontalLayout->addWidget(renamePushButton);


    verticalLayout_2->addLayout(horizontalLayout);

    groupBox_2 = new QGroupBox(this);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setFlat(true);

    verticalLayout_2->addWidget(groupBox_2);

    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    tableWidget = new QTableWidget(this);
    tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

    horizontalLayout_3->addWidget(tableWidget);

    verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    addPushButton = new QPushButton(this);
    addPushButton->setObjectName(QString::fromUtf8("addPushButton"));
    verticalLayout->addWidget(addPushButton);

    selectPushButton = new QPushButton(this);
    selectPushButton->setObjectName(QString::fromUtf8("selectPushButton"));
    verticalLayout->addWidget(selectPushButton);

    removePushButton = new QPushButton(this);
    removePushButton->setObjectName(QString::fromUtf8("removePushButton"));
    verticalLayout->addWidget(removePushButton);

    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout->addItem(verticalSpacer);

    closePushButton = new QPushButton(this);
    closePushButton->setObjectName(QString::fromUtf8("closePushButton"));

    verticalLayout->addWidget(closePushButton);
    horizontalLayout_3->addLayout(verticalLayout);
    verticalLayout_2->addLayout(horizontalLayout_3);

    retranslate();
    QObject::connect(closePushButton, SIGNAL(clicked()), this, SLOT(reject()));

    QMetaObject::connectSlotsByName(this);
}

void QFilterTableViewSettingsDialog::retranslate()
{
    setWindowTitle(QApplication::translate("QFilterTableViewSettingsDialog", "Filter Sets", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("QFilterTableViewSettingsDialog", "Name", 0, QApplication::UnicodeUTF8));
    renamePushButton->setText(QApplication::translate("QFilterTableViewSettingsDialog", "Rename", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("QFilterTableViewSettingsDialog", "Available Filter Sets", 0, QApplication::UnicodeUTF8));
    addPushButton->setText(QApplication::translate("QFilterTableViewSettingsDialog", "Add", 0, QApplication::UnicodeUTF8));
    removePushButton->setText(QApplication::translate("QFilterTableViewSettingsDialog", "Remove", 0, QApplication::UnicodeUTF8));
    selectPushButton->setText(QApplication::translate("QFilterTableViewSettingsDialog", "Select", 0, QApplication::UnicodeUTF8));
    closePushButton->setText(QApplication::translate("QFilterTableViewSettingsDialog", "Close", 0, QApplication::UnicodeUTF8));
} // retranslateUi

//-----------------------------------------------
// class QAdvancedTableView
//-----------------------------------------------

QAdvancedTableView::QAdvancedTableView(QWidget *parent) :
    QWidget(parent), ui(new Ui::QAdvancedTableView)
{
    d =  new QAdvancedTableViewPrivate(this);
    ui->setupUi(this);
    //
    ui->splittedDataTableView->hide();
    //
    d->selectionModel = 0;

    d->autoResizeRowsToContents = true;
    d->defaultFilterType = QAbstractFilter::Type;
    d->settingsDialog = 0;
    // Create header view (model) proxy
    d->filterModel = new QFilterModel(this);
    // Create horizontal header view
    d->horizontalHeader = new QAdvancedHeaderView(Qt::Horizontal, this);
    ui->headerTableView->setHorizontalHeader(d->horizontalHeader);
    ui->headerTableView->setItemDelegate(new QFilterViewItemDelegate(this));
    ui->headerTableView->horizontalHeader()->setMovable(true);

    // Connect data views
    connect(ui->dataTableView->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(verticalHeaderSectionClicked(int)));
    connect(ui->splittedDataTableView->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(verticalHeaderSectionClicked(int)));
    // Create vertical header view
    d->verticalHeader = new QAdvancedHeaderView(Qt::Vertical, this);

    ui->headerTableView->setVerticalHeader(d->verticalHeader);
    ui->headerTableView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    // set models
    d->dataViewProxy->setSourceModel(ui->fixedRowsTableView->decorationProxy());
    ui->dataTableView->setModel(d->dataViewProxy);
    ui->splittedDataTableView->setModel(d->dataViewProxy);
    ui->headerTableView->setModel(d->filterModel);

    d->dataViewProxy->setFilterModel(d->filterModel);
    // set selection models
    ui->splittedDataTableView->setSelectionModel(new QSharedItemSelectionModel(ui->splittedDataTableView->model(), ui->dataTableView->selectionModel(), this));
    ui->fixedRowsTableView->setSelectionModel(new QSharedItemSelectionModel(ui->fixedRowsTableView->model(), ui->dataTableView->selectionModel(), this));
    //
    connect(ui->dataTableView->horizontalScrollBar(), SIGNAL(sliderMoved(int)), this, SLOT(dataViewHorizontalScrollBarSilderMoved(int)));
    connect(ui->dataTableView->horizontalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(updateHeaderViewHorizontalScrollBar(int,int)));
    connect(ui->dataTableView->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(dataViewHorizontalScrollBarValueChanged(int)));
    connect(ui->dataTableView->verticalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(updateHeaderViewVerticalScrollBar(int,int)));

    connect(d->filterModel, SIGNAL(modelReset()), this, SLOT(updateHeaderViewGeometries()));
    connect(d->filterModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(updateHeaderViewGeometries()));

    connect(d->dataViewProxy, SIGNAL(modelReset()), this, SLOT(modelReset()));
    connect(d->dataViewProxy, SIGNAL(layoutChanged()), this, SLOT(dataModelLayoutChanged()));

    connect(ui->headerTableView, SIGNAL(cornerButtonClicked()), this, SLOT(selectAll()));
    connect(ui->headerTableView, SIGNAL(calcGeometryRequested()), this, SLOT(updateHeaderViewGeometries()));
    connect(ui->headerTableView, SIGNAL(visibilityChanged(bool)), this, SLOT(updateHeaderViewGeometries()));
    connect(ui->headerTableView->model(), SIGNAL(rowsInserted(QModelIndex, int, int)), this, SLOT(filterAdded(QModelIndex, int, int)));
    connect(ui->headerTableView->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(headerViewSectionResized(int,int,int)));
    connect(ui->headerTableView->horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(horizontalHeaderViewSectionMoved(int,int,int)));
    connect(ui->headerTableView->horizontalHeader(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(horizontalHeaderSortIndicatorChanged(int,Qt::SortOrder)));
    connect(ui->headerTableView->horizontalScrollBar(), SIGNAL(sliderMoved(int)), this, SLOT(headerViewHorizontalScrollBarSilderMoved(int)));
    connect(ui->headerTableView->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(headerViewHorizontalScrollBarValueChanged(int)));

    //
    connect(ui->dataTableView->verticalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(verticalHeaderSectionResized(int,int,int)));
    connect(ui->splittedDataTableView->verticalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(verticalHeaderSectionResized(int,int,int)));

//    showFilterView();
    updateHeaderViewGeometries();
    // Forward data view signals
    connect(ui->dataTableView, SIGNAL(activated(QModelIndex)), this, SIGNAL(activated(QModelIndex)));
    connect(ui->dataTableView, SIGNAL(clicked(QModelIndex)), this, SIGNAL(clicked(QModelIndex)));
    connect(ui->dataTableView, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));
    connect(ui->dataTableView, SIGNAL(doubleClicked(QModelIndex)), this, SIGNAL(doubleClicked(QModelIndex)));
    connect(ui->dataTableView, SIGNAL(entered(QModelIndex)), this, SIGNAL(entered(QModelIndex)));
    connect(ui->dataTableView, SIGNAL(pressed(QModelIndex)), this, SIGNAL(pressed(QModelIndex)));
    connect(ui->dataTableView, SIGNAL(viewportEntered()), this, SIGNAL(viewportEntered()));

    connect(ui->fixedRowsTableView, SIGNAL(activated(QModelIndex)), this, SIGNAL(activated(QModelIndex)));
    connect(ui->fixedRowsTableView, SIGNAL(clicked(QModelIndex)), this, SIGNAL(clicked(QModelIndex)));
    connect(ui->fixedRowsTableView, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));
    connect(ui->fixedRowsTableView, SIGNAL(doubleClicked(QModelIndex)), this, SIGNAL(doubleClicked(QModelIndex)));
    connect(ui->fixedRowsTableView, SIGNAL(entered(QModelIndex)), this, SIGNAL(entered(QModelIndex)));
    connect(ui->fixedRowsTableView, SIGNAL(pressed(QModelIndex)), this, SIGNAL(pressed(QModelIndex)));
    connect(ui->fixedRowsTableView, SIGNAL(viewportEntered()), this, SIGNAL(viewportEntered()));

    connect(ui->splittedDataTableView, SIGNAL(activated(QModelIndex)), this, SIGNAL(activated(QModelIndex)));
    connect(ui->splittedDataTableView, SIGNAL(clicked(QModelIndex)), this, SIGNAL(clicked(QModelIndex)));
    connect(ui->splittedDataTableView, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));
    connect(ui->splittedDataTableView, SIGNAL(doubleClicked(QModelIndex)), this, SIGNAL(doubleClicked(QModelIndex)));
    connect(ui->splittedDataTableView, SIGNAL(entered(QModelIndex)), this, SIGNAL(entered(QModelIndex)));
    connect(ui->splittedDataTableView, SIGNAL(pressed(QModelIndex)), this, SIGNAL(pressed(QModelIndex)));
    connect(ui->splittedDataTableView, SIGNAL(viewportEntered()), this, SIGNAL(viewportEntered()));

    // setup focus handling
    setFocusProxy(ui->dataTableView);
    ui->fixedRowsTableView->setFocusProxy(ui->dataTableView);
    ui->splittedDataTableView->setFocusProxy(ui->dataTableView);
    ui->headerTableView->setFocusProxy(ui->dataTableView);
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

QMenu *QAdvancedTableView::createStandardContextMenu()
{
    QAction* a;
    QMenu* m = new QMenu();
    m = m->addMenu(tr("View"));
    a = m->addAction(tr("Splitted View"));
    a->setCheckable(true);
    a->setChecked(isViewSplitted());
    connect(a, SIGNAL(toggled(bool)), this, SLOT(splitView(bool)));

    a = m->addAction(tr("Fixed Rows"));
    a->setCheckable(true);
    a->setChecked(isFixedRowsModeEnabled());
    connect(a, SIGNAL(toggled(bool)), this, SLOT(setFixedRowsModeEnabled(bool)));

    m->addSeparator();

    a = m->addAction(tr("Show Filter"));
    a->setCheckable(true);
    a->setChecked(filterVisible());
    connect(a, SIGNAL(toggled(bool)), this, SLOT(setFilterVisible(bool)));

    m->addSeparator();

    a = m->addAction(tr("Show Grid"));
    a->setCheckable(true);
    a->setChecked(showGrid());

    connect(a, SIGNAL(toggled(bool)), this, SLOT(setShowGrid(bool)));

    return m;
}

void QAdvancedTableView::clearSelection()
{
    ui->dataTableView->clearSelection();
}

QModelIndex QAdvancedTableView::currentIndex() const
{
    return ui->dataTableView->currentIndex();
}

int QAdvancedTableView::defaultFilterType(int column) const
{
    return d->filterModel->index(0, column).data(QAbstractFilterModel::DefaultFilterTypeRole).toInt();
}

void QAdvancedTableView::dataViewHorizontalScrollBarSilderMoved( int value )
{
    ui->headerTableView->horizontalScrollBar()->setValue(value);
}

void QAdvancedTableView::dataViewHorizontalScrollBarValueChanged( int value )
{
    ui->headerTableView->horizontalScrollBar()->setValue(value);
}

bool QAdvancedTableView::dragEnabled() const
{
	return ui->dataTableView->dragEnabled();
}

void QAdvancedTableView::edit(const QModelIndex & index)
{
    if (ui->fixedRowsTableView->hasFocus()){
        ui->fixedRowsTableView->edit(index);
    } else if (ui->splittedDataTableView->hasFocus()){
        ui->splittedDataTableView->edit(index);
    } else {
        ui->dataTableView->edit(index);
    }
}

QAbstractItemView::EditTriggers QAdvancedTableView::editTriggers () const
{
    return ui->dataTableView->editTriggers();
}

QAbstractFilter *QAdvancedTableView::filterAt(int row, int col) const
{
    return d->filterModel->filter(d->filterModel->index(row, col));
}

void QAdvancedTableView::filterAdded(const QModelIndex & parent, int start, int end)
{
	Q_UNUSED(parent);
	Q_UNUSED(start);
	Q_UNUSED(end);
	updateHeaderViewGeometries();
}

QAbstractFilterModel* QAdvancedTableView::filterModel() const
{
    return d->filterModel;
}

bool QAdvancedTableView::filterVisible() const
{
    return ui->headerTableView->filterVisible();
}

QModelIndexList QAdvancedTableView::fixedRows(int column) const
{
    QModelIndexList l;
    for (int i = 0; i < ui->fixedRowsTableView->model()->rowCount(); i++){
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

void QAdvancedTableView::headerViewHorizontalScrollBarSilderMoved( int value )
{
    ui->dataTableView->horizontalScrollBar()->setValue(value);
    ui->fixedRowsTableView->horizontalScrollBar()->setValue(value);
    ui->splittedDataTableView->horizontalScrollBar()->setValue(value);
}

void QAdvancedTableView::headerViewHorizontalScrollBarValueChanged( int value )
{
    ui->dataTableView->horizontalScrollBar()->setValue(value);
    ui->fixedRowsTableView->horizontalScrollBar()->setValue(value);
    ui->splittedDataTableView->horizontalScrollBar()->setValue(value);
}

void QAdvancedTableView::headerViewSectionResized( int logicalIndex, int oldSize, int newSize )
{
    Q_UNUSED(oldSize);
    ui->dataTableView->horizontalHeader()->resizeSection(logicalIndex, newSize);
    ui->fixedRowsTableView->horizontalHeader()->resizeSection(logicalIndex, newSize);
    ui->splittedDataTableView->horizontalHeader()->resizeSection(logicalIndex, newSize);
}

void QAdvancedTableView::hideColumn(int column)
{
	ui->headerTableView->horizontalHeader()->hideSection(ui->headerTableView->horizontalHeader()->logicalIndex(column));
}

void QAdvancedTableView::hideFilterView()
{
    setFilterVisible(false);
}

void QAdvancedTableView::hideRow(int row)
{
    V_CALL(hideRow(row))
}

QHeaderView* QAdvancedTableView::horizontalHeader() const
{
	return ui->headerTableView->horizontalHeader();
}

void QAdvancedTableView::horizontalHeaderViewSectionMoved( int logicalIndex, int oldVisualIndex, int newVisualIndex )
{
    ui->dataTableView->horizontalHeader()->moveSection(oldVisualIndex, newVisualIndex);
    ui->fixedRowsTableView->horizontalHeader()->moveSection(oldVisualIndex, newVisualIndex);
    ui->splittedDataTableView->horizontalHeader()->moveSection(oldVisualIndex, newVisualIndex);
}

void QAdvancedTableView::horizontalHeaderSortIndicatorChanged( int logicalIndex, Qt::SortOrder order )
{
    ui->dataTableView->sortByColumn(logicalIndex, order);
    ui->fixedRowsTableView->sortByColumn(logicalIndex, order);
    ui->splittedDataTableView->sortByColumn(logicalIndex, order);
}

QModelIndex QAdvancedTableView::indexAt(const QPoint & point) const
{
	return ui->dataTableView->indexAt(point);
}

bool QAdvancedTableView::isColumnHidden(int column) const
{
	return ui->dataTableView->isColumnHidden(column);
}

bool QAdvancedTableView::isRowHidden(int row) const
{
	return ui->dataTableView->isRowHidden(row);
}

bool QAdvancedTableView::isSortIndicatorShown() const
{
    return ui->headerTableView->horizontalHeader()->isSortIndicatorShown();
}

bool QAdvancedTableView::isSortingEnabled() const
{
    return ui->dataTableView->isSortingEnabled();
}

bool QAdvancedTableView::isViewSplitted() const
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

QSize QAdvancedTableView::minimumSizeHint() const
{
    return QSize(QWidget::minimumSizeHint().width(), ui->headerTableView->height() * 2);
}

QAbstractItemModel* QAdvancedTableView::model() const
{
    return d->model;
}

bool QAdvancedTableView::isFixedRowsModeEnabled() const
{
    return ui->fixedRowsTableView->decorationProxy()->isEnabled();
}

void QAdvancedTableView::modelReset()
{
    if (d->autoResizeRowsToContents){
        ui->dataTableView->resizeRowsToContents();
        ui->fixedRowsTableView->resizeRowsToContents();
        ui->splittedDataTableView->resizeRowsToContents();
    }
}

void QAdvancedTableView::removeFilter( int column )
{
    ui->dataTableView->resizeRowsToContents();
}

bool QAdvancedTableView::restoreFilter(const QByteArray & data)
{
    QByteArray mData(data);
    QDataStream mStream(&mData, QIODevice::ReadOnly);
    if (mStream.atEnd()){
        return false;
    }
    qint32 mCol;
    qint32 mRow;
    qint32 mRows;
    QString mName;
    QVariantMap mProperties;
    // Clear current filter model
    d->filterModel->removeRows(0, d->filterModel->rowCount());
    mStream >> mRows;
    d->filterModel->insertRows(0, mRows);
    for (int iRow = 0; iRow < mRows; iRow++){
        mStream >> mName;
        d->filterModel->setHeaderData(iRow, Qt::Vertical, mName);
    }
    while(!mStream.atEnd()){
        mStream >> mRow >> mCol >> mProperties;
        if (d->filterModel->rowCount() < mRow){
            d->filterModel->insertRows(d->filterModel->rowCount(), 1);
        }
        d->filterModel->setData(d->filterModel->index(mRow, mCol), mProperties);
    }
    return true;
}

QModelIndex QAdvancedTableView::rootIndex() const
{
    return ui->dataTableView->rootIndex();
}

int QAdvancedTableView::rowAt(int y) const
{
    if (ui->splittedDataTableView->isVisible()){
        int r = ui->splittedDataTableView->rowAt(y);
        if (r > -1){
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
    ui->dataTableView->resizeColumnToContents(column);
}

void QAdvancedTableView::resizeColumnsToContents()
{
    ui->dataTableView->resizeColumnsToContents();
}

void QAdvancedTableView::resizeRowToContents(int row)
{
    ui->dataTableView->resizeRowToContents(row);
}

void QAdvancedTableView::resizeRowsToContents()
{
	ui->dataTableView->resizeRowsToContents();
}

QByteArray QAdvancedTableView::saveFilter() const
{
    QByteArray mData;
    QDataStream mStream(&mData, QIODevice::WriteOnly);
    QVariantMap mProperties;

    mStream << qint32(d->filterModel->rowCount());
    for (int iRow = 0; iRow < d->filterModel->rowCount(); iRow++){
        mStream << d->filterModel->headerData(iRow, Qt::Vertical).toString();
    }
    for (int iRow = 0; iRow < d->filterModel->rowCount(); iRow++){
        for (int iCol = 0; iCol < d->filterModel->columnCount(); iCol++){
            mProperties = d->filterModel->index(iRow, iCol).data(Qt::EditRole).toMap();
            if (!mProperties.isEmpty()){
                mStream << qint32(iRow) << qint32(iCol) << mProperties;
            }

        }
    }
    return mData;
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

QAbstractItemView::SelectionMode QAdvancedTableView::selectionMode() const
{
	return ui->dataTableView->selectionMode();
}

QItemSelectionModel* QAdvancedTableView::selectionModel() const
{
    return d->selectionModel;
}

void QAdvancedTableView::setAlternatingRowColors( bool enable )
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

void QAdvancedTableView::setCurrentIndex(const QModelIndex & index)
{
	ui->dataTableView->setCurrentIndex(index);
}

void QAdvancedTableView::setDefaultFilterType(int column, int type)
{
    d->filterModel->setData(d->filterModel->index(0, column), type, QAbstractFilterModel::DefaultFilterTypeRole);
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

void QAdvancedTableView::setFilterEnabled(int row, int column, bool enable )
{
//    d->filterModel->setFilterEnabled(row, column, enable);
}

void QAdvancedTableView::setItemDelegate(QAbstractItemDelegate* delegate)
{
    V_CALL(setItemDelegate(delegate))
}

void QAdvancedTableView::setItemDelegateForColumn(int column, QAbstractItemDelegate *delegate)
{
    V_CALL(setItemDelegateForColumn(column, delegate))
}

void QAdvancedTableView::setItemDelegateForRow(int row, QAbstractItemDelegate * delegate)
{
    V_CALL(setItemDelegateForRow(row, delegate))
}

void QAdvancedTableView::setGridStyle(Qt::PenStyle style)
{
    ui->headerTableView->setGridStyle(style);
    V_CALL(setGridStyle(style))
}

void QAdvancedTableView::setIconSize(const QSize &size)
{
    V_CALL(setIconSize(size))
}

void QAdvancedTableView::setModel( QAbstractItemModel* model )
{
    d->model = model;
    ui->fixedRowsTableView->setModel(d->model);
    d->dataViewProxy->setSourceModel(ui->fixedRowsTableView->decorationProxy());
//    d->dataViewProxy->setSourceModel(d->model);

    d->filterModel->setSourceModel(d->model);
    for(int iCol = 0; iCol < d->horizontalHeader->count(); iCol++){
        ui->dataTableView->horizontalHeader()->resizeSection(iCol, d->horizontalHeader->sectionSize(iCol));
        ui->dataTableView->horizontalHeader()->moveSection(ui->dataTableView->horizontalHeader()->visualIndex(iCol), d->horizontalHeader->visualIndex(iCol));
    }
    if (horizontalHeader()->stretchLastSection()){
        horizontalHeader()->setResizeMode(horizontalHeader()->count() - 1, QHeaderView::Stretch);
    }
}

void QAdvancedTableView::setRowHeight(int row, int height)
{
    ui->dataTableView->setRowHeight(row, height);
}

void QAdvancedTableView::setRootIndex(const QModelIndex & index)
{
    V_CALL(setRootIndex(index))
}

void QAdvancedTableView::showColumn(int column)
{
    horizontalHeader()->showSection(column);
}

void QAdvancedTableView::showRow(int row)
{
    V_CALL(showRow(row))
}

void QAdvancedTableView::setSelectionBehavior(QAbstractItemView::SelectionBehavior behavior)
{
    V_CALL(setSelectionBehavior(behavior))
}

void QAdvancedTableView::setSelectionMode(QAbstractItemView::SelectionMode mode)
{
    V_CALL(setSelectionMode(mode))
}

bool QAdvancedTableView::setFilterType(int type, int column, int row)
{
    QVariantMap mProperties;
    mProperties["type"] = type;
    if (d->filterModel->createFilter(d->filterModel->index(row, column), mProperties) != 0){
        return true;
    }
    return false;
}

void QAdvancedTableView::setFixedRowsModeEnabled(bool on)
{
    ui->fixedRowsTableView->decorationProxy()->setEnabled(on);
}

void QAdvancedTableView::setShowGrid( bool show )
{
    ui->headerTableView->setShowGrid(show);
    V_CALL(setShowGrid(show))
}

void QAdvancedTableView::setSortIndicatorShown( bool show )
{
    ui->headerTableView->horizontalHeader()->setSortIndicatorShown(show);
}

void QAdvancedTableView::setSortingEnabled( bool enable )
{
    V_CALL(setSortingEnabled(enable))
}

QFilterTableViewSettingsDialog* QAdvancedTableView::settingsDialog()
{
    if (d->settingsDialog == 0){
        d->settingsDialog = new QFilterTableViewSettingsDialog(d->filterModel, this);
	}
    return d->settingsDialog;
}

void QAdvancedTableView::setTextElideMode(Qt::TextElideMode mode)
{
    V_CALL(setTextElideMode(mode))
}

void QAdvancedTableView::setFilterVisible(bool visible)
{
    ui->headerTableView->setFilterVisible(visible);
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

void QAdvancedTableView::showSettingsDialog()
{
	QFilterTableViewSettingsDialog* mDlg = settingsDialog();
    mDlg->exec();
}

void QAdvancedTableView::updateHeaderViewHorizontalScrollBar(int min, int max)
{
    ui->headerTableView->horizontalScrollBar()->setRange(min, max);
}

QSize QAdvancedTableView::sizeHint() const
{
    return QSize(QWidget::sizeHint().width(), ui->headerTableView->height() * 2);
}

void QAdvancedTableView::splitView(bool on)
{
    if (on == ui->splittedDataTableView->isVisible()){
        return;
    }
    if (on){
        ui->splittedDataTableView->show();
        if (d->autoResizeRowsToContents){
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

void QAdvancedTableView::update(const QModelIndex & index)
{
    ui->dataTableView->update(index);
}

void QAdvancedTableView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu* m = createStandardContextMenu();

    m->exec(event->globalPos());
    delete m;
}

void QAdvancedTableView::dataModelLayoutChanged()
{
    if (d->autoResizeRowsToContents){
        ui->dataTableView->resizeRowsToContents();
        ui->fixedRowsTableView->resizeRowsToContents();
        ui->splittedDataTableView->resizeRowsToContents();
    }
}

void QAdvancedTableView::updateHeaderViewGeometries()
{
    int rows = ui->headerTableView->model()->rowCount();
    if (rows > ui->headerTableView->maxVisibileFilterSets()){
        rows = ui->headerTableView->maxVisibileFilterSets();
    }
    for (int iRow = 0; iRow < ui->headerTableView->model()->rowCount(); iRow++){
        ui->headerTableView->verticalHeader()->resizeSection(iRow, ui->headerTableView->verticalHeader()->sizeHint().height());
    }
    int mRowHeight = ui->headerTableView->verticalHeader()->sizeHint().height();
    if (mRowHeight == 0){
        mRowHeight = ui->headerTableView->horizontalHeader()->sizeHint().height();
    }
    int mHeaderHeight = ui->headerTableView->horizontalHeader()->sizeHint().height();
    if (mHeaderHeight == 0){
        mHeaderHeight = mRowHeight;
    }
    if (ui->headerTableView->filterVisible()){
        ui->headerTableView->setFixedHeight(mHeaderHeight + mRowHeight * rows + (1 * rows) + 1);
    } else {
        ui->headerTableView->setFixedHeight(ui->headerTableView->horizontalHeader()->sizeHint().height());
    }

    mRowHeight = ui->dataTableView->verticalHeader()->sizeHint().height();
//    ui->fixedRowsTableView->setFixedHeight();
    if (ui->headerTableView->model()->rowCount() > ui->headerTableView->maxVisibileFilterSets()){
        ui->headerTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->dataTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->splittedDataTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
    if (ui->headerTableView->verticalHeader()->width() > ui->dataTableView->verticalHeader()->width()){
        ui->dataTableView->verticalHeader()->setFixedWidth(ui->headerTableView->verticalHeader()->width());
        ui->fixedRowsTableView->verticalHeader()->setFixedWidth(ui->headerTableView->verticalHeader()->width());
        ui->splittedDataTableView->verticalHeader()->setFixedWidth(ui->headerTableView->verticalHeader()->width());
    } else {
        ui->headerTableView->verticalHeader()->setFixedWidth(ui->dataTableView->verticalHeader()->width());
    }
}

void QAdvancedTableView::updateHeaderViewVerticalScrollBar( int min, int max )
{
    Q_UNUSED(min);
    Q_UNUSED(max);
    if (ui->dataTableView->verticalScrollBar()->maximum() == 0){
        ui->headerTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->fixedRowsTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    } else {
        ui->headerTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->fixedRowsTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
}

void QAdvancedTableView::verticalHeaderSectionClicked(int section)
{
    ui->fixedRowsTableView->decorationProxy()->toggleRow(ui->dataTableView->model()->index(section, 0));
}

void QAdvancedTableView::verticalHeaderSectionResized(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(oldSize);
    if (sender() == ui->splittedDataTableView->verticalHeader()){
        ui->dataTableView->verticalHeader()->resizeSection(logicalIndex, newSize);
    } else if (sender() == ui->dataTableView->verticalHeader()){
        ui->splittedDataTableView->verticalHeader()->resizeSection(logicalIndex, newSize);
    }
}

QHeaderView* QAdvancedTableView::verticalHeader() const
{
	return ui->dataTableView->verticalHeader();
}

QWidget* QAdvancedTableView::viewport() const
{
	return ui->dataTableView->viewport();
}

bool QAdvancedTableView::wordWrap() const
{
    return ui->dataTableView->wordWrap();
}

QAdvancedTableViewProxy::QAdvancedTableViewProxy(QWidget *parent) :
    QTableView(parent)
{
}

QAdvancedTableViewProxy::~QAdvancedTableViewProxy()
{
}

void QAdvancedTableViewProxy::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    if (editor->parent()->parent() == this){
        QTableView::closeEditor(editor, hint);
    }
}
