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

#ifndef QADVANCEDMULTILEVELHEADERVIEW_H
#define QADVANCEDMULTILEVELHEADERVIEW_H

#include "qadvancedheaderitem.h"
#include "qadvancedheaderview.h"

class QAdvancedMultiLevelHeaderViewPrivate;

class QAIVLIBSHARED_EXPORT QAdvancedMultiLevelHeaderView : public QAdvancedHeaderView {
    Q_OBJECT

public:
    friend QAdvancedMultiLevelHeaderViewPrivate;
    explicit QAdvancedMultiLevelHeaderView(Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = nullptr);
    ~QAdvancedMultiLevelHeaderView() override;

    void setModel(QAbstractItemModel *model) override;
    void setGroupItemsByClick(bool value);

    void setRootLevelVisible(bool visible);
    [[nodiscard]] bool isRootLevelVisible() const noexcept;

    /**
     * @brief Set minimum depth to paint (0 = root, 1 = first children, etc.)
     * @param depth Minimum depth level to start painting from
     *
     * This is more flexible than setRootLevelVisible()
     *  - depth = 0: Paint everything
     *  - depth = 1: Skip root level (default)
     *  - depth = 2: Skip root and first level
     */
    void setMinimumPaintDepth(int depth);
    [[nodiscard]] int minimumPaintDepth() const noexcept;

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
    QSize sectionSizeFromContents(int logicalIndex) const override;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    QAdvancedMultiLevelHeaderViewPrivate *d { nullptr };

    // Useful while registering clicks
    static QAdvancedHeaderItem *current_active_item;

    QStyleOptionHeader styleOptionForCell(int logicalIdx) const;

private slots:
    void on_sectionResized(int logicalIndex, int oldSize, int newSize);
};

#endif // QADVANCEDMULTILEVELHEADERVIEW_H
