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
        QBrush b = qvariant_cast<QBrush>(QAbstractFilterProxyModel::data(index, role));
        int t = -1;
		QModelIndex mSourceIndex = mapToSource(index);
		QList<QColor> cl;
        for(int iRow = 0; iRow < filterModel()->rowCount(); iRow++){
            Q_FOREACH(QAbstractFilter* f, filterModel()->filtersAtRow(iRow)){
                t = filterModel()->index(0, f->column()).data(QAbstractFilterModel::ValueFilterTypeRole).toInt();
                if (f->isEnabled()){
                    if (filterModel()->matchMode() == QAdvancedItemViews::MatchNormal){
                        if (f->matches(sourceModel()->index(mSourceIndex.row(), f->column()).data(), t)){
							cl << f->highlightColor();
                        }
                    } else if (filterModel()->matchMode() == QAdvancedItemViews::MatchInverted){
                        if (!f->matches(sourceModel()->index(mSourceIndex.row(), f->column()).data(), t)){
							cl << f->highlightColor();
                        }
                    }
                }
            }
        }
		if (cl.size() == 1){
			return QBrush(cl.first());
		} else if (cl.size() > 1){
			QLinearGradient g(QPointF(0, 50), QPointF(100, 50));
			qreal s = 1 / ((qreal)cl.size() - 1);
			g.setColorAt(0, cl.first());
			g.setColorAt(1, cl.last());
			for (qreal i  = 1; i < cl.size() - 1; i++){
				g.setColorAt(s * i, cl.at(i));
			}
			return QBrush(g);
		}
		return b;
    }
    return QAbstractFilterProxyModel::data(index, role);
}

bool QFilterModelProxy::filterAcceptsRow( int source_row, const QModelIndex & source_parent ) const
{
    if (filterModel()->mode() == QAdvancedItemViews::HighlightMode){
        return true;
    }
    int t = -1;
	int r = -1;
    for(int iRow = 0; iRow < filterModel()->rowCount(); iRow++){
		int rr = -1;
		int fc = 0;
        Q_FOREACH(QAbstractFilter* f, filterModel()->filtersAtRow(iRow)){
            t = filterModel()->index(0, f->column()).data(QAbstractFilterModel::ValueFilterTypeRole).toInt();
            if (f->isEnabled()){
				fc++;
                if (filterModel()->matchMode() == QAdvancedItemViews::MatchNormal){
                    if (f->matches(sourceModel()->data(sourceModel()->index(source_row, f->column())), t)){
                        rr++;
					}
                } else if (filterModel()->matchMode() == QAdvancedItemViews::MatchInverted){
                    if (!f->matches(sourceModel()->data(sourceModel()->index(source_row, f->column())), t)){
                        rr++;
                    }
                }
			}
        }
		// fc == 0 && rr == -1 -> all filter in the current row are disabled
		// fc > 0 && rc == -1 -> at least on filter in the current row is enabled, but no match -> if r == -1 the r = 0
		// fc > 0 && rr + 1 == fc -> all filter in current row have matched -> if r == -1 set r = 0 -> inc r
		if (fc > 0){
			if (r == -1){
				r = 0;
			}
			if (rr + 1 == fc){
				r++;
			}
		}

    }
	if (r == -1){
		return true;
	}
    return r > 0;
}
