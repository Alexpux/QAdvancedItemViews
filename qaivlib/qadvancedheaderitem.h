/*
Copyright (c) 2019, Grgo Mariani
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

#ifndef QADVANCEDHEADERITEM_H
#define QADVANCEDHEADERITEM_H

#include "qadvancedconstants.h"

#include <QPixmap>
#include <QStandardItem>
#include <QString>

/* *************************************************************************************
 * Usage:
 *  QAdvancedHeaderItem* node = QAdvancedHeaderItem::CreateRootItem("This is root", RotationType::VERTICAL);
 *  node->AppendNewItem("One");
 *  node->AppendNewItem("Two").AppendNewItem("Three");
 *  node->RemoveItem((*node)[0]);
 * It's also possible to set the maximum depth and rotations for all elements.
 * You only need to set the rotation for the first element, all the others are handled
 * automatically.
 * Placeholder/doppelganger is an item that serves to collapse the subtree on element
 * clicked.
 * *************************************************************************************/

class QAdvancedHeaderItem : public QStandardItem {
    struct TreeInfo;
    /* -----------------------VARIABLES-------------------------- */
public:
    QString _name;

private:
    bool _enableFilter { false };
    const int _depth { 0 };
    RotationType _rotation;
    QAdvancedHeaderItem *const _parent { nullptr };
    QAdvancedHeaderItem *_doppelganger { nullptr };
    TreeInfo *_treeinfo { nullptr };
    QPixmap _pixmap { QPixmap() };

    enum class ItemTypeEnum { NORMAL,
                              PLACEHOLDER } _type;

    /* ------------------------METHODS--------------------------- */
public:
    ~QAdvancedHeaderItem();
    static QAdvancedHeaderItem *CreateRootItem(const QString &name = QString(), RotationType rotation = RotationType::DEFAULT, int maxDepth = -1);
    static QAdvancedHeaderItem *CastFromQStandardItem(QStandardItem *);

    QAdvancedHeaderItem &AppendNewItem(const QString &newName, RotationType rotation = RotationType::DEFAULT);
    bool RemoveItem(QAdvancedHeaderItem *);

    int Size();

    QAdvancedHeaderItem *PlaceholderItemPut();
    QAdvancedHeaderItem *PlaceholderItemRemove();
    QAdvancedHeaderItem *PlaceholderToggle();

    QAdvancedHeaderItem *operator[](int num);
    QAdvancedHeaderItem *GetLeaf(int num);

    QAdvancedHeaderItem *GetParentItemAtDepth(int depth);

    bool IsPlaceholder();
    int GetNumberOfChildren(int wantedDepth = -2);

    QAdvancedHeaderItem &SetName(const QString &name);
    QAdvancedHeaderItem &SetPixmap(const QPixmap &pixmap);
    QAdvancedHeaderItem &SetRotation(RotationType rotation);
    QAdvancedHeaderItem &SetType(ItemTypeEnum type);
    QAdvancedHeaderItem &SetFilterButtonVisible(bool visible);

    QAdvancedHeaderItem *clone() const override;

private:
    explicit QAdvancedHeaderItem(const QString &name, RotationType rotation = RotationType::DEFAULT, QAdvancedHeaderItem *parent = nullptr, int depth = 0, ItemTypeEnum type = ItemTypeEnum::NORMAL);
    QAdvancedHeaderItem(const QAdvancedHeaderItem &other);

    QAdvancedHeaderItem &operator=(QAdvancedHeaderItem &other) = delete;
};

#endif // QADVANCEDHEADERITEM_H
