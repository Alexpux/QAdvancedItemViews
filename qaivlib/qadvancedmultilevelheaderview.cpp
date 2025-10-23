/*
Copyright (c) 2009, Krasnoshchekov Petr
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Krasnoshchekov Petr ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Krasnoshchekov Petr BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* **********************************************************************************
 * This code is based on the work done by Petr, however it was modified quite a
 * bit to work on the newer Qt Versions and be slightly more actual.
 * The original code was downloaded from :
 *   https://www.linux-apps.com/content/show.php/HierarchicalHeaderView?content=103154
 * however it felt it required some modifications.
 * Copyright (c) 2019, Mariani Grgo
 * All rights reserved.
 * **********************************************************************************/

#include "qadvancedmultilevelheaderview.h"

#include "qadvancedconstants.h"
#include "qadvancedmultilevelheaderview_p.h"
#include "qadvancedtablemodel.h"

#include <QAbstractItemModel>
#include <QMouseEvent>
#include <QPainter>
#include <QStandardItemModel>
#include <QVariant>

// #ifdef QT_DEBUG
#include <QDebug>
// #endif

QAdvancedHeaderItem *QAdvancedMultiLevelHeaderView::current_active_item = nullptr;

/* *****************************************************************************************************
 * class QAdvancedMultiLevelHeaderView
 * *****************************************************************************************************/

QAdvancedMultiLevelHeaderView::QAdvancedMultiLevelHeaderView(Qt::Orientation orientation, QWidget *parent) :
    QAdvancedHeaderView(orientation, parent),
    d(new QAdvancedMultiLevelHeaderViewPrivate())
{
    setHighlightSections(d->HIGHLIGHT_SECTIONS);
    setSectionsClickable(d->SECTIONS_CLICKABLE);
    setIconSize(HeaderIconSize);

    connect(this, &QAdvancedMultiLevelHeaderView::sectionResized, this, &QAdvancedMultiLevelHeaderView::on_sectionResized);
}

QAdvancedMultiLevelHeaderView::~QAdvancedMultiLevelHeaderView()
{
    delete d;
}

void QAdvancedMultiLevelHeaderView::setModel(QAbstractItemModel *model)
{
    QHeaderView::setModel(model);
    d->initFromNewModel(orientation(), model);
    int cnt = -1;
    if (model) {
        cnt = (orientation() == Qt::Horizontal ? model->columnCount() : model->rowCount());
    }
    if (cnt) {
        initializeSections(0, cnt - 1);
    }
}

QStyleOptionHeader QAdvancedMultiLevelHeaderView::styleOptionForCell(int logicalIdx) const
{
    QStyleOptionHeader opt;
    initStyleOption(&opt);

    QStyle::State state = QStyle::State_None;
    if (isEnabled()) {
        state |= QStyle::State_Enabled;
    }
    if (window()->isActiveWindow()) {
        state |= QStyle::State_Active;
    }

    if (sectionsClickable()) {
        if (highlightSections() && selectionModel()) {
            if (orientation() == Qt::Horizontal) {
                if (selectionModel()->columnIntersectsSelection(logicalIdx, rootIndex())) {
                    state |= QStyle::State_On;
                }
                if (selectionModel()->isColumnSelected(logicalIdx, rootIndex())) {
                    state |= QStyle::State_Sunken;
                }
            } else {
                if (selectionModel()->rowIntersectsSelection(logicalIdx, rootIndex())) {
                    state |= QStyle::State_On;
                }
                if (selectionModel()->isRowSelected(logicalIdx, rootIndex())) {
                    state |= QStyle::State_Sunken;
                }
            }
        }
    }
    opt.state |= state;

    int visual = visualIndex(logicalIdx);
    if (count() == 1) {
        opt.position = QStyleOptionHeader::OnlyOneSection;
    } else {
        if (visual == 0) {
            opt.position = QStyleOptionHeader::Beginning;
        } else {
            opt.position = (visual == count() - 1 ? QStyleOptionHeader::End : QStyleOptionHeader::Middle);
        }
    }

    if (selectionModel()) {
        bool previousSelected = false;
        if (orientation() == Qt::Horizontal) {
            previousSelected = selectionModel()->isColumnSelected(logicalIndex(visual - 1), rootIndex());
        } else {
            previousSelected = selectionModel()->isRowSelected(logicalIndex(visual - 1), rootIndex());
        }
        bool nextSelected = false;
        if (orientation() == Qt::Horizontal) {
            nextSelected = selectionModel()->isColumnSelected(logicalIndex(visual + 1), rootIndex());
        } else {
            nextSelected = selectionModel()->isRowSelected(logicalIndex(visual + 1), rootIndex());
        }
        if (previousSelected && nextSelected) {
            opt.selectedPosition = QStyleOptionHeader::NextAndPreviousAreSelected;
        } else {
            if (previousSelected) {
                opt.selectedPosition = QStyleOptionHeader::PreviousIsSelected;
            } else {
                if (nextSelected) {
                    opt.selectedPosition = QStyleOptionHeader::NextIsSelected;
                } else {
                    opt.selectedPosition = QStyleOptionHeader::NotAdjacent;
                }
            }
        }
    }

    opt.textAlignment = Qt::AlignCenter;
    opt.iconAlignment = Qt::AlignVCenter;
    opt.section = logicalIdx;

    /*if (isSortIndicatorShown() && sortIndicatorSection() == logicalInd) {
        opt.sortIndicator = (sortIndicatorOrder() == Qt::AscendingOrder) ? QStyleOptionHeader::SortDown : QStyleOptionHeader::SortUp;
    }*/

    return opt;
}

void QAdvancedMultiLevelHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    if (rect.isValid()) {
        QModelIndex childIndex(d->leafIndex(logicalIndex));
        if (childIndex.isValid()) {
            if (orientation() == Qt::Horizontal) {
                d->paintHorizontalSection(painter, rect, logicalIndex, this, styleOptionForCell(logicalIndex), childIndex);
            } else {
                d->paintVerticalSection(painter, rect, logicalIndex, this, styleOptionForCell(logicalIndex), childIndex);
            }
            return;
        }
    }
    QHeaderView::paintSection(painter, rect, logicalIndex);
}

QSize QAdvancedMultiLevelHeaderView::sectionSizeFromContents(int logicalIndex) const
{
    if (d->headerModel) {
        QModelIndex curLeafIndex(d->leafIndex(logicalIndex));
        if (curLeafIndex.isValid()) {
            QStyleOptionHeader styleOption(styleOptionForCell(logicalIndex));
            QSize s = d->enhancedCellSize(curLeafIndex, this, styleOption);
            curLeafIndex = curLeafIndex.parent();

            int depth = d->getDepth(curLeafIndex);

            while (curLeafIndex.isValid()) {
                if (depth >= d->minimumPaintepth) {
                    if (orientation() == Qt::Horizontal) {
                        s.rheight() += d->enhancedCellSize(curLeafIndex, this, styleOption).height();
                    } else {
                        s.rwidth() += d->enhancedCellSize(curLeafIndex, this, styleOption).width();
                    }
                }
                curLeafIndex = curLeafIndex.parent();
                --depth;
            }
            return s;
        }
    }
    return QHeaderView::sectionSizeFromContents(logicalIndex);
}

void QAdvancedMultiLevelHeaderView::mousePressEvent(QMouseEvent *e)
{
    current_active_item = nullptr;
    QHeaderView::mousePressEvent(e);
    int li = logicalIndexAt(e->pos());
    QModelIndex childIndex(d->leafIndex(li));
    if (childIndex.isValid() && e->button() == Qt::LeftButton) {
        int depth = d->getDepthOfCurrentlyClickedItem(this, e->pos(), styleOptionForCell(li), childIndex, orientation()); // li=logical index
        current_active_item = qobject_cast<QAdvancedTableModel *>(model())->GetRootItem(orientation())->GetLeaf(li)->GetParentItemAtDepth(depth);
    }
}

void QAdvancedMultiLevelHeaderView::mouseReleaseEvent(QMouseEvent *e)
{
    QHeaderView::mouseReleaseEvent(e);
    int li = logicalIndexAt(e->pos());
    QModelIndex childIndex(d->leafIndex(li));
    if (childIndex.isValid() && e->button() == Qt::LeftButton) {
        int depth = d->getDepthOfCurrentlyClickedItem(this, e->pos(), styleOptionForCell(li), childIndex, orientation()); // li=logical index
        QAdvancedHeaderItem *clicked_item = qobject_cast<QAdvancedTableModel *>(model())->GetRootItem(orientation())->GetLeaf(li)->GetParentItemAtDepth(depth);
        if (current_active_item == clicked_item) {
            if (d->GROUP_ITEMS_ON_CLICK) {
                clicked_item->PlaceholderToggle(); // <--- 'clicked_item' was clicked in the header
                qobject_cast<QAdvancedTableModel *>(model())->emit_layoutChanged();
            }
        }
    }
}

void QAdvancedMultiLevelHeaderView::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::StyleChange
        || event->type() == QEvent::PaletteChange
        || event->type() == QEvent::ThemeChange) {
        // Update the view when style or theme changes
        updateGeometry();
        viewport()->update();
    }
    QAdvancedHeaderView::changeEvent(event);
}

void QAdvancedMultiLevelHeaderView::on_sectionResized(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(oldSize)
    Q_UNUSED(newSize)
    if (isSectionHidden(logicalIndex)) {
        return;
    }

    QModelIndex childIndex(d->leafIndex(logicalIndex));
    if (childIndex.isValid()) {
        QModelIndexList leafsList(d->leafs(d->findRootIndex(childIndex)));
        for (int n = leafsList.indexOf(childIndex); n > 0; --n) {
            --logicalIndex;

            int w = viewport()->width();
            int h = viewport()->height();
            int pos = sectionViewportPosition(logicalIndex);
            QRect r(pos, 0, w - pos, h);
            if (orientation() == Qt::Horizontal) {
                if (isRightToLeft()) {
                    r.setRect(0, 0, pos + sectionSize(logicalIndex), h);
                }
            } else {
                r.setRect(0, pos, w, h - pos);
            }

            viewport()->update(r.normalized());
        }
    }
}

void QAdvancedMultiLevelHeaderView::setGroupItemsByClick(bool value)
{
    d->GROUP_ITEMS_ON_CLICK = value;
}

void QAdvancedMultiLevelHeaderView::setRootLevelVisible(bool visible)
{
    if (d->rootLevelVisible != visible) {
        d->rootLevelVisible = visible;
        d->minimumPaintepth = visible ? 0 : 1;

        updateGeometry();
        viewport()->update();
    }
}

bool QAdvancedMultiLevelHeaderView::isRootLevelVisible() const noexcept
{
    return d->rootLevelVisible;
}

void QAdvancedMultiLevelHeaderView::setMinimumPaintDepth(int depth)
{
    if (d->minimumPaintepth != depth) {
        d->minimumPaintepth = std::max(0, depth);
        d->rootLevelVisible = (depth == 0);

        updateGeometry();
        viewport()->update();
    }
}

int QAdvancedMultiLevelHeaderView::minimumPaintDepth() const noexcept
{
    return d->minimumPaintepth;
}
