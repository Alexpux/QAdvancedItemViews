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
#include "qadvancedheaderstyle.h"
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
 * struct DATA
 * *****************************************************************************************************/
struct QAdvancedMultiLevelHeaderView::DATA {
    /* ----------------------------VARIABLES----------------------- */
    // contains the model
    QStandardItemModel *headerModel { nullptr };
    // Do we want to hide/show items when we click on their parents?
    bool GROUP_ITEMS_ON_CLICK { true };
    // standard headerview setting
    bool HIGHLIGHT_SECTIONS { true };
    // standard headerview setting
    bool SECTIONS_CLICKABLE { true };

    /* -----------------------------METHODS------------------------ */
    DATA() = default;

    ~DATA()
    {
        ClearModel();
    }

    DATA *ClearModel()
    {
        delete headerModel;
        headerModel = nullptr;
        return this;
    }

    DATA *initFromNewModel(Qt::Orientation orientation, QStandardItemModel *model)
    {
        ClearModel();
        CustomRoles hf;
        switch (orientation) {
        case Qt::Orientation::Vertical: {
            hf = CustomRoles::MultiLevelHeaderView_Rows;
            break;
        }
        case Qt::Orientation::Horizontal: {
            hf = CustomRoles::MultiLevelHeaderView_Columns;
            break;
        }
        }
        headerModel = new QStandardItemModel();
        if (model) {
            QVariant v(model->headerData(0, Qt::Horizontal, hf));
            if (v.isValid()) {
                headerModel = qobject_cast<QStandardItemModel *>(v.value<QObject *>());
            }
        }
        return this;
    }

    /* Find the root-iest model */
    QModelIndex findRootIndex(const QModelIndex &index) const
    {
        return index.parent().isValid() ? findRootIndex(index.parent()) : index;
    }

    /* return all parents of elem (ordered root to elem) */
    QModelIndexList parentIndexes(const QModelIndex &index) const
    {
        QModelIndexList indexes;
        indexes.prepend(index);
        while (indexes.first().isValid()) {
            indexes.prepend(indexes.first().parent());
        }
        indexes.removeFirst();
        return indexes;
    }

    /* Return leaf on position target */
    QModelIndex findLeaf(const QModelIndex &currIndex, const int target, int &currentLeafIndex) const
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

    /* find leaf on index sectionindex */
    QModelIndex leafIndex(const int sectionIndex) const
    {
        if (headerModel) {
            int currentLeafIndex = -1;
            for (int i = 0; i < headerModel->rowCount(); i++) {
                QModelIndex result = findLeaf(headerModel->index(i, 0), sectionIndex, currentLeafIndex);
                if (result.isValid()) {
                    return result;
                }
            }
        }
        return QModelIndex();
    }

    /* Get all leaves of this index */
    QModelIndexList searchLeafs(const QModelIndex &currentIndex) const
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

    /* Same as above but groups finished items differently */
    QModelIndexList leafs(const QModelIndex &searchedIndex) const
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

    /* Look and feel */
    void fillStyleOptionsFromModel(QPainter *painter, const QAdvancedMultiLevelHeaderView *hv, QStyleOptionHeader &opt, const QModelIndex &index) const
    {
        QVariant fgBrush = index.data(Qt::ForegroundRole);
        if (fgBrush.canConvert(QMetaType(QMetaType::QBrush))) {
            opt.palette.setBrush(QPalette::ButtonText, qvariant_cast<QBrush>(fgBrush));
            opt.palette.setBrush(QPalette::WindowText, qvariant_cast<QBrush>(fgBrush));
        }

        QVariant bgBrush = index.data(Qt::BackgroundRole);
        if (bgBrush.canConvert(QMetaType(QMetaType::QBrush))) {
            opt.palette.setBrush(QPalette::Button, qvariant_cast<QBrush>(bgBrush));
            opt.palette.setBrush(QPalette::Window, qvariant_cast<QBrush>(bgBrush));
            painter->setBrushOrigin(opt.rect.topLeft());
        }

        QVariant bgIcon = index.data(Qt::DecorationRole);
        if (bgIcon.canConvert(QMetaType(QMetaType::QIcon))) {
            opt.icon = qvariant_cast<QIcon>(bgIcon);
        } else if (bgIcon.canConvert(QMetaType(QMetaType::QPixmap))) {
            opt.icon = qvariant_cast<QPixmap>(bgIcon);
        }
        opt.iconAlignment = Qt::AlignCenter;

        /*QModelIndexList leafsList(leafs(index));
        if (leafsList.size() > 0) {
            opt.sortIndicator = QStyleOptionHeader::None;
        }

        QVariant varFilter = index.data(Item_Filtering);
        if (varFilter.isValid()) {
            //opt.filterIcon = QIcon(HeaderFilterIcon);
            opt.filteringItem = true;
        }*/

        QFont fnt(hv->font());
        QVariant varFont(index.data(Qt::FontRole));
        if (varFont.isValid() && varFont.canConvert(QMetaType(QMetaType::QFont))) {
            fnt = qvariant_cast<QFont>(varFont);
        }
        fnt.setBold(true);
        QFontMetrics fm(fnt);
        opt.fontMetrics = std::move(fm);

        // opt.text = index.data(Qt::DisplayRole).toString();
    }

    QSize cellSize(const QModelIndex &leafIdx, const QAdvancedMultiLevelHeaderView *hv, QStyleOptionHeader &styleOptions) const
    {
        QSize res;
        QVariant variant(leafIdx.data(Qt::SizeHintRole));
        if (variant.isValid()) {
            res = qvariant_cast<QSize>(variant);
        }

        int margin = hv->style()->pixelMetric(QStyle::PM_HeaderMargin, nullptr, hv);

        QSize indicatorSize(0, 0);
        int section = hv->sortIndicatorSection();
        QModelIndex idx = leafIndex(section);
        if (hv->isSortIndicatorShown() && idx == leafIdx) {
            styleOptions.sortIndicator = (hv->sortIndicatorOrder() == Qt::AscendingOrder) ? QStyleOptionHeader::SortDown : QStyleOptionHeader::SortUp;
            indicatorSize = HeaderSortMarkSize + QSize(margin, 0);
        }

        /*QModelIndexList leafsList = leafs(leafIdx);
        if (leafsList.size() > 0) {
            //styleOptions.sortIndicator = QStyleOptionHeader::None;
        } else {
        }*/

        // fillStyleOptionsFromModel(hv, styleOptions, leafIndex);

        QSize decorationsSize = hv->style()->sizeFromContents(QStyle::CT_HeaderSection, &styleOptions, QSize(), hv);

        QSize emptyTextSize = styleOptions.fontMetrics.size(0, "");
        QString txt = leafIdx.data(Qt::DisplayRole).toString();
        QSize textSize = styleOptions.fontMetrics.size(0, txt);
        if (!txt.isNull()) {
            textSize.rwidth() += margin;
        }
        if (leafIdx.data(CustomRoles::Item_Rotated).isValid()) {
            textSize.transpose();
            emptyTextSize.transpose();
        }

        res = res.expandedTo(textSize + decorationsSize + indicatorSize /*+ iconElemSize + QSize(0, iconFilterSize.height())*/ - emptyTextSize);
        return res;
    }

    int currentCellWidth(const QModelIndex &searchedIndex, const QModelIndex &leafIndex, int sectionIndex, const QAdvancedMultiLevelHeaderView *hv) const
    {
        QModelIndexList leafsList(leafs(searchedIndex));
        if (leafsList.empty()) {
            return hv->sectionSize(sectionIndex);
        }
        int width = 0;
        int firstLeafSectionIndex = sectionIndex - leafsList.indexOf(leafIndex);
        for (int i = 0; i < leafsList.size(); ++i) {
            width += hv->sectionSize(firstLeafSectionIndex + i);
        }
        return width;
    }

    int currentCellLeft(const QModelIndex &searchedIndex, const QModelIndex &leafIndex, int sectionIndex, int left, const QAdvancedMultiLevelHeaderView *hv) const
    {
        QModelIndexList leafsList(leafs(searchedIndex));
        if (!leafsList.empty()) {
            int n = leafsList.indexOf(leafIndex);
            int firstLeafSectionIndex = sectionIndex - n;
            --n;
            for (; n >= 0; --n) {
                left -= hv->sectionSize(firstLeafSectionIndex + n);
            }
        }
        return left;
    }

    /* Returns how deep the parent clicked is */
    int getDepthOfCurrentlyClickedItem(const QAdvancedMultiLevelHeaderView *hv,
                                       const QPoint &point,
                                       const QStyleOptionHeader &styleOptions,
                                       const QModelIndex &leafIndex,
                                       Qt::Orientation orientation) const
    {
        int xy = (orientation == Qt::Horizontal) ? point.y() : point.x(); // XY - position; WH - width, height
        QModelIndexList indexes(parentIndexes(leafIndex));
        int result = 0;
        for (int i = 0; i < indexes.size(); ++i) {
            QStyleOptionHeader uniopt(styleOptions);
            QSize box = cellSize(indexes[i], hv, uniopt);
            int wh = (orientation == Qt::Horizontal) ? box.height() : box.width();
            result += wh;
            if (xy <= result) {
                return i;
            }
        }
        return indexes.size() - 1;
    }

    int paintHorizontalCell(QPainter *painter, const QAdvancedMultiLevelHeaderView *hv, const QModelIndex &cellIndex, const QModelIndex &leafIndex,
                            int logicalLeafIndex, const QStyleOptionHeader &styleOptions, const QRect &sectionRect, int top) const
    {
        QStyleOptionHeader uniopt(styleOptions);
        fillStyleOptionsFromModel(painter, hv, uniopt, cellIndex);

        QSize cellSz = cellSize(cellIndex, hv, uniopt);

        int height = cellSz.height();
        if (cellIndex == leafIndex) {
            height = sectionRect.height() - top;
        }
        int left = currentCellLeft(cellIndex, leafIndex, logicalLeafIndex, sectionRect.left(), hv);
        int width = currentCellWidth(cellIndex, leafIndex, logicalLeafIndex, hv);
        QRect r(left, top, width, height);
        uniopt.text = cellIndex.data(Qt::DisplayRole).toString();

        painter->save();
        uniopt.rect = r;

        if (cellIndex.data(CustomRoles::Item_Rotated).isValid()) {
            hv->style()->drawControl(QStyle::CE_HeaderSection, &uniopt, painter, hv);
            QTransform m;
            m.rotate(-90);
            painter->setWorldTransform(m, true);
            QRect new_r(0, 0, r.height(), r.width());
            new_r.moveCenter(QPoint(-r.center().y(), r.center().x()));
            uniopt.rect = new_r;
            hv->style()->drawControl(QStyle::CE_HeaderLabel, &uniopt, painter, hv);
        } else {
            hv->style()->drawControl(/*(QStyle::ControlElement)(QAdvancedHeaderStyle::CE_HeaderLook)*/ QStyle::CE_Header, &uniopt, painter, hv);
        }

        painter->restore();
        return top + height;
    }

    void paintHorizontalSection(QPainter *painter, const QRect &sectionRect, int logicalLeafIndex, const QAdvancedMultiLevelHeaderView *hv,
                                const QStyleOptionHeader &styleOptions, const QModelIndex &leafIndex) const
    {
        QPointF oldBO(painter->brushOrigin());
        int top = sectionRect.y();
        QModelIndexList indexes(parentIndexes(leafIndex));
        for (int i = 0; i < indexes.size(); ++i) {
            // main logic here is actually taking all the parents of the element and disabling the styles for them
            QStyleOptionHeader realStyleOptions(styleOptions);
            if (i < indexes.size() - 1) {
                if (realStyleOptions.state.testFlag(QStyle::State_Sunken) || realStyleOptions.state.testFlag(QStyle::State_On)) {
                    QStyle::State t(QStyle::State_Sunken | QStyle::State_On);
                    realStyleOptions.state &= (~t);
                }
            }
            top = paintHorizontalCell(painter, hv, indexes[i], leafIndex, logicalLeafIndex, realStyleOptions, sectionRect, top);
        }
        painter->setBrushOrigin(oldBO);
    }

    int paintVerticalCell(QPainter *painter, const QAdvancedMultiLevelHeaderView *hv, const QModelIndex &cellIndex, const QModelIndex &leafIndex,
                          int logicalLeafIndex, const QStyleOptionHeader &styleOptions, const QRect &sectionRect, int left) const
    {
        QStyleOptionHeader uniopt(styleOptions);
        fillStyleOptionsFromModel(painter, hv, uniopt, cellIndex);

        int width = cellSize(cellIndex, hv, uniopt).width();
        if (cellIndex == leafIndex) {
            width = sectionRect.width() - left;
        }
        int top = currentCellLeft(cellIndex, leafIndex, logicalLeafIndex, sectionRect.top(), hv);
        int height = currentCellWidth(cellIndex, leafIndex, logicalLeafIndex, hv);
        QRect r(left, top, width, height);
        uniopt.text = cellIndex.data(Qt::DisplayRole).toString();

        painter->save();
        uniopt.rect = r;
        if (cellIndex.data(CustomRoles::Item_Rotated).isValid()) {
            hv->style()->drawControl(QStyle::CE_HeaderSection, &uniopt, painter, hv);
            QTransform m;
            m.rotate(-90);
            painter->setWorldTransform(m, true);
            QRect new_r(0, 0, r.height(), r.width());
            new_r.moveCenter(QPoint(-r.center().y(), r.center().x()));
            uniopt.rect = new_r;
            hv->style()->drawControl(QStyle::CE_HeaderLabel, &uniopt, painter, hv);
        } else {
            hv->style()->drawControl(QStyle::CE_Header, &uniopt, painter, hv);
        }
        painter->restore();
        return left + width;
    }

    void paintVerticalSection(QPainter *painter, const QRect &sectionRect, int logicalLeafIndex, const QAdvancedMultiLevelHeaderView *hv,
                              const QStyleOptionHeader &styleOptions, const QModelIndex &leafIndex) const
    {
        QPointF oldBO(painter->brushOrigin());
        int left = sectionRect.x();
        QModelIndexList indexes(parentIndexes(leafIndex));
        for (int i = 0; i < indexes.size(); ++i) {
            QStyleOptionHeader realStyleOptions(styleOptions);
            if (i < indexes.size() - 1 && (realStyleOptions.state.testFlag(QStyle::State_Sunken) || realStyleOptions.state.testFlag(QStyle::State_On))) {
                QStyle::State t(QStyle::State_Sunken | QStyle::State_On);
                realStyleOptions.state &= (~t);
            }
            left = paintVerticalCell(painter, hv, indexes[i], leafIndex, logicalLeafIndex, realStyleOptions, sectionRect, left);
        }
        painter->setBrushOrigin(oldBO);
    }
};

/* *****************************************************************************************************
 * class QAdvancedMultiLevelHeaderView
 * *****************************************************************************************************/

QAdvancedMultiLevelHeaderView::QAdvancedMultiLevelHeaderView(Qt::Orientation orientation, QWidget *parent) :
    QAdvancedHeaderView(orientation, parent), _data(new DATA())
{
    setStyle(new QAdvancedHeaderStyle());
    setHighlightSections(_data->HIGHLIGHT_SECTIONS);
    setSectionsClickable(_data->SECTIONS_CLICKABLE);
    setIconSize(HeaderIconSize);

    connect(this, &QAdvancedMultiLevelHeaderView::sectionResized, this, &QAdvancedMultiLevelHeaderView::on_sectionResized);
}

QAdvancedMultiLevelHeaderView::~QAdvancedMultiLevelHeaderView()
{
    delete _data;
}

void QAdvancedMultiLevelHeaderView::setModel(QAbstractItemModel *model)
{
    QHeaderView::setModel(model);
    _data->initFromNewModel(orientation(), qobject_cast<QStandardItemModel *>(model));
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
        QModelIndex childIndex(_data->leafIndex(logicalIndex));
        if (childIndex.isValid()) {
            if (orientation() == Qt::Horizontal) {
                _data->paintHorizontalSection(painter, rect, logicalIndex, this, styleOptionForCell(logicalIndex), childIndex);
            } else {
                _data->paintVerticalSection(painter, rect, logicalIndex, this, styleOptionForCell(logicalIndex), childIndex);
            }
            return;
        }
    }
    QHeaderView::paintSection(painter, rect, logicalIndex);
}

QSize QAdvancedMultiLevelHeaderView::sectionSizeFromContents(int logicalIndex) const
{
    if (_data->headerModel) {
        QModelIndex curLeafIndex(_data->leafIndex(logicalIndex));
        if (curLeafIndex.isValid()) {
            QStyleOptionHeader styleOption(styleOptionForCell(logicalIndex));
            QSize s = _data->cellSize(curLeafIndex, this, styleOption);
            curLeafIndex = curLeafIndex.parent();
            while (curLeafIndex.isValid()) {
                if (orientation() == Qt::Horizontal) {
                    s.rheight() += _data->cellSize(curLeafIndex, this, styleOption).height();
                } else {
                    s.rwidth() += _data->cellSize(curLeafIndex, this, styleOption).width();
                }
                curLeafIndex = curLeafIndex.parent();
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
    QModelIndex childIndex(_data->leafIndex(li));
    if (childIndex.isValid() && e->button() == Qt::LeftButton) {
        int d = _data->getDepthOfCurrentlyClickedItem(this, e->pos(), styleOptionForCell(li), childIndex, orientation()); // li=logical index
        current_active_item = qobject_cast<QAdvancedTableModel *>(model())->GetRootItem(orientation())->GetLeaf(li)->GetParentItemAtDepth(d);
    }
}

void QAdvancedMultiLevelHeaderView::mouseReleaseEvent(QMouseEvent *e)
{
    QHeaderView::mouseReleaseEvent(e);
    int li = logicalIndexAt(e->pos());
    QModelIndex childIndex(_data->leafIndex(li));
    if (childIndex.isValid() && e->button() == Qt::LeftButton) {
        int d = _data->getDepthOfCurrentlyClickedItem(this, e->pos(), styleOptionForCell(li), childIndex, orientation()); // li=logical index
        QAdvancedHeaderItem *clicked_item = qobject_cast<QAdvancedTableModel *>(model())->GetRootItem(orientation())->GetLeaf(li)->GetParentItemAtDepth(d);
        if (current_active_item == clicked_item) {
            if (_data->GROUP_ITEMS_ON_CLICK) {
                clicked_item->PlaceholderToggle(); // <--- 'clicked_item' was clicked in the header
                qobject_cast<QAdvancedTableModel *>(model())->emit_layoutChanged();
            }
        }
    }
}

void QAdvancedMultiLevelHeaderView::on_sectionResized(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(oldSize)
    Q_UNUSED(newSize)
    if (isSectionHidden(logicalIndex)) {
        return;
    }

    QModelIndex childIndex(_data->leafIndex(logicalIndex));
    if (childIndex.isValid()) {
        QModelIndexList leafsList(_data->leafs(_data->findRootIndex(childIndex)));
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
    _data->GROUP_ITEMS_ON_CLICK = value;
}
