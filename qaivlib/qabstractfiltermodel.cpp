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
#include "qabstractfiltermodel.h"

#include <qabstractfilter.h>
#include "qabstractfilterproxymodel.h"
#include <qfiltergroup.h>

class QAbstractFilterModelPrivate
{
public:
    QAbstractFilterModelPrivate(QAbstractFilterModel* fm);
    ~QAbstractFilterModelPrivate();

    QMap<int,int> defaultFilterType;

    QMap<int, QVariantList> columnFilterTypes;

    QList<QFilterGroup*> filterGroupList;

    QAdvancedItemViews::FilterProxyMode mode;

    QAdvancedItemViews::FilterMatchMode matchMode;

    QAbstractItemModel* sourceModel;

    QMap<int,int> valueTypeMap;

    QList<QAbstractFilterModel::FilterTypeEntry> filterTypes;

    QAbstractFilterModel* m;
};

QAbstractFilterModelPrivate::QAbstractFilterModelPrivate(QAbstractFilterModel *fm)
{
    m = fm;
}

QAbstractFilterModelPrivate::~QAbstractFilterModelPrivate()
{
    qDeleteAll(filterGroupList);
}

QAbstractFilterModel::QAbstractFilterModel(QObject *parent)
    : QAbstractItemModel(parent), d(new QAbstractFilterModelPrivate(this))
{
    d->filterGroupList.append(new QFilterGroup(tr("<default>")));
    d->matchMode = QAdvancedItemViews::MatchNormal;
    d->mode = QAdvancedItemViews::FilterMode;
    d->sourceModel = 0;
}

QAbstractFilterModel::~QAbstractFilterModel()
{
    delete d;
}

int QAbstractFilterModel::columnCount(const QModelIndex & parent) const
{
    if (d->sourceModel){
        return d->sourceModel->columnCount(parent);
    }
    return 1;
}

QVariant QAbstractFilterModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()){
        return QVariant();
    }
    QAbstractFilter* filter = d->filterGroupList.at(index.row())->filterAtColumn(index.column());
    if (role == Qt::DecorationRole){
        if (filter){
            if (filter->isEnabled()){
                return QIcon(":/qaiv/filter/enabled");
            } else {
                return QIcon(":/qaiv/filter/disabled");
            }
        }
        return QVariant();
    } else if (role == QAbstractFilterModel::DefaultFilterTypeRole){
        return d->defaultFilterType.value(index.column(), QAbstractFilter::Type);
    } else if (role == Qt::DisplayRole){
        if (filter){
            return filter->data();
        } else {
            return tr("<all>");
        }        
    } else if (role == Qt::EditRole){
        filter = d->filterGroupList.at(index.row())->filterAtColumn(index.column());
        if (filter){
            return filter->properties();
        } else {
            return QVariant();
        }
    } else if (role == QAbstractFilterModel::ColumnFilterTypesRole){
        return d->columnFilterTypes.value(index.column());
    } else if (role == QAbstractFilterModel::ValueFilterTypeRole){
        return d->valueTypeMap.value(index.column(), -1);
    }
    return QVariant();
}

QAbstractFilter* QAbstractFilterModel::filter(const QModelIndex & index) const
{
    if (index.isValid() && index.model() == this){
        return d->filterGroupList.at(index.row())->filterAtColumn(index.column());
    }
    return 0;
}

QFilterGroup* QAbstractFilterModel::filterGroup(const QModelIndex & index) const
{
    if (index.row() < d->filterGroupList.size()){
        return d->filterGroupList.at(index.row());
    }
    return 0;
}

QList<QAbstractFilter*> QAbstractFilterModel::filtersAtRow(int row) const
{
    if (row < d->filterGroupList.size()){
        return d->filterGroupList.at(row)->filters();
    }
    return QList<QAbstractFilter*>();
}

Qt::ItemFlags QAbstractFilterModel::flags(const QModelIndex & index) const
{
	Q_UNUSED(index);
    Qt::ItemFlags f;
    f |= Qt::ItemIsEditable;
    f |= Qt::ItemIsEnabled;
    f |= Qt::ItemIsSelectable;
    return f;
}

QFilterGroup* QAbstractFilterModel::groupAt(int row) const
{
    if (row < d->filterGroupList.size()){
        return d->filterGroupList.at(row);
    }
    return 0;
}

QVariant QAbstractFilterModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if (orientation == Qt::Vertical){
        if (role == Qt::DisplayRole){
            return d->filterGroupList.at(section)->name();
        }
    } else if (orientation == Qt::Horizontal){
        if (d->sourceModel){
            if (role == Qt::DisplayRole){
                return d->sourceModel->headerData(section, Qt::Horizontal, Qt::DisplayRole);
            }
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

QModelIndex QAbstractFilterModel::index(int row, int column, const QModelIndex & parent) const
{
	Q_UNUSED(parent);
    return createIndex(row, column);
}

bool QAbstractFilterModel::insertRows(int row, int count, const QModelIndex & parent)
{
    Q_UNUSED(parent);
    if(count == 0){
        return false;
    }
    beginInsertRows(parent, row, row + count - 1);
    if (row == rowCount()){
        for (int iCount = 0; iCount < count; iCount++){
            d->filterGroupList.append(new QFilterGroup(tr("<filter>")));
        }
    } else {
        for (int iRow = row; iRow < d->filterGroupList.size(); iRow++){
            for (int iFilter = 0; iFilter < d->filterGroupList.at(iRow)->count(); iFilter++){
                d->filterGroupList.at(iRow)->filter(iFilter)->setRow(d->filterGroupList.at(iRow)->filter(iFilter)->row() + count);
            }
        }
        for (int iCount = 0; iCount < count; iCount++){
            d->filterGroupList.insert(row, new QFilterGroup(tr("<filter>")));
        }
    }
    endInsertRows();
    return true;
}

QAdvancedItemViews::FilterMatchMode QAbstractFilterModel::matchMode() const
{
    return d->matchMode;
}

QAdvancedItemViews::FilterProxyMode QAbstractFilterModel::mode() const
{
    return d->mode;
}

QModelIndex QAbstractFilterModel::parent(const QModelIndex & index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

QList<QAbstractFilterModel::FilterTypeEntry> QAbstractFilterModel::registeredFilterTypes() const
{
    return d->filterTypes;
}

bool QAbstractFilterModel::removeRows(int row, int count, const QModelIndex & parent)
{
    Q_UNUSED(parent);
    if (count == 0){
        return false;
    }
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int iCount = 0; iCount < count; iCount++){
        d->filterGroupList.removeAt(row);
    }
    for (int iRow = row; iRow < d->filterGroupList.size(); iRow++){
        for (int iFilter = 0; iFilter < d->filterGroupList.at(iRow)->count(); iFilter++){
            d->filterGroupList.at(iRow)->filter(iFilter)->setRow(d->filterGroupList.at(iRow)->filter(iFilter)->row() - count);
        }
    }
    endRemoveRows();
    return true;
}

int QAbstractFilterModel::rowCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent);
    return d->filterGroupList.size();
}

bool QAbstractFilterModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (!index.isValid()){
        return false;
    }
    if (role == QAbstractFilterModel::DefaultFilterTypeRole){
        d->defaultFilterType[index.column()] = value.toInt();
    } else if (role == Qt::EditRole){
        if (value.toMap().isEmpty()){
            d->filterGroupList.at(index.row())->remove(index.column());
        } else {
            QAbstractFilter* filter = d->filterGroupList.at(index.row())->filterAtColumn(index.column());
            if (filter == 0){
                // Create new filter
                filter = createFilter(index, value.toMap());
                if (filter == 0){
                    return false;
                }
            } else {
                QMapIterator<QString,QVariant> it(value.toMap());
                while(it.hasNext()){
                    it.next();
                    filter->setProperty(it.key(), it.value());
                }
            }
        }
        emit dataChanged(index, index);
        return true;
    } else if (role == QAbstractFilterModel::ColumnFilterTypesRole){
        d->columnFilterTypes[index.column()] = value.toList();
    } else if (role == QAbstractFilterModel::ValueFilterTypeRole){
        d->valueTypeMap[index.column()] = value.toInt();
    }
    return false;
}

void QAbstractFilterModel::setDefaultFilterType(int column, int type)
{
    d->defaultFilterType[column] = type;
}

bool QAbstractFilterModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
    if (role != Qt::EditRole){
        return false;
    }
    if (orientation == Qt::Vertical){
        d->filterGroupList.at(section)->setName(value.toString());
        emit headerDataChanged(Qt::Horizontal, section, section);
        return true;
    }
    return false;
}

void QAbstractFilterModel::setMatchMode(QAdvancedItemViews::FilterMatchMode flag)
{
    if (d->matchMode != flag){
        d->matchMode = flag;
        emit matchModeChanged(d->matchMode);
    }
}

void QAbstractFilterModel::setMode(QAdvancedItemViews::FilterProxyMode mode)
{
    if (d->mode != mode){
        d->mode = mode;
        emit modeChanged(d->mode);
    }
}

void QAbstractFilterModel::setSourceModel(QAbstractItemModel* model)
{
    if (model){
        d->sourceModel = model;
		connect(d->sourceModel, SIGNAL(layoutChanged()), this, SLOT(sourceModelLayoutChanged()));
        headerDataChanged(Qt::Horizontal, 0, d->sourceModel->columnCount() - 1);
    }
}

QAbstractItemModel* QAbstractFilterModel::sourceModel() const
{
    return d->sourceModel;
}

void QAbstractFilterModel::sourceModelLayoutChanged()
{
	if (d->sourceModel){
		headerDataChanged(Qt::Horizontal, 0, d->sourceModel->columnCount() - 1);
	}
}

bool QAbstractFilterModel::registerFilter(int type, const QString & text, const QString & toolTip)
{
    return registerFilter(type, QIcon(), text, toolTip);
}

bool QAbstractFilterModel::registerFilter(int type, const QIcon &icon, const QString &text, const QString &toolTip)
{
    Q_FOREACH(FilterTypeEntry e, d->filterTypes){
        if (e.type == type){
            return false;
        }
    }

    FilterTypeEntry e;
    e.type = type;
    e.icon = icon;
    e.text = text;
    e.tooltip = toolTip;
    d->filterTypes << e;
    return true;
}

