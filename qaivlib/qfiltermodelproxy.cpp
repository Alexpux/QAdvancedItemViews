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
#include "qfiltermodelproxy.h"

#include "qabstractfilter.h"
#include "qabstractfiltermodel.h"

QFilterModelProxy::QFilterModelProxy(QObject* parent)
    : QAbstractFilterProxyModel(parent)
{
}

QFilterModelProxy::~QFilterModelProxy()
{
}

QVariant QFilterModelProxy::data(const QModelIndex & index, int role) const
{    
    if (role == Qt::BackgroundRole && filterModel()->mode() == QAdvancedItemViews::HighlightMode){
        QBrush mBrush = qvariant_cast<QBrush>(QAbstractFilterProxyModel::data(index, role));
        int mType = -1;
		QModelIndex mSourceIndex = mapToSource(index);
        for(int iRow = 0; iRow < filterModel()->rowCount(); iRow++){
            Q_FOREACH(QAbstractFilter* mFilter, filterModel()->filtersAtRow(iRow)){
                mType = filterModel()->index(0, mFilter->column()).data(QAbstractFilterModel::ValueFilterTypeRole).toInt();
                if (mFilter->isEnabled()){
                    if (filterModel()->matchMode() == QAdvancedItemViews::MatchNormal){
                        if (mFilter->matches(sourceModel()->index(mSourceIndex.row(), mFilter->column()).data(), mType)){
                            mBrush.setColor(mFilter->highlightColor());
                            mBrush.setStyle(Qt::SolidPattern);
                            return mBrush;
                        }
                    } else if (filterModel()->matchMode() == QAdvancedItemViews::MatchInverted){
                        if (!mFilter->matches(sourceModel()->index(mSourceIndex.row(), mFilter->column()).data(), mType)){
                            mBrush.setColor(mFilter->highlightColor());
                            mBrush.setStyle(Qt::SolidPattern);
                            return mBrush;
                        }
                    }
                }
            }
        }
    }
    return QAbstractFilterProxyModel::data(index, role);
}

bool QFilterModelProxy::filterAcceptsRow( int source_row, const QModelIndex & source_parent ) const
{
    if (filterModel()->mode() == QAdvancedItemViews::HighlightMode){
        return true;
    }
    int mType = -1;
    for(int iRow = 0; iRow < filterModel()->rowCount(); iRow++){
        Q_FOREACH(QAbstractFilter* mFilter, filterModel()->filtersAtRow(iRow)){
            mType = filterModel()->index(0, mFilter->column()).data(QAbstractFilterModel::ValueFilterTypeRole).toInt();
            if (mFilter->isEnabled()){
                if (filterModel()->matchMode() == QAdvancedItemViews::MatchNormal){
                    if (mFilter->matches(sourceModel()->data(sourceModel()->index(source_row, mFilter->column())), mType)){
                        return true;
                    } else {
                        return false;
                    }
                } else if (filterModel()->matchMode() == QAdvancedItemViews::MatchInverted){
                    if (!mFilter->matches(sourceModel()->data(sourceModel()->index(source_row, mFilter->column())), mType)){
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
