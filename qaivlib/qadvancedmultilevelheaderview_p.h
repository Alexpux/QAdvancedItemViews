#ifndef QADVANCEDMULTILEVELHEADERVIEWPRIVATE_H
#define QADVANCEDMULTILEVELHEADERVIEWPRIVATE_H

#include "qadvancedheaderitem.h"
#include "qadvancedmultilevelheaderview.h"

#include <QAbstractItemModel>
#include <QPainter>
#include <QStandardItemModel>

class QAdvancedMultiLevelHeaderViewPrivate {
public:
    // contains the model
    QAbstractItemModel *headerModel { nullptr };
    // Do we want to hide/show items when we click on their parents?
    bool GROUP_ITEMS_ON_CLICK { true };
    // standard headerview setting
    bool HIGHLIGHT_SECTIONS { true };
    // standard headerview setting
    bool SECTIONS_CLICKABLE { true };

    bool rootLevelVisible { false };
    int minimumPaintepth { 1 };

    struct SectionRange {
        int first;
        int last;

        int count() const { return last - first + 1; }
    };

    /* -----------------------------METHODS------------------------ */
    QAdvancedMultiLevelHeaderViewPrivate() = default;

    ~QAdvancedMultiLevelHeaderViewPrivate()
    {
        ClearModel();
    }

    QAdvancedMultiLevelHeaderViewPrivate *ClearModel()
    {
        delete headerModel;
        headerModel = nullptr;
        return this;
    }

    QAdvancedMultiLevelHeaderViewPrivate *initFromNewModel(Qt::Orientation orientation, QAbstractItemModel *model)
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
            QVariant v(model->data(QModelIndex(), hf));
            if (v.isValid()) {
                headerModel = qobject_cast<QAbstractItemModel *>(v.value<QObject *>());
            }
        }
        return this;
    }

    /**
     * @brief logicalIndexFor
     * @param index
     * @return Logical index from model index
     */
    [[nodiscard]] int logicalIndexFor(const QModelIndex &index) const
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

    /**
     * @brief minimumSpanWidth Calculate minimum width needed to span all children
     * @param cellIndex
     * @param hv
     * @return
     */
    [[nodiscard]] int minimumSpanWidth(const QModelIndex &cellIndex,
                                       const QAdvancedMultiLevelHeaderView *hv) const
    {
        if (!cellIndex.isValid() || !hv) {
            return 0;
        }

        QModelIndexList childLeaves = searchLeafs(cellIndex);

        if (childLeaves.isEmpty()) {
            return 0;
        }

        int totalWidth = 0;

        // Sum up the current widths of all child columns
        for (const QModelIndex &leafIdx : childLeaves) {
            QVariant orderData = leafIdx.data(CustomRoles::Item_Order_Identify);
            if (orderData.isValid()) {
                int logicalIdx = orderData.toInt();
                totalWidth += hv->sectionSize(logicalIdx);
            }
        }

        return totalWidth;
    }

    [[nodiscard]] QSize enhancedCellSize(const QModelIndex &cellIndex,
                                         const QAdvancedMultiLevelHeaderView *hv,
                                         QStyleOptionHeader &styleOptions)
    {
        QSize sz = cellSize(cellIndex, hv, styleOptions);

        // Check if this cell has children (is a parant)
        bool hasChildren = cellIndex.model() && cellIndex.model()->rowCount(cellIndex) > 0;

        if (hasChildren) {
            int minWidth = minimumSpanWidth(cellIndex, hv);
            sz.setWidth(std::max(sz.width(), minWidth));
        }

        return sz;
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

    [[nodiscard]] int getDepth(const QModelIndex &index) const
    {
        int depth = 0;
        QModelIndex current = index;
        while (current.parent().isValid()) {
            depth++;
            current = current.parent();
        }
        return depth;
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

    [[nodiscard]] SectionRange getChildSectionRange(QAdvancedHeaderItem *item) const
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

    /* Look and feel */
    void fillStyleOptionsFromModel(QPainter *painter,
                                   const QAdvancedMultiLevelHeaderView *hv,
                                   QStyleOptionHeader &opt,
                                   const QModelIndex &index) const
    {
        QVariant fgBrush = index.data(Qt::ForegroundRole);
        if (fgBrush.canConvert(QMetaType::QBrush)) {
            opt.palette.setBrush(QPalette::ButtonText, qvariant_cast<QBrush>(fgBrush));
            opt.palette.setBrush(QPalette::WindowText, qvariant_cast<QBrush>(fgBrush));
        }

        QVariant bgBrush = index.data(Qt::BackgroundRole);
        if (bgBrush.canConvert(QMetaType::QBrush)) {
            opt.palette.setBrush(QPalette::Button, qvariant_cast<QBrush>(bgBrush));
            opt.palette.setBrush(QPalette::Window, qvariant_cast<QBrush>(bgBrush));
            painter->setBrushOrigin(opt.rect.topLeft());
        }

        QVariant bgIcon = index.data(Qt::DecorationRole);
        if (bgIcon.canConvert(QMetaType::QIcon)) {
            opt.icon = qvariant_cast<QIcon>(bgIcon);
        } else if (bgIcon.canConvert(QMetaType::QPixmap)) {
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
        if (varFont.isValid() && varFont.canConvert(QMetaType::QFont)) {
            fnt = qvariant_cast<QFont>(varFont);
        }
        fnt.setBold(true);
        QFontMetrics fm(fnt);
        opt.fontMetrics = std::move(fm);

        // opt.text = index.data(Qt::DisplayRole).toString();
    }

    QSize cellSize(const QModelIndex &leafIdx,
                   const QAdvancedMultiLevelHeaderView *hv,
                   QStyleOptionHeader &styleOptions) const
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

    int currentCellWidth(const QModelIndex &searchedIndex,
                         const QModelIndex &leafIndex,
                         int sectionIndex,
                         const QAdvancedMultiLevelHeaderView *hv) const
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

    int currentCellLeft(const QModelIndex &searchedIndex,
                        const QModelIndex &leafIndex,
                        int sectionIndex,
                        int left,
                        const QAdvancedMultiLevelHeaderView *hv) const
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
                                       const QPoint point,
                                       const QStyleOptionHeader &styleOptions,
                                       const QModelIndex &leafIndex,
                                       Qt::Orientation orientation) const
    {
        int xy = (orientation == Qt::Horizontal) ? point.y() : point.x(); // XY - position; WH - width, height
        QModelIndexList indexes(parentIndexes(leafIndex));
        int result = 0;
        int startIndex = std::max(0, minimumPaintepth);
        for (int i = startIndex; i < indexes.size(); ++i) {
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

    int paintHorizontalCell(QPainter *painter,
                            const QAdvancedMultiLevelHeaderView *hv,
                            const QModelIndex &cellIndex,
                            const QModelIndex &leafIndex,
                            int logicalLeafIndex,
                            const QStyleOptionHeader &styleOptions,
                            const QRect sectionRect, int top) const
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

    void paintHorizontalSection(QPainter *painter,
                                const QRect sectionRect,
                                int logicalLeafIndex,
                                const QAdvancedMultiLevelHeaderView *hv,
                                const QStyleOptionHeader &styleOptions,
                                const QModelIndex &leafIndex) const
    {
        QPointF oldBO(painter->brushOrigin());
        int top = sectionRect.y();
        QModelIndexList indexes(parentIndexes(leafIndex));

        int startIndex = std::max(0, minimumPaintepth);

        for (int i = startIndex; i < indexes.size(); ++i) {
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

    int paintVerticalCell(QPainter *painter,
                          const QAdvancedMultiLevelHeaderView *hv,
                          const QModelIndex &cellIndex,
                          const QModelIndex &leafIndex,
                          int logicalLeafIndex,
                          const QStyleOptionHeader &styleOptions,
                          const QRect sectionRect,
                          int left) const
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

    void paintVerticalSection(QPainter *painter,
                              const QRect sectionRect,
                              int logicalLeafIndex,
                              const QAdvancedMultiLevelHeaderView *hv,
                              const QStyleOptionHeader &styleOptions,
                              const QModelIndex &leafIndex) const
    {
        QPointF oldBO(painter->brushOrigin());
        int left = sectionRect.x();
        QModelIndexList indexes(parentIndexes(leafIndex));

        int startIndex = std::max(0, minimumPaintepth);

        for (int i = startIndex; i < indexes.size(); ++i) {
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

#endif // QADVANCEDMULTILEVELHEADERVIEWPRIVATE_H
