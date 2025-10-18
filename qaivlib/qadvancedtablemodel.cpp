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

#include "qadvancedtablemodel.h"

#include "qadvancedconstants.h"

/* ***************************************************************************************
 * Some info:
 *  Check the data() and headerData() methods
 * ***************************************************************************************/

QAdvancedTableModel::QAdvancedTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    _columns_model = new QStandardItemModel;
    _rows_model = new QStandardItemModel;
}

QAdvancedTableModel::~QAdvancedTableModel()
{
    delete _columns_model;
    delete _rows_model;
}

int QAdvancedTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (_rows_model->item(0) == nullptr) {
        return 0;
    }
    return QAdvancedHeaderItem::CastFromQStandardItem(_rows_model->item(0))->GetNumberOfChildren();
}

int QAdvancedTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (_columns_model->item(0) == nullptr) {
        return 0;
    }
    return QAdvancedHeaderItem::CastFromQStandardItem(_columns_model->item(0))->GetNumberOfChildren();
}

QAdvancedTableModel &QAdvancedTableModel::setRow(QAdvancedHeaderItem *rowItem)
{
    _rows_model->setItem(0, 0, rowItem);
    return *this;
}

QAdvancedTableModel &QAdvancedTableModel::setColumn(QAdvancedHeaderItem *columnItem)
{
    _columns_model->setItem(0, 0, columnItem);
    return *this;
}

QAdvancedHeaderItem *QAdvancedTableModel::GetRootItem(Qt::Orientation orientation) const
{
    if (orientation == Qt::Horizontal) {
        return QAdvancedHeaderItem::CastFromQStandardItem(_columns_model->item(0));
    }
    return QAdvancedHeaderItem::CastFromQStandardItem(_rows_model->item(0));
}

QAdvancedHeaderItem *QAdvancedTableModel::GetItemAtSection(int section, Qt::Orientation orientation) const
{
    const QStandardItemModel *rowcol = (orientation == Qt::Horizontal) ? _columns_model : _rows_model;
    QAdvancedHeaderItem *rootItem = QAdvancedHeaderItem::CastFromQStandardItem(rowcol->item(0));
    if (rootItem) {
        return rootItem->GetLeaf(section);
    }
    return nullptr;
}

// data and headerData control what is shown
QVariant QAdvancedTableModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index)
    if (role == CustomRoles::MultiLevelHeaderView_Rows) {
        QVariant v;
        v.setValue(_rows_model);
        return v;
    }
    if (role == CustomRoles::MultiLevelHeaderView_Columns) {
        QVariant v;
        v.setValue(_columns_model);
        return v;
    }
    return QVariant();
}

QVariant QAdvancedTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == CustomRoles::MultiLevelHeaderView_Rows) {
        QVariant v;
        v.setValue(_rows_model);
        return v;
    }
    if (role == CustomRoles::MultiLevelHeaderView_Columns) {
        QVariant v;
        v.setValue(_columns_model);
        return v;
    }
    if (role == Qt::ToolTipRole) {
        /*QAdvancedHeaderItem *item = GetItemAtSection(section, orientation);
        if (item) {
            switch(orientation) {
            case Qt::Orientation::Horizontal:
            {
                return QString("HTooltip ") + GetItemAtSection(section, orientation)->_name;
            }
            case Qt::Orientation::Vertical:
            {
                return QString("VTooltip ") + GetItemAtSection(section, orientation)->_name;
            }
            default:
                break;
            }
        }*/
    }
    if (role == Qt::DisplayRole) {
        QAdvancedHeaderItem *rootItem = GetRootItem(orientation);
        if (rootItem) {
            return rootItem->GetLeaf(section)->_name;
        }
    }
    /*if (role == Qt::DecorationRole) {
        QAdvancedHeaderIteml *currItem = GetItemAtSection(section, orientation);
        if (currItem) {
            return currItem->_pixmap;
        }
    }*/
    return QVariant();
}

void QAdvancedTableModel::emit_layoutChanged()
{
    emit layoutChanged();
}
