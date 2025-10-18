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

#include "qadvancedheaderitem.h"

/* ***************************************************************************************
 * Some info:
 *  QAdvancedHeaderItem class could be best described as a node in a tree like structure.
 *  There can be only one root element and all the other elements must be created from it.
 *  We can collapse some items by replacing them with a temporary placeholder/doppelganger.
 *  operator[] is overridden so we can get a child at required position.
 *  All the items share a pointer to their shared main tree information.
 * ***************************************************************************************/

struct QAdvancedHeaderItem::TreeInfo {
    QAdvancedHeaderItem *const root_node { nullptr };
    RotationType _rotation { RotationType::DEFAULT };
    int _maxDepth { 0 };

    TreeInfo(QAdvancedHeaderItem *htvitem, RotationType rotation, int maxDepth) :
        root_node(htvitem), _rotation(rotation), _maxDepth(maxDepth)
    {
        _rotation = (rotation == RotationType::DEFAULT ? RotationType::HORIZONTAL : rotation);
    }
};

QAdvancedHeaderItem::QAdvancedHeaderItem(const QString &name, RotationType rotation, QAdvancedHeaderItem *parent, int depth, ItemTypeEnum type) :
    QStandardItem(name),
    _name(name),
    _depth(depth),
    _rotation(rotation),
    _parent(parent),
    _type(type)
{
    if (depth > 0) {
        // Inherit the info from the parent
        _treeinfo = _parent->_treeinfo;
    }
    this->setToolTip(_name);

    // SetFilterButtonVisible(true);
    SetPixmap(QPixmap());
    SetRotation(_rotation);
}

QAdvancedHeaderItem::QAdvancedHeaderItem(const QAdvancedHeaderItem &other) :
    QStandardItem(other),
    _name(other._name),
    _enableFilter(other._enableFilter),
    _depth(other._depth),
    _rotation(other._rotation),
    _parent(other._parent),
    _treeinfo(other._treeinfo),
    _pixmap(other._pixmap),
    _type(other._type)
{
    this->setToolTip(_name);
    SetRotation(other._rotation);
}

QAdvancedHeaderItem::~QAdvancedHeaderItem()
{
    for (int i = Size(); i >= 0; --i) {
        removeRow(0);
    }

    // if root node -> delete tree data
    if (_depth == 0) {
        delete _treeinfo;
    }
}

QAdvancedHeaderItem *QAdvancedHeaderItem::CreateRootItem(const QString &name, RotationType rotation, int maxDepth)
{
    if (rotation == RotationType::DEFAULT) {
        rotation = RotationType::HORIZONTAL;
    }
    QAdvancedHeaderItem *newItem = new QAdvancedHeaderItem(name, rotation, nullptr, 0);

    // This is our only constructor for the complete data structure
    newItem->_treeinfo = new QAdvancedHeaderItem::TreeInfo(newItem, rotation, maxDepth);
    return newItem;
}

QAdvancedHeaderItem *QAdvancedHeaderItem::CastFromQStandardItem(QStandardItem *qsitem)
{
    return dynamic_cast<QAdvancedHeaderItem *>(qsitem);
}

QAdvancedHeaderItem &QAdvancedHeaderItem::AppendNewItem(const QString &newName, RotationType rotation)
{
    // Make sure we don't cross our maximum depth
    if (_treeinfo->_maxDepth > -1 && _treeinfo->_maxDepth < (_depth + 1)) {
        return *this;
    }
    if (rotation == RotationType::DEFAULT) {
        rotation = (_treeinfo->_rotation == RotationType::HORIZONTAL) ? RotationType::HORIZONTAL : RotationType::VERTICAL;
    }
    SetRotation(_treeinfo->_rotation);

    // Create Item -> ____
    QAdvancedHeaderItem *newItem = new QAdvancedHeaderItem(newName, rotation, this, _depth + 1);
    // ____ -> Append it as a child
    this->appendRow(CastFromQStandardItem(newItem));
    return *newItem;
}

bool QAdvancedHeaderItem::RemoveItem(QAdvancedHeaderItem *child)
{
    // In case child is a placeholder
    child = child->PlaceholderItemRemove();
    if (_depth >= child->_depth) {
        return false;
    }
    int r = child->_depth;
    QAdvancedHeaderItem *phtv = child;
    while (r != _depth) {
        phtv = phtv->_parent;
        r--;
    }
    if (phtv != this) {
        return false;
    }
    if (child->_parent->Size() == 1 && child->_parent != _treeinfo->root_node) {
        child->_parent->SetRotation(_rotation == RotationType::HORIZONTAL ? RotationType::VERTICAL : RotationType::HORIZONTAL);
    }
    child->_parent->removeRow(child->row());
    return true;
}

int QAdvancedHeaderItem::Size()
{
    return this->rowCount();
}

QAdvancedHeaderItem *QAdvancedHeaderItem::PlaceholderItemPut()
{
    // This function can only be used on children items
    // which don't have placeholders currently active
    if (_depth == 0 && row() > -1) {
        return this;
    }

    // As well as the prerequisite that
    // They must have some children
    if (!hasChildren()) {
        return this;
    }
    int trow = row();

    // Usually returns a pointer to this
    _parent->takeRow(trow);
    RotationType newRotation = (_treeinfo->_rotation == RotationType::HORIZONTAL) ? RotationType::VERTICAL : RotationType::HORIZONTAL;
    /*auto* newItem = new QAdvancedHeaderItem(QString("*") + _name, newRotation, _parent,
                                _depth, ItemTypeEnum::PLACEHOLDER);         //           Create Item -> ____*/

    // Create Item -> ____*/
    QAdvancedHeaderItem *newItem = this->clone();
    newItem->SetName(QStringLiteral("*") + _name);
    newItem->SetType(ItemTypeEnum::PLACEHOLDER);

    // ____ -> Set Rotation -> ____
    newItem->SetRotation(newRotation);
    //  ____ -> Append it as a child to parent
    _parent->insertRow(trow, CastFromQStandardItem(newItem));
    // Link these two items
    newItem->_doppelganger = this;
    _doppelganger = newItem;
    return this;
}

QAdvancedHeaderItem *QAdvancedHeaderItem::PlaceholderItemRemove()
{
    QAdvancedHeaderItem *p = IsPlaceholder() ? _doppelganger : this;
    // We want the option
    // of calling this function directly from
    // the doppelganger
    if (_doppelganger == nullptr) {
        return this;
    }
    int trow = p->_doppelganger->row();
    _parent->takeRow(trow);
    delete p->_doppelganger;
    p->_doppelganger = nullptr;
    _parent->insertRow(trow, p);
    return p;
}

QAdvancedHeaderItem *QAdvancedHeaderItem::PlaceholderToggle()
{
    QAdvancedHeaderItem *res;
    if (IsPlaceholder()) {
        res = PlaceholderItemRemove();
    } else {
        res = PlaceholderItemPut();
    }
    return res;
}

QAdvancedHeaderItem *QAdvancedHeaderItem::operator[](int num)
{
    if (num < 0 || num >= Size()) {
        return this;
    }
    return QAdvancedHeaderItem::CastFromQStandardItem(this->child(num));
}

QAdvancedHeaderItem *QAdvancedHeaderItem::GetLeaf(int num)
{
    if (num < 0) {
        return this;
    }
    if (Size() == 0) {
        return this;
    }
    for (int sum = 0, trow = 0; sum <= num; trow++) {
        QAdvancedHeaderItem *temph = (*this)[trow];
        int res = temph->GetNumberOfChildren();
        if (num < sum + res) {
            return (*temph).GetLeaf(num - sum);
        }
        sum += res;
    }
    return this;
}

QAdvancedHeaderItem *QAdvancedHeaderItem::GetParentItemAtDepth(int depth)
{
    if (depth < 0) {
        return this->_treeinfo->root_node;
    }
    QAdvancedHeaderItem *p = this;
    while (depth < p->_depth) {
        p = p->_parent;
    }
    return p;
}

bool QAdvancedHeaderItem::IsPlaceholder()
{
    return _type == ItemTypeEnum::PLACEHOLDER;
}

int QAdvancedHeaderItem::GetNumberOfChildren(int wantedDepth)
{
    if (_depth == wantedDepth) {
        return 1;
    }
    if (!hasChildren()) {
        // special case in case we only
        // wish to count at specific depth
        if (wantedDepth != -2) {
            return 0;
        }
        return 1;
    }
    int result = 0;
    for (int i = 0; i < Size(); i++) {
        QAdvancedHeaderItem *item = CastFromQStandardItem(child(i));
        if (item) {
            result += item->GetNumberOfChildren();
        }
    }
    return result;
}

QAdvancedHeaderItem &QAdvancedHeaderItem::SetFilterButtonVisible(bool visible)
{
    _enableFilter = visible;
    if (_enableFilter) {
        this->setData(QVariant(_enableFilter), CustomRoles::Item_Filtering);
    } else {
        this->setData(QVariant(), CustomRoles::Item_Filtering);
    }
    return *this;
}

QAdvancedHeaderItem &QAdvancedHeaderItem::SetName(const QString &name)
{
    QStandardItem::setText(name);
    _name = name;
    this->setToolTip(_name);
    return *this;
}

QAdvancedHeaderItem &QAdvancedHeaderItem::SetPixmap(const QPixmap &pixmap)
{
    _pixmap = pixmap;
    if (!pixmap.isNull()) {
        this->setData(QVariant(pixmap), Qt::DecorationRole);
    } else {
        this->setData(QVariant(), Qt::DecorationRole);
    }
    return *this;
}

QAdvancedHeaderItem &QAdvancedHeaderItem::SetRotation(RotationType rotation)
{
    if (rotation == RotationType::VERTICAL) {
        this->setData(1, CustomRoles::Item_Rotated);
    } else {
        this->setData(QVariant(), CustomRoles::Item_Rotated);
    }
    _rotation = rotation;
    return *this;
}

QAdvancedHeaderItem &QAdvancedHeaderItem::SetType(ItemTypeEnum type)
{
    _type = type;
    return *this;
}

QAdvancedHeaderItem *QAdvancedHeaderItem::clone() const
{
    return new QAdvancedHeaderItem(*this);
}
