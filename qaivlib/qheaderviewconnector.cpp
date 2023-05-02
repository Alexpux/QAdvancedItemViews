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

#include "qheaderviewconnector.h"

#include <QScrollBar>
#include <QTableView>

QHeaderViewConnector::QHeaderViewConnector(Qt::Orientation orientation, QObject *parent) :
    QObject(parent),
    cAdjustSectionSizePending(true),
    cOrientation(orientation)
{
}

void QHeaderViewConnector::append(QHeaderView* view)
{
    if (view->orientation() != cOrientation) {
        return;
    }
    cHeaderViewList.append(view);
    connect(view, &QHeaderView::destroyed, this, &QHeaderViewConnector::viewDestroyed);
    connect(view, &QHeaderView::sectionMoved, this, &QHeaderViewConnector::sectionMoved);
    connect(view, &QHeaderView::sectionResized, this, &QHeaderViewConnector::sectionResized);
    connect(view, &QHeaderView::sortIndicatorChanged, this, &QHeaderViewConnector::sortIndicatorChanged);

    if (cOrientation == Qt::Horizontal) {
        QTableView* mView = qobject_cast<QTableView*>(view->parentWidget());
        if (mView) {
            connect(mView->horizontalScrollBar(), &QScrollBar::rangeChanged, this, &QHeaderViewConnector::scrollBarRangeChanged);
            connect(mView->horizontalScrollBar(), &QScrollBar::sliderMoved, this, &QHeaderViewConnector::scrollBarSilderMoved);
            connect(mView->horizontalScrollBar(), &QScrollBar::valueChanged, this, &QHeaderViewConnector::scrollBarSilderMoved);
        }
    }
}

void QHeaderViewConnector::adjustSectionSize()
{
    cAdjustSectionSizePending = false;
    int mWidth = 0;
    for (QHeaderView* mHeaderView : cHeaderViewList) {
        if (mHeaderView) {
            if (mWidth < mHeaderView->width()) {
                mWidth = mHeaderView->width();
            }
        }
    }
    if (mWidth > 0) {
        for (QHeaderView* mHeaderView : cHeaderViewList) {
            if (mHeaderView) {
                mHeaderView->blockSignals(true);
                mHeaderView->setFixedWidth(mWidth);
                mHeaderView->blockSignals(false);
            }
        }
    }
}

void QHeaderViewConnector::scrollBarRangeChanged(int min, int max)
{
    Q_UNUSED(min);
    Q_UNUSED(max);
    if (cOrientation == Qt::Vertical) {
        for (QHeaderView* mHeaderView : cHeaderViewList) {
            if (mHeaderView) {
                QTableView* mTableView = qobject_cast<QTableView*>(mHeaderView->parentWidget());
                if (mTableView) {
                    if (mTableView->verticalScrollBar()->isVisible()) {

                    }
                }
            }
        }
    }
    //    QHeaderView* mSender = qobject_cast<QHeaderView*>(sender());
    //    for (QHeaderView* mHeaderView : cHeaderViewList)  {
    //        if (mHeaderView && mHeaderView != mSender){
    //            QTableView* mTableView = qobject_cast<QTableView*>(mHeaderView->parentWidget());
    //            if (mTableView ){
    //                if (cOrientation == Qt::Horizontal) {
    //                    mTableView->horizontalScrollBar()->setRange(min, max);
    //                }
    //            }
    //        }
    //    }
}

void QHeaderViewConnector::scrollBarSilderMoved(int value)
{
    QHeaderView* mSender = qobject_cast<QHeaderView*>(sender());
    for (QHeaderView* mHeaderView : cHeaderViewList) {
        if (mHeaderView && mHeaderView != mSender) {
            QTableView* mTableView = qobject_cast<QTableView*>(mHeaderView->parentWidget());
            if (mTableView) {
                if (cOrientation == Qt::Horizontal) {
                    mTableView->horizontalScrollBar()->setValue(value);
                }
            }
        }
    }
}

void QHeaderViewConnector::sectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    Q_UNUSED(logicalIndex);
    QHeaderView* mSender = qobject_cast<QHeaderView*>(sender());
    for (QHeaderView* mHeaderView : cHeaderViewList) {
        if (mHeaderView && mHeaderView != mSender) {
            mHeaderView->blockSignals(true);
            mHeaderView->moveSection(oldVisualIndex, newVisualIndex);
            mHeaderView->blockSignals(false);
        }
    }
}

void QHeaderViewConnector::sectionResized(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(oldSize);
    QHeaderView* mSender = qobject_cast<QHeaderView*>(sender());
    for (QHeaderView* mHeaderView : cHeaderViewList) {
        if (mHeaderView && mHeaderView != mSender) {
            mHeaderView->resizeSection(logicalIndex, newSize);
        }
    }
    if (cAdjustSectionSizePending) {
        adjustSectionSize();
    }
}

void QHeaderViewConnector::sortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    QHeaderView* mSender = qobject_cast<QHeaderView*>(sender());
    for (QHeaderView* mHeaderView : cHeaderViewList) {
        if (mHeaderView && mHeaderView != mSender) {
            mHeaderView->blockSignals(true);
            mHeaderView->setSortIndicator(logicalIndex, order);
            mHeaderView->blockSignals(false);
        }
    }
}

void QHeaderViewConnector::viewDestroyed(QObject* object)
{
    cHeaderViewList.removeAt(cHeaderViewList.indexOf(qobject_cast<QHeaderView*>(object)));
}
