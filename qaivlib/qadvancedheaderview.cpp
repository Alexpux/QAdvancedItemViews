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
#include "qadvancedheaderview.h"

#include "qabstractfiltermodel.h"
#include "qfilterview.h"

QAdvancedHeaderView::QAdvancedHeaderView( Qt::Orientation orientation, QWidget* parent )
    : QHeaderView(orientation, parent)
{
    setClickable(true);
}

QAdvancedHeaderView::~QAdvancedHeaderView()
{
}

void QAdvancedHeaderView::addFilterSetActionTriggered()
{
    QAction* mAction = qobject_cast<QAction*>(sender());
    if (mAction){
        model()->insertRows(mAction->data().toInt(), 1);
    } else {
        model()->insertRows(model()->rowCount(), 1);
    }
}

void QAdvancedHeaderView::contextMenuEvent(QContextMenuEvent* event)
{
    QAction* mAction;
    QMenu mMenu;
//    QAbstractItemModel* mModel = model();
//    QAbstractFilterModel* mFilterModel = 0;
//    QSortFilterProxyModel* mProxy = qobject_cast<QSortFilterProxyModel*>(model());
//    if (mProxy){
//        mModel = mProxy->sourceModel();
//        mFilterModel = qobject_cast<QAbstractFilterModel*>(mProxy);
//    }
    QAbstractFilterModel* mFilterModel = qobject_cast<QAbstractFilterModel*>(model());
    if (orientation() == Qt::Horizontal){
        int mColumn = logicalIndexAt(event->pos());
        if (mColumn > -1){
            mAction = new QAction(QIcon(":/qadvancedtableviews/column.hide"), QString(tr("Hide Column '%1'")).arg(model()->headerData(mColumn, Qt::Horizontal).toString()), &mMenu);
            connect(mAction, SIGNAL(triggered()), this, SLOT(hideSectionActionTriggered()));
            mAction->setData(mColumn);
            mMenu.addAction(mAction);
        }
        mMenu.addAction(tr("Show All Columns"), this, SLOT(showAllColumns()));
        QVector<QAction*> mActions(model()->columnCount() < 11?model()->columnCount():10);
        for (int iColumn = 0; iColumn < model()->columnCount() && iColumn < 10; iColumn++){
            mAction = new QAction(model()->headerData(iColumn, Qt::Horizontal).toString(), &mMenu);
            mAction->setCheckable(true);
            mAction->setChecked(!isSectionHidden(iColumn));
            mAction->setData(iColumn);
            mActions[visualIndex(iColumn)] = mAction;
            connect(mAction, SIGNAL(toggled(bool)), this, SLOT(sectionToggled(bool)));
        }
        mMenu.addSeparator();
        QMenu* mColumnsMenu = mMenu.addMenu(QIcon(":/qadvancedtableviews/columns.select"), tr("Columns"));
        for (int iAction = 0; iAction < mActions.size(); iAction++){
            mColumnsMenu->addAction(mActions.at(iAction));
        }
        if (model()->columnCount() > 10){
            mColumnsMenu->addSeparator();
            mColumnsMenu->addAction(tr("More Columns..."), this, SLOT(moreColumnsActionTriggered()));
        }
        if (mFilterModel){
            mMenu.addSeparator();
            QMenu* mModeMenu = mMenu.addMenu(tr("Mode"));
            mAction = mModeMenu->addAction(tr("Filter"), this, SLOT(filterModeActionTriggered()));
            mAction->setCheckable(true);
            mAction->setChecked(mFilterModel->mode() == QAdvancedItemViews::FilterMode);

            mAction = mModeMenu->addAction(tr("Highlight"), this, SLOT(highlightModeActionTriggered()));
            mAction->setCheckable(true);
            mAction->setChecked(mFilterModel->mode() == QAdvancedItemViews::HighlightMode);

            mModeMenu->addSeparator();
            mAction = mModeMenu->addAction(tr("Inverted"), this, SLOT(invertedMatchModeActionTriggered()));
            mAction->setCheckable(true);
            mAction->setChecked(mFilterModel->matchMode() == QAdvancedItemViews::MatchInverted);
        }
        QFilterView* mView = qobject_cast<QFilterView*>(parentWidget());
        if (mView != 0){
            mMenu.addSeparator();
            if (mView->filterVisible()){
                mMenu.addAction(tr("Hide Filter"), this, SLOT(hideFilterActionTriggered()));
            } else {
                mMenu.addAction(tr("Show Filter"), this, SLOT(showFilterActionTriggered()));
            }
        }
    } else if (orientation() == Qt::Vertical){
        int mLogicalIndex = logicalIndexAt(event->pos());

        mAction = mMenu.addAction(QIcon(":/qaiv/filter/add"), tr("Add Filter Set"), this, SLOT(addFilterSetActionTriggered()));
        mAction->setData(mLogicalIndex);
        mAction = mMenu.addAction(QIcon(":/qaiv/filter/rename"), tr("Rename Filter Set"), this, SLOT(renameFilterActionTriggered()));
        mAction->setData(mLogicalIndex);
        //
        mAction = mMenu.addAction(QIcon(":/qaiv/filter/delete"), tr("Remove Filter Set"), this, SLOT(removeFilterSetActionTriggered()));
        mAction->setData(mLogicalIndex);
        mAction->setEnabled(model()->rowCount() > 1);
    }
    event->accept();
    mMenu.exec(mapToGlobal(event->pos()));
}

void QAdvancedHeaderView::filterModeActionTriggered()
{
    QAbstractFilterModel* mFilterModel = qobject_cast<QAbstractFilterModel*>(model());
    if (mFilterModel){
        mFilterModel->setMode(QAdvancedItemViews::FilterMode);
    }
}

void QAdvancedHeaderView::hideFilterActionTriggered()
{
    QFilterView* mView = qobject_cast<QFilterView*>(parentWidget());
    if (mView){
        mView->setFilterVisible(false);
    }
}
void QAdvancedHeaderView::hideSectionActionTriggered()
{
    QAction* mAction = qobject_cast<QAction*>(sender());
    if (mAction){
        hideSection(mAction->data().toInt());
    }
}

void QAdvancedHeaderView::highlightModeActionTriggered()
{
    QAbstractFilterModel* mFilterModel = qobject_cast<QAbstractFilterModel*>(model());
    if (mFilterModel){
        mFilterModel->setMode(QAdvancedItemViews::HighlightMode);
    }
}

void QAdvancedHeaderView::invertedMatchModeActionTriggered()
{
    QAbstractFilterModel* mFilterModel = qobject_cast<QAbstractFilterModel*>(model());
    if (mFilterModel){
        if (mFilterModel->matchMode() == QAdvancedItemViews::MatchNormal){
            mFilterModel->setMatchMode(QAdvancedItemViews::MatchInverted);
        } else {
            mFilterModel->setMatchMode(QAdvancedItemViews::MatchNormal);
        }
    }
}

void QAdvancedHeaderView::moreColumnsActionTriggered()
{
    QDialog* mDialog = new QDialog(this,Qt::Widget);
    mDialog->setWindowTitle(tr("More columns..."));
    QVBoxLayout* mLayout = new QVBoxLayout(mDialog);

    QTableView* mView = new QTableView(mDialog);
    mLayout->addWidget(mView);

    QDialogButtonBox* mButtonBox = new QDialogButtonBox(mDialog);
    mButtonBox->setOrientation(Qt::Horizontal);
    mButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    mLayout->addWidget(mButtonBox);

    QObject::connect(mButtonBox, SIGNAL(accepted()), mDialog, SLOT(accept()));
    QObject::connect(mButtonBox, SIGNAL(rejected()), mDialog, SLOT(reject()));

    QAbstractItemModel* mModel = model();
    QAbstractFilterModel* mHeaderProxy;
    QSortFilterProxyModel* mProxy = qobject_cast<QSortFilterProxyModel*>(model());
    if (mProxy){
        mModel = mProxy->sourceModel();
        mHeaderProxy = qobject_cast<QAbstractFilterModel*>(mProxy);
    }
    QStandardItemModel* mColumnModel = new QStandardItemModel(mDialog);
    mColumnModel->setColumnCount(1);
    mColumnModel->setRowCount(mModel->columnCount());

    mView->setModel(mColumnModel);
    mView->horizontalHeader()->setStretchLastSection(true);
    mView->horizontalHeader()->setVisible(false);
    mView->verticalHeader()->setVisible(false);

    QStandardItem* mItem;
    for (int iColumn = 0; iColumn < mModel->columnCount(); iColumn++){
        mItem = new QStandardItem(mModel->headerData(iColumn, Qt::Horizontal).toString());
        mItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        mItem->setData(isSectionHidden(iColumn)?Qt::Unchecked:Qt::Checked, Qt::CheckStateRole);
        mColumnModel->setItem(iColumn, 0, mItem);
    }
    mView->resizeRowsToContents();
    if (mDialog->exec()){
        for (int iRow = 0; iRow < mColumnModel->rowCount(); iRow++){
            setSectionHidden(iRow, mColumnModel->index(iRow, 0).data(Qt::CheckStateRole).toInt() == Qt::Unchecked);
        }
    }
    delete mDialog;
}

void QAdvancedHeaderView::removeFilterSetActionTriggered()
{
    QAction* mAction = qobject_cast<QAction*>(sender());
    if (mAction){
        if (QMessageBox::question(this, tr("Remove Filter"), QString(tr("Please confirm to delete filter '%1'.")).arg(model()->headerData(mAction->data().toInt(), Qt::Vertical, Qt::DisplayRole).toString()), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes){
            model()->removeRows(mAction->data().toInt(), 1);
        }
    }
}

void QAdvancedHeaderView::renameFilterActionTriggered()
{
    QAction* mAction = qobject_cast<QAction*>(sender());
    if (mAction){
        QString mName = QInputDialog::getText(this, tr("Rename Filter Set"), tr("Name"), QLineEdit::Normal, model()->headerData(mAction->data().toInt(), Qt::Vertical, Qt::DisplayRole).toString());
        if (!mName.isEmpty()){
            model()->setHeaderData(mAction->data().toInt(), Qt::Vertical, mName);
        }
    }
}

void QAdvancedHeaderView::sectionToggled( bool on )
{
    QAction* mAction = qobject_cast<QAction*>(sender());
    if (mAction){
        setSectionHidden(mAction->data().toInt(), !on);
    }
}

void QAdvancedHeaderView::showAllColumns()
{
    for (int iSection = 0; iSection < count(); iSection++){
        showSection(iSection);
    }
}

void QAdvancedHeaderView::showFilterActionTriggered()
{
    QFilterView* mView = qobject_cast<QFilterView*>(parentWidget());
    if (mView){
        mView->setFilterVisible(true);
    }
}

int QAdvancedHeaderView::sizeHintForRow(int row) const
{
    return 25;
}

void QAdvancedHeaderView::toggleSortOrderActionTriggered()
{
    QAction* mAction = qobject_cast<QAction*>(sender());
    if (mAction){
        Qt::SortOrder mSortOrder = sortIndicatorOrder();
        if (mSortOrder == Qt::AscendingOrder){
            model()->sort(sortIndicatorSection(), Qt::DescendingOrder);
        } else {
            model()->sort(sortIndicatorSection(), Qt::AscendingOrder);
        }
    }
}
