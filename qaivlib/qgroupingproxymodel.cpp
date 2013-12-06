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
#include "qgroupingproxymodel.h"
#include "qgroupingproxymodel_p.h"

class QGroupingProxyModelPrivate
{
public:
    QGroupingProxyModelPrivate(QGroupingProxyModel* pm);
    ~QGroupingProxyModelPrivate();

    int modelColumn;
    int groupItemDataRole;
    QGroupingProxyModelGroup* root;
    QAbstractItemModel* sourceModel;

    QGroupingProxyModel* m;
};

QGroupingProxyModelPrivate::QGroupingProxyModelPrivate(QGroupingProxyModel *pm)
{

}

QGroupingProxyModelPrivate::~QGroupingProxyModelPrivate()
{

}

QGroupingProxyModelGroup::QGroupingProxyModelGroup(QGroupingProxyModelGroup* parent)
{
    cParent = parent;
    if (cParent){
        cParent->cChildren.append(this);
    }
    cSourceModelRow = -1;
}

QGroupingProxyModelGroup::QGroupingProxyModelGroup(int sourceModelRow, QGroupingProxyModelGroup *parent)
{
    cParent = parent;
    if (cParent){
        cParent->cChildren.append(this);
    }
    cSourceModelRow = sourceModelRow;
}

QGroupingProxyModelGroup::~QGroupingProxyModelGroup()
{
    qDeleteAll(cChildren);
}

void QGroupingProxyModelGroup::addSourceModelRow(int row)
{
    if (!cSourceModelRows.contains(row)){
        cSourceModelRows.append(row);
    }
}

void QGroupingProxyModelGroup::clear()
{
    for (int iChild = cChildren.size() - 1; iChild > -1; --iChild){
        if (cChildren.at(iChild)->sourceModelRow() == -1){
            cChildren.at(iChild)->clear();
        } else {
            delete cChildren.takeAt(iChild);
        }
    }
}

QGroupingProxyModelGroup* QGroupingProxyModelGroup::child(int index) const
{
    return cChildren.value(index);
}

int QGroupingProxyModelGroup::childCount() const
{
    return cChildren.size();
}

QVariant QGroupingProxyModelGroup::data(int role) const
{
    return cData.value(role);
}

QGroupingProxyModelGroup *QGroupingProxyModelGroup::findSourceModelRow(int sourceModelRow) const
{
    if (cSourceModelRow == sourceModelRow){
        return const_cast<QGroupingProxyModelGroup*>(this);
    }
    QGroupingProxyModelGroup* mGroup;
    Q_FOREACH(QGroupingProxyModelGroup* mItem, cChildren){
        mGroup = mItem->findSourceModelRow(sourceModelRow);
        if (mGroup){
            return mGroup;
        }
    }
    return 0;
}

QGroupingProxyModelGroup* QGroupingProxyModelGroup::group(int sourceModelRow) const
{
    if (cSourceModelRow != -1 && cSourceModelRow == sourceModelRow){
        return parent();
    }
    QGroupingProxyModelGroup* mGroup;
    Q_FOREACH(QGroupingProxyModelGroup* mItem, cChildren){
        mGroup = mItem->findSourceModelRow(sourceModelRow);
        if (mGroup){
            return mGroup->parent();
        }
    }
    return 0;
}

int QGroupingProxyModelGroup::indexOf(QGroupingProxyModelGroup* group) const
{
    return cChildren.indexOf(group);
}

int QGroupingProxyModelGroup::sourceModelRowIndexAt(int sourceModelRow) const
{
    for (int iChild = 0; iChild < cChildren.size(); iChild++){
        if (cChildren.at(iChild)->sourceModelRow() == sourceModelRow){
            return iChild;
        }
    }
    return -1;
}

QGroupingProxyModelGroup* QGroupingProxyModelGroup::matches(const QVariant & value) const
{
    if (cSourceModelRow != -1){
        return 0;
    }
    if (cParent != 0 && data(Qt::EditRole) == value){
        return const_cast<QGroupingProxyModelGroup*>(this);
    }
    QGroupingProxyModelGroup* mGroup;
    Q_FOREACH(QGroupingProxyModelGroup* mItem, cChildren){
        mGroup = mItem->matches(value);
        if (mGroup){
            return mGroup;
        }
    }
    return 0;
}

void QGroupingProxyModelGroup::moveSourceRow(int count)
{
    cSourceModelRow += count;
}

QGroupingProxyModelGroup *QGroupingProxyModelGroup::parent() const
{
    return cParent;
}

void QGroupingProxyModelGroup::removeChild(int index)
{
    if (index < cChildren.size()){
        delete cChildren.takeAt(index);
    }
}

void QGroupingProxyModelGroup::removeChildAtSourceModelRow(int sourceModelRow)
{
    for (int iChild = 0; iChild < cChildren.size(); iChild++){
        if (cChildren.at(iChild)->sourceModelRow() == sourceModelRow){
            delete cChildren.takeAt(iChild);
        }
    }
}

void QGroupingProxyModelGroup::removeSourceModelRow(int row)
{
    cSourceModelRows.removeAt(cSourceModelRows.indexOf(row));
}

int QGroupingProxyModelGroup::row() const
{
    if (cParent){
        return cParent->cChildren.indexOf(const_cast<QGroupingProxyModelGroup*>(this));
    }
    return 0;
}

int QGroupingProxyModelGroup::row(int sourceModelRow) const
{
    for (int iChild = 0; iChild < cChildren.size(); iChild++){
        if (cChildren.at(iChild)->sourceModelRow() == sourceModelRow){
            return iChild;
        }
    }
    return -1;
}

void QGroupingProxyModelGroup::setData(const QVariant & data, int role)
{
    cData[role] = data;
}

int QGroupingProxyModelGroup::sourceModelRow() const
{
    return cSourceModelRow;
}

QList<int> QGroupingProxyModelGroup::sourceModelRows() const
{
    return cSourceModelRows;
}

QGroupingProxyModel::QGroupingProxyModel(QObject *parent) :
    QAbstractItemModel(parent), d(new QGroupingProxyModelPrivate(this))
{    
    d->modelColumn = 0;
    d->groupItemDataRole = Qt::DisplayRole;

    d->sourceModel = new QStandardItemModel(this);
    d->root = new QGroupingProxyModelGroup();
    QGroupingProxyModelGroup* item = new QGroupingProxyModelGroup(d->root);
    item->setData(tr("Ungrouped"), Qt::DisplayRole);
}

QGroupingProxyModel::~QGroupingProxyModel()
{
    delete d;
}

void QGroupingProxyModel::addGroup(const QString & text, const QVariant & value)
{
    QGroupingProxyModelGroup* g = new QGroupingProxyModelGroup(d->root);
    g->setData(text, Qt::DisplayRole);
    if (value.isNull()){
        g->setData(text, Qt::EditRole);
    } else {
        g->setData(value, Qt::EditRole);
    }
    buildGroups();
}

void QGroupingProxyModel::addGroup(const QIcon & icon, const QString &text, const QVariant &value)
{
    QGroupingProxyModelGroup* g = new QGroupingProxyModelGroup(d->root);
    g->setData(icon, Qt::DecorationRole);
    g->setData(text, Qt::DisplayRole);
    if (value.isNull()){
        g->setData(text, Qt::EditRole);
    } else {
        g->setData(value, Qt::EditRole);
    }
    buildGroups();
}

int QGroupingProxyModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return d->sourceModel->columnCount() + 1;
}

QVariant QGroupingProxyModel::data(const QModelIndex & proxyIndex, int role) const
{
    if (!proxyIndex.isValid()){
        return QVariant();
    }
//    if (role == Qt::DisplayRole){
//        if (proxyIndex.column() == 0){
//            QGroupingProxyModelGroup* mItem = static_cast<QGroupingProxyModelGroup*>(proxyIndex.internalPointer());
//            if (mItem->parent() == d->root){
//                return QString("%1 [%2]").arg(mItem->data(Qt::DisplayRole).toString()).arg(mItem->childCount());
//            } else {
//                return "A";
//            }
//        } else {

//        }
//    }
    QGroupingProxyModelGroup* item = static_cast<QGroupingProxyModelGroup*>(proxyIndex.internalPointer());
    if (item->parent() == d->root){
        if (proxyIndex.column() == 0){
            if (role == Qt::DisplayRole){
                return QString("%1 [%2]").arg(item->data(Qt::DisplayRole).toString()).arg(item->childCount());
            } else {
                return item->data(role);
            }
        }
    } else {
        if (proxyIndex.column() == 0){
            if (role == Qt::DisplayRole){
                return "";
            }
        }
    }
    return mapToSource(proxyIndex).data(role);
}

Qt::ItemFlags QGroupingProxyModel::flags(const QModelIndex & index) const
{
    QGroupingProxyModelGroup* item = static_cast<QGroupingProxyModelGroup*>(index.internalPointer());
    if (item && item->parent() == d->root){
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    } else {
        if (index.column() == 0){
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
    }
    return d->sourceModel->flags(mapToSource(index));
}

QVariant QGroupingProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section == 0){
        if (role == Qt::DisplayRole){
            return tr("Group");
        }
        return QVariant();
    }
    return d->sourceModel->headerData(section - 1, orientation, role);
}

QModelIndex QGroupingProxyModel::index(int row, int column, const QModelIndex & parent) const
{
    if (!hasIndex(row, column, parent)){
        return QModelIndex();
    }
    QGroupingProxyModelGroup* mParentItem;
    QGroupingProxyModelGroup* mChildItem;
    if (!parent.isValid()){
        mParentItem = d->root;
    } else {
        mParentItem = static_cast<QGroupingProxyModelGroup*>(parent.internalPointer());
    }
    mChildItem = mParentItem->child(row);
    if (mChildItem){
        return createIndex(row, column, mChildItem);
    }
    return QModelIndex();
}

int QGroupingProxyModel::findText(const QString & text) const
{
    for (int iGroup = 0; iGroup < d->root->childCount(); iGroup++){
        if (d->root->child(iGroup)->data(Qt::DisplayRole).toString() == text){
            return iGroup;
        }
    }
    return -1;
}

QModelIndex QGroupingProxyModel::mapFromSource(const QModelIndex & sourceIndex) const
{
    return QModelIndex();
}

QModelIndex QGroupingProxyModel::mapToSource(const QModelIndex & proxyIndex) const
{
    QGroupingProxyModelGroup* item = static_cast<QGroupingProxyModelGroup*>(proxyIndex.internalPointer());
    if (item == d->root){
        return QModelIndex();
    }
    if (item == 0){
        return QModelIndex();
    }
    return d->sourceModel->index(item->sourceModelRow(), proxyIndex.column() - 1);
}

int QGroupingProxyModel::modelColumn() const
{
    return d->modelColumn;
}

QModelIndex QGroupingProxyModel::parent(const QModelIndex & child) const
{
    if (!child.isValid()){
        return QModelIndex();
    }
    QGroupingProxyModelGroup* childItem = static_cast<QGroupingProxyModelGroup*>(child.internalPointer());
    QGroupingProxyModelGroup* parentItem = childItem->parent();
    if (parentItem == d->root){
        return QModelIndex();
    }
    return createIndex(parentItem->row(), 0, parentItem);
}

bool QGroupingProxyModel::removeGroup(int index)
{
    if (index < 1){
        return false;
    }
    beginRemoveRows(QModelIndex(), index, index);
    d->root->removeChild(index);
    buildGroups();
    endRemoveRows();
    return true;
}

bool QGroupingProxyModel::restoreGroups(const QByteArray & data)
{
    QByteArray groups(data);
    QDataStream stream(&groups, QIODevice::ReadOnly);
    qint32 version;

    stream >> version;
    if (version == 1){
        beginResetModel();

        for (int iGroup = 1; iGroup < d->root->childCount(); iGroup++){
            d->root->removeChild(iGroup);
        }
        QIcon icon;
        QString text;
        QVariant value;
        stream >> d->modelColumn >> d->groupItemDataRole;
        while(!stream.atEnd()){
            stream >> icon >> text >> value;
            addGroup(icon, text, value);
        }
        endResetModel();
        buildGroups();
        return true;
    }
    return false;
}

int QGroupingProxyModel::rowCount(const QModelIndex & parent) const
{
    if (parent.column() > 0){
        return 0;
    }
    QGroupingProxyModelGroup* parentItem;
    if (!parent.isValid()){
        parentItem = d->root;
    } else {
        parentItem = static_cast<QGroupingProxyModelGroup*>(parent.internalPointer());
    }
    return parentItem->childCount();
}

bool QGroupingProxyModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (!index.parent().isValid()){
        d->root->child(index.row())->setData(value, role);
    }
    return d->sourceModel->setData(mapToSource(index), value, role);
}

QByteArray QGroupingProxyModel::saveGroups() const
{
    QByteArray groups;
    QDataStream stream(&groups, QIODevice::WriteOnly);
    stream << qint32(1) << qint32(d->modelColumn) << qint32(d->groupItemDataRole);

    for (int iGroup = 1; iGroup < d->root->childCount(); iGroup++){
        stream << d->root->child(iGroup)->data(Qt::DecorationRole)
                << d->root->child(iGroup)->data(Qt::DisplayRole)
                << d->root->child(iGroup)->data(Qt::EditRole);
    }
    return groups;
}

void QGroupingProxyModel::setModelColumn(int column)
{
    d->modelColumn = column;
    // @todo filter
}

void QGroupingProxyModel::setSourceModel(QAbstractItemModel* sourceModel)
{
    d->sourceModel = sourceModel;
    buildGroups();
    connect(d->sourceModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(dataChangedHandler(QModelIndex,QModelIndex)));
    connect(d->sourceModel, SIGNAL(rowsAboutToBeInserted(QModelIndex, int, int)), this, SLOT(rowsAboutToBeInsertedHandler(QModelIndex,int,int)));
    connect(d->sourceModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(rowsInsertedHandler(QModelIndex,int,int)));
    connect(d->sourceModel, SIGNAL(modelReset()), this, SLOT(sourceModelResetHandler()));
}

void QGroupingProxyModel::dataChangedHandler(const QModelIndex & topLeft, const QModelIndex & bottomRight)
{
    if (topLeft.column() != d->modelColumn){
        QGroupingProxyModelGroup* mCurrentGroup = d->root->group(topLeft.row());
        QModelIndex mTopLeft = index(mCurrentGroup->parent()->indexOf(mCurrentGroup), 0);
        QModelIndex mBottomRight = index(mTopLeft.row(), d->sourceModel->columnCount());
        emit dataChanged(mTopLeft, mBottomRight);
    } else {
        QGroupingProxyModelGroup* currentGroup = d->root->group(topLeft.row());
        QGroupingProxyModelGroup* newGroup = d->root->matches(d->sourceModel->index(topLeft.row(), d->modelColumn).data(d->groupItemDataRole));
        if (newGroup == 0){
            newGroup = d->root->child(0);
        }
        if (currentGroup != newGroup){
            QModelIndex p;
            p = index(currentGroup->parent()->indexOf(currentGroup), 0);
            beginRemoveRows(p, currentGroup->row(topLeft.row()), currentGroup->row(topLeft.row()));
            currentGroup->removeChildAtSourceModelRow(topLeft.row());
            endRemoveRows();

            p = index(newGroup->parent()->indexOf(newGroup), 0);
            beginInsertRows(p, newGroup->childCount(), newGroup->childCount());
            (void)new QGroupingProxyModelGroup(topLeft.row(), newGroup);
            endInsertRows();
        }
    }
}

void QGroupingProxyModel::rowsAboutToBeInsertedHandler(const QModelIndex & parent, int start, int end)
{
//    d->root->child(0)->addSourceModelRow(start);
    //    qDebug() << d->root->child(0)->sourceModelRows();
}

void QGroupingProxyModel::rowsInsertedHandler(const QModelIndex & parent, int first, int last)
{
    // @todo
//    int mGroup = d->root->group(cSourceModel->index(first, cModelColumn).data(cGroupItemDataRole));
//    beginResetModel();
//    moveRows(first, 1);
//    if (mGroup == -1){
//        new QGroupingProxyModelGroup(first, d->root->child(0));
//    } else {
//        new QGroupingProxyModelGroup(first, d->root->child(mGroup));
//    }
    //    endResetModel();
}

void QGroupingProxyModel::buildGroups()
{
    beginResetModel();
    d->root->clear();
    if (d->sourceModel){
        QGroupingProxyModelGroup* group;
        for (int iRow = 0; iRow < d->sourceModel->rowCount(); iRow++){
            group = d->root->matches(d->sourceModel->index(iRow, d->modelColumn).data(d->groupItemDataRole));
            if (group == 0){
                group = d->root->child(0);
            }
            new QGroupingProxyModelGroup(iRow, group);
        }
    }
    endResetModel();
}

int QGroupingProxyModel::groupAt(int sourceModelRow)
{
    for (int iGroup = 0; iGroup < d->root->childCount(); iGroup++){
        for (int iRow = 0; iRow < d->root->child(iGroup)->childCount(); iRow++){
            if (d->root->child(iGroup)->child(iRow)->sourceModelRow() == sourceModelRow){
                return iGroup;
            }
        }
    }
    return -1;
}

void QGroupingProxyModel::moveRows(int row, int count)
{
    for (int iGroup = 0; iGroup < d->root->childCount(); iGroup++){
        for (int iChild = 0; iChild < d->root->child(iGroup)->childCount(); iChild++){
            if (d->root->child(iGroup)->child(iChild)->sourceModelRow() >= row){
                d->root->child(iGroup)->child(iChild)->moveSourceRow(count);
            }
        }
    }
}

void QGroupingProxyModel::removeSourceModelRow(int sourceModelRow)
{
    for (int iGroup = 0; iGroup < d->root->childCount(); iGroup++){
        d->root->child(iGroup)->removeChildAtSourceModelRow(sourceModelRow);
    }
}

void QGroupingProxyModel::sourceModelResetHandler()
{
	buildGroups();
}
