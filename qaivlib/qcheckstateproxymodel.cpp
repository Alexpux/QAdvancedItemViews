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
#include "qcheckstateproxymodel.h"

class QCheckStateProxyModelPrivate
{
public:
    QCheckStateProxyModelPrivate(QCheckStateProxyModel* pm);
    ~QCheckStateProxyModelPrivate();

    QList<QModelIndex> checkedIndexes;
    QList<int> columns;

    QCheckStateProxyModel* m;
};

QCheckStateProxyModelPrivate::QCheckStateProxyModelPrivate(QCheckStateProxyModel *pm)
{
    columns << 0;
    m = pm;
}

QCheckStateProxyModelPrivate::~QCheckStateProxyModelPrivate()
{
}

QCheckStateProxyModel::QCheckStateProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent), d(new QCheckStateProxyModelPrivate(this))
{
}

QCheckStateProxyModel::~QCheckStateProxyModel()
{
    delete d;
}

int QCheckStateProxyModel::checkableColumnsCount() const
{
	return d->columns.size();
}

QModelIndexList QCheckStateProxyModel::checkedIndexes() const
{
    return d->checkedIndexes;
}

QVariant QCheckStateProxyModel::data(const QModelIndex & index, int role) const
{
    if (role == Qt::CheckStateRole && d->columns.contains(index.column())){
        return d->checkedIndexes.contains(index)?Qt::Checked:Qt::Unchecked;
    }
    return QSortFilterProxyModel::data(index, role);
}

Qt::ItemFlags QCheckStateProxyModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags f = QSortFilterProxyModel::flags(index);
    if (index.isValid() && d->columns.contains(index.column())){
        f |= Qt::ItemIsUserCheckable;
    }
    return f;
}

bool QCheckStateProxyModel::isColumnCheckable(int column) const
{
    return d->columns.contains(column);
}

void QCheckStateProxyModel::setAllChecked(bool checked)
{
	if (rowCount() == 0 || columnCount() == 0){
		return;
	}
	if (checked){
		for (int c = 0; c < d->columns.size(); c++){
			for (int r = 0; r < rowCount(); r++){
				QModelIndex i = index(r, c);
				if (!d->checkedIndexes.contains(i)){
					d->checkedIndexes.append(i);
				}
			}
		}
	} else {
		d->checkedIndexes.clear();
	}
	emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

void QCheckStateProxyModel::setColumnCheckable(int column, bool checkable)
{
    if (checkable){
        if (!d->columns.contains(column)){
            d->columns << column;
        }
    } else {
        d->columns.removeAt(d->columns.indexOf(column));
    }
    invalidateFilter();
}

void QCheckStateProxyModel::setChecked(const QModelIndex & index, bool checked)
{
	if (d->columns.contains(index.column()) && index.row() < rowCount()){
		if (checked){
			d->checkedIndexes.append(index);
			emit dataChanged(index, index);
		} else {
			if (d->checkedIndexes.contains(index)){
				d->checkedIndexes.removeAt(d->checkedIndexes.indexOf(index));
				emit dataChanged(index, index);
			}
		}
	}
}

void QCheckStateProxyModel::setCheckedIndexes(const QModelIndexList & indexes)
{
    d->checkedIndexes = indexes;
}

void QCheckStateProxyModel::setCheckedValues(int column, const QVariantList & values)
{
    if (sourceModel() == 0){
        return;
    }
    d->checkedIndexes.clear();
    QModelIndex mIndex;
    for(int iRow = 0; iRow < sourceModel()->rowCount(); iRow++){
        mIndex = mapFromSource(sourceModel()->index(iRow, column));
        if (values.contains(mIndex.data())){
            d->checkedIndexes.append(mIndex);
        }
    }
    if (!d->checkedIndexes.isEmpty()){
        emit dataChanged(d->checkedIndexes.first(), d->checkedIndexes.last());
    }
}

bool QCheckStateProxyModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::CheckStateRole){
        if (value.toInt() == Qt::Checked){
            d->checkedIndexes.append(index);
        } else {
            d->checkedIndexes.removeAt(d->checkedIndexes.indexOf(index));
        }
        emit dataChanged(index, index);
        return true;
    }
    return QSortFilterProxyModel::setData(index, value, role);
}

void QCheckStateProxyModel::setSourceModel(QAbstractItemModel* sourceModel)
{
    connect(sourceModel, SIGNAL(modelAboutToBeReset()), this, SLOT(sourceModelAboutToBeReset()));
    QSortFilterProxyModel::setSourceModel(sourceModel);
}

void QCheckStateProxyModel::sourceModelAboutToBeReset()
{
    d->checkedIndexes.clear();
}
