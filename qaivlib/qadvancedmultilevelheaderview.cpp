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

    return opt;
}

QStyleOptionHeader QAdvancedMultiLevelHeaderView::styleOptionFromModel(const QStyleOptionHeader &opt, const QModelIndex &index) const
{
    QStyleOptionHeader uni(opt);

    if (model()) {
        // Foreground color - use model data if provided, otherwise default
        QVariant fgBrush = index.data(Qt::ForegroundRole);
        if (fgBrush.canConvert(QMetaType::QBrush)) {
            uni.palette.setBrush(QPalette::ButtonText, qvariant_cast<QBrush>(fgBrush));
            // uni.palette.setBrush(QPalette::WindowText, qvariant_cast<QBrush>(fgBrush));
        }

        // Background color - use model data if provided, otherwise default
        QVariant bgBrush = index.data(Qt::BackgroundRole);
        if (bgBrush.canConvert(QMetaType::QBrush)) {
            uni.palette.setBrush(QPalette::Button, qvariant_cast<QBrush>(bgBrush));
            uni.palette.setBrush(QPalette::Window, qvariant_cast<QBrush>(bgBrush));
        }

        QVariant decorationData = index.data(Qt::DecorationRole);
        if (decorationData.isValid()) {
            if (decorationData.canConvert(QMetaType::QIcon)) {
                uni.icon = qvariant_cast<QIcon>(decorationData);
            } else if (decorationData.canConvert(QMetaType::QPixmap)) {
                QPixmap pixmap = qvariant_cast<QPixmap>(decorationData);
                uni.icon = QIcon(pixmap);
            }
            uni.iconAlignment = Qt::AlignCenter;
        } else {
            // Clear icon if no decoration
            uni.icon = QIcon();
        }

        // Text alignment - use model data if provided, otherwise default to center
        QVariant alignmentVar = index.data(Qt::TextAlignmentRole);
        if (alignmentVar.isValid()) {
            uni.textAlignment = Qt::Alignment(alignmentVar.toInt());
        } else {
            uni.textAlignment = Qt::AlignCenter; // Default for headers
        }

        QFont fnt(font());
        QVariant varFont(index.data(Qt::FontRole));
        if (varFont.isValid() && varFont.canConvert(QMetaType::QFont)) {
            fnt = qvariant_cast<QFont>(varFont);
        }
        // fnt.setBold(true);
        QFontMetrics fm(fnt);
        uni.fontMetrics = std::move(fm);

        QString txt = index.data(Qt::DisplayRole).toString();
        txt = txt.replace(QStringLiteral("<br>"), QStringLiteral("\n"));
        uni.text = txt;
    }

    return uni;
}

int QAdvancedMultiLevelHeaderView::minimumSpanWidth(const QModelIndex &cellIndex) const
{
    if (!cellIndex.isValid()) {
        return 0;
    }

    QModelIndexList childLeaves = searchLeafs(cellIndex);

    if (childLeaves.isEmpty()) {
        QVariant orderData = cellIndex.data(CustomRoles::Item_Order_Identify);
        if (orderData.isValid()) {
            int logicalIdx = orderData.toInt();
            return sectionSize(logicalIdx);
        }
        return 0;
    }

    int totalWidth = 0;

    // Sum up the current widths of all child columns
    for (const QModelIndex &leafIdx : childLeaves) {
        QVariant orderData = leafIdx.data(CustomRoles::Item_Order_Identify);
        if (orderData.isValid()) {
            int logicalIdx = orderData.toInt();
            totalWidth += sectionSize(logicalIdx);
        }
    }

    return totalWidth;
}

QSize QAdvancedMultiLevelHeaderView::cellSize(const QModelIndex &leafIdx, QStyleOptionHeader &styleOptions) const
{
    QSize res;
    QVariant variant(leafIdx.data(Qt::SizeHintRole));
    if (variant.isValid()) {
        res = qvariant_cast<QSize>(variant);
    }

    int section = sortIndicatorSection();
    QModelIndex idx = leafIndex(section);

    int margin_px = style()->pixelMetric(QStyle::PM_HeaderMargin, &styleOptions, this);
    int margin = 2 * margin_px;

    if (isSortIndicatorShown() && idx == leafIdx) {
        margin += style()->pixelMetric(QStyle::PM_HeaderMarkSize, nullptr, this);
        // margin += margin_px;
        styleOptions.sortIndicator = (sortIndicatorOrder() == Qt::AscendingOrder)
            ? QStyleOptionHeader::SortDown
            : QStyleOptionHeader::SortUp;
    }

    QString txt = styleOptions.text;
    bool isMultiline = txt.contains('\n') || txt.contains(QStringLiteral("<br>"));
    bool isRotated = leafIdx.data(CustomRoles::Item_Rotated).isValid();
    bool hasIcon = !styleOptions.icon.isNull();

    // Calculate icon size if present
    QSize iconSize(0, 0);
    if (hasIcon) {
        iconSize = this->iconSize();
        if (!iconSize.isValid() || iconSize.isEmpty()) {
            int icon_h = style()->pixelMetric(QStyle::PM_SmallIconSize, nullptr, this);
            iconSize = QSize(icon_h, icon_h); // Default size
        }
        // Add margin between icon and text
        margin += margin_px;
    }

    int minWidth = minimumSpanWidth(leafIdx);

    // Calculate text size properly for multiline text
    QSize textSize;
    if (isMultiline) {
        // Multiline text - use boundingRect for proper calculation
        // For parent cells that span multiple columns, use a reasonable initial width
        // that will be adjusted later in enhancedCellSize
        QRect textRect = styleOptions.fontMetrics.boundingRect(
            QRect(0, 0, minWidth - 2 * margin, 1000),
            Qt::TextWordWrap | styleOptions.textAlignment,
            txt);
        textSize = textRect.size();
    } else {
        // Single line text
        textSize = styleOptions.fontMetrics.size(0, txt);
    }

    // Calculate total content size
    QSize totalSize;
    totalSize.setWidth(textSize.width() + iconSize.width() + margin);
    totalSize.setHeight(std::max(textSize.height(), iconSize.height()) + margin);

    // Use maximum of all calculated sizes
    res = res.expandedTo(totalSize);

    if (isRotated) {
        res.transpose();
    }

    return res;
}

int QAdvancedMultiLevelHeaderView::currentCellWidth(const QModelIndex &searchedIndex,
                                                    const QModelIndex &leafIndex,
                                                    int sectionIndex) const
{
    QModelIndexList leafsList(leafs(searchedIndex));
    if (leafsList.empty()) {
        return sectionSize(sectionIndex);
    }
    int width = 0;
    int firstLeafSectionIndex = sectionIndex - leafsList.indexOf(leafIndex);
    for (int i = 0; i < leafsList.size(); ++i) {
        width += sectionSize(firstLeafSectionIndex + i);
    }
    return width;
}

int QAdvancedMultiLevelHeaderView::currentCellLeft(const QModelIndex &searchedIndex,
                                                   const QModelIndex &leafIndex,
                                                   int sectionIndex,
                                                   int left) const
{
    QModelIndexList leafsList(leafs(searchedIndex));
    if (!leafsList.empty()) {
        int n = leafsList.indexOf(leafIndex);
        int firstLeafSectionIndex = sectionIndex - n;
        --n;
        for (; n >= 0; --n) {
            left -= sectionSize(firstLeafSectionIndex + n);
        }
    }
    return left;
}

QModelIndex QAdvancedMultiLevelHeaderView::findRootIndex(const QModelIndex &index) const
{
    return index.parent().isValid() ? findRootIndex(index.parent()) : index;
}

QModelIndex QAdvancedMultiLevelHeaderView::findLeaf(const QModelIndex &currIndex,
                                                    const int target,
                                                    int &currentLeafIndex) const
{
    if (currIndex.isValid()) {
        int childCount = currIndex.model()->rowCount(currIndex);
        if (childCount) {
            for (int i = 0; i < childCount; ++i) {
                QModelIndex result = findLeaf(currIndex.model()->index(i, 0, currIndex), target, currentLeafIndex);
                if (result.isValid()) {
                    return result;
                }
            }
        } else {
            ++currentLeafIndex;
            if (currentLeafIndex == target) {
                return currIndex;
            }
        }
    }
    return QModelIndex();
}

QModelIndex QAdvancedMultiLevelHeaderView::leafIndex(const int sectionIndex) const
{
    if (d->headerModel) {
        int currentLeafIndex = -1;
        for (int i = 0; i < d->headerModel->rowCount(); i++) {
            QModelIndex result = findLeaf(d->headerModel->index(i, 0), sectionIndex, currentLeafIndex);
            if (result.isValid()) {
                return result;
            }
        }
    }
    return QModelIndex();
}

QModelIndexList QAdvancedMultiLevelHeaderView::searchLeafs(const QModelIndex &currentIndex) const
{
    QModelIndexList result;
    if (currentIndex.isValid()) {
        int childCount = currentIndex.model()->rowCount(currentIndex);
        if (childCount) {
            for (int i = 0; i < childCount; ++i) {
                result += searchLeafs(currentIndex.model()->index(i, 0, currentIndex));
            }
        } else {
            result.push_back(currentIndex);
        }
    }
    return result;
}

QModelIndexList QAdvancedMultiLevelHeaderView::leafs(const QModelIndex &searchedIndex) const
{
    QModelIndexList leafs;
    if (searchedIndex.isValid()) {
        int childCount = searchedIndex.model()->rowCount(searchedIndex);
        for (int i = 0; i < childCount; ++i) {
            leafs += searchLeafs(searchedIndex.model()->index(i, 0, searchedIndex));
        }
    }
    return leafs;
}

int QAdvancedMultiLevelHeaderView::logicalIndexFor(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return -1;
    }

    // If leaf, has Item_Order_Identity
    QVariant orderData = index.data(CustomRoles::Item_Order_Identify);
    if (orderData.isValid()) {
        return orderData.toInt();
    }

    // If parent, get firt child's index
    QModelIndexList leaves = searchLeafs(index);
    if (!leaves.isEmpty()) {
        QVariant firstData = leaves.first().data(CustomRoles::Item_Order_Identify);
        if (firstData.isValid()) {
            return firstData.toInt();
        }
    }

    return -1;
}

QModelIndexList QAdvancedMultiLevelHeaderView::parentIndexes(const QModelIndex &index) const
{
    QModelIndexList indexes;
    indexes.prepend(index);
    while (indexes.first().isValid()) {
        indexes.prepend(indexes.first().parent());
    }
    indexes.removeFirst();
    return indexes;
}

int QAdvancedMultiLevelHeaderView::getDepth(const QModelIndex &index) const
{
    int depth = 0;
    QModelIndex current = index;
    while (current.parent().isValid()) {
        depth++;
        current = current.parent();
    }
    return depth;
}

int QAdvancedMultiLevelHeaderView::getDepthOfCurrentlyClickedItem(const QPoint point,
                                                                  const QStyleOptionHeader &styleOptions,
                                                                  const QModelIndex &leafIndex,
                                                                  Qt::Orientation orientation) const
{
    int xy = (orientation == Qt::Horizontal) ? point.y() : point.x(); // XY - position; WH - width, height
    QModelIndexList indexes(parentIndexes(leafIndex));
    int result = 0;
    int startIndex = std::max(0, d->minimumPaintepth);
    for (int i = startIndex; i < indexes.size(); ++i) {
        QStyleOptionHeader uniopt(styleOptions);
        QSize box = cellSize(indexes[i], uniopt);
        int wh = (orientation == Qt::Horizontal) ? box.height() : box.width();
        result += wh;
        if (xy <= result) {
            return i;
        }
    }
    return indexes.size() - 1;
}

QAdvancedMultiLevelHeaderView::SectionRange
QAdvancedMultiLevelHeaderView::getChildSectionRange(QAdvancedHeaderItem *item) const
{
    if (!item || item->Size() == 0) {
        return { -1, -1 };
    }

    // Get first child leaf
    QAdvancedHeaderItem *firstLeaf = (*item)[0];
    while (firstLeaf->Size() > 0) {
        firstLeaf = (*firstLeaf)[0];
    }

    // Get last child leaf
    QAdvancedHeaderItem *lastChild = (*item)[item->Size() - 1];
    QAdvancedHeaderItem *lastLeaf = lastChild;
    while (lastLeaf->Size() > 0) {
        lastLeaf = (*lastLeaf)[lastLeaf->Size() - 1];
    }

    // Get their column indices from Item_Order_Identify
    QVariant firstData = firstLeaf->data(CustomRoles::Item_Order_Identify);
    QVariant lastData = lastLeaf->data(CustomRoles::Item_Order_Identify);

    if (firstData.isValid() && lastData.isValid()) {
        return { firstData.toInt(), lastData.toInt() };
    }
    return { -1, -1 };
}

void QAdvancedMultiLevelHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    if (rect.isValid()) {
        QModelIndex childIndex(leafIndex(logicalIndex));
        if (childIndex.isValid()) {

            int pos = orientation() == Qt::Horizontal ? rect.y() : rect.x();

            QStyleOptionHeader styleOptions = styleOptionForCell(logicalIndex);
            QPointF oldBO(painter->brushOrigin());

            QModelIndexList indexes(parentIndexes(childIndex));
            int startIndex = std::max(0, d->minimumPaintepth);

            for (int i = startIndex; i < indexes.size(); ++i) {
                QModelIndex &cellIndex = indexes[i];

                QStyleOptionHeader uniopt = styleOptionFromModel(styleOptions, cellIndex);

                if (i < indexes.size() - 1) {
                    if (uniopt.state.testFlag(QStyle::State_Sunken) || uniopt.state.testFlag(QStyle::State_On)) {
                        QStyle::State t(QStyle::State_Sunken | QStyle::State_On);
                        uniopt.state &= (~t);
                    }
                }

                QSize cell_size = cellSize(cellIndex, uniopt);

                QPoint top_left;
                if (orientation() == Qt::Horizontal) {
                    top_left.setX(currentCellLeft(cellIndex, childIndex, logicalIndex, rect.left()));
                    top_left.setY(pos);

                    if (cellIndex == childIndex) {
                        cell_size.setHeight(rect.height() - pos);
                    }
                    cell_size.setWidth(currentCellWidth(cellIndex, childIndex, logicalIndex));
                } else {
                    top_left.setX(pos);
                    top_left.setY(currentCellLeft(cellIndex, childIndex, logicalIndex, rect.top()));
                    if (cellIndex == childIndex) {
                        cell_size.setWidth(rect.width() - pos);
                    }
                    cell_size.setHeight(currentCellWidth(cellIndex, childIndex, logicalIndex));
                }

                QRect r(top_left, cell_size);
                uniopt.rect = r;

                painter->save();

                if (cellIndex.data(CustomRoles::Item_Rotated).isValid()) {
                    QTransform m;
                    m.rotate(-90);
                    painter->setWorldTransform(m, true);
                    QRect new_r(0, 0, r.height(), r.width());
                    new_r.moveCenter(QPoint(-r.center().y(), r.center().x()));
                    uniopt.rect = new_r;
                }
                style()->drawControl(QStyle::CE_Header, &uniopt, painter, this);

                painter->restore();
                pos += orientation() == Qt::Horizontal ? cell_size.height() : cell_size.width();
            }

            painter->setBrushOrigin(oldBO);
            return;
        }
    }
    QHeaderView::paintSection(painter, rect, logicalIndex);
}

QSize QAdvancedMultiLevelHeaderView::sectionSizeFromContents(int logicalIndex) const
{
    if (d->headerModel) {
        QModelIndex curLeafIndex(leafIndex(logicalIndex));
        if (curLeafIndex.isValid()) {
            QStyleOptionHeader styleOption(styleOptionForCell(logicalIndex));
            QStyleOptionHeader sectionOption = styleOptionFromModel(styleOption, curLeafIndex);
            QSize s = cellSize(curLeafIndex, sectionOption);
            curLeafIndex = curLeafIndex.parent();

            int depth = getDepth(curLeafIndex);

            while (curLeafIndex.isValid()) {
                if (depth >= d->minimumPaintepth) {
                    QStyleOptionHeader parentStyleOption = styleOptionFromModel(styleOption, curLeafIndex);

                    QSize parentSize = cellSize(curLeafIndex, parentStyleOption);

                    if (orientation() == Qt::Horizontal) {
                        s.rheight() += parentSize.height();
                    } else {
                        s.rwidth() += parentSize.width();
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
    QModelIndex childIndex(leafIndex(li));
    if (childIndex.isValid() && e->button() == Qt::LeftButton) {
        int depth = getDepthOfCurrentlyClickedItem(e->pos(), styleOptionForCell(li), childIndex, orientation()); // li=logical index
        current_active_item = qobject_cast<QAdvancedTableModel *>(model())->GetRootItem(orientation())->GetLeaf(li)->GetParentItemAtDepth(depth);
    }
}

void QAdvancedMultiLevelHeaderView::mouseReleaseEvent(QMouseEvent *e)
{
    QHeaderView::mouseReleaseEvent(e);
    int li = logicalIndexAt(e->pos());
    QModelIndex childIndex(leafIndex(li));
    if (childIndex.isValid() && e->button() == Qt::LeftButton) {
        int depth = getDepthOfCurrentlyClickedItem(e->pos(), styleOptionForCell(li), childIndex, orientation()); // li=logical index
        QAdvancedHeaderItem *clicked_item = qobject_cast<QAdvancedTableModel *>(model())->GetRootItem(orientation())->GetLeaf(li)->GetParentItemAtDepth(depth);
        if (current_active_item == clicked_item) {
            if (d->collapseHeaderSection) {
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

    QModelIndex childIndex(leafIndex(logicalIndex));
    if (childIndex.isValid()) {
        QModelIndexList leafsList(leafs(findRootIndex(childIndex)));
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
    d->collapseHeaderSection = value;
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
