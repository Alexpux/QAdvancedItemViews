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
#ifndef QFILTERVIEWCONNECTOR_H
#define QFILTERVIEWCONNECTOR_H

#include <QObject>

#include <qaivlib_global.h>

class QFilterView;
class QTableView;
class QTreeView;

//! The QFilterViewConnector class connects a QFilterView with a QTableView or QTreeView.
class QAIVLIBSHARED_EXPORT QFilterViewConnector : public QObject
{
    Q_OBJECT
public:
    explicit QFilterViewConnector(QFilterView* filterView);
    explicit QFilterViewConnector(QFilterView* filterView, QTableView* tableView, QObject *parent = 0);
    explicit QFilterViewConnector(QFilterView* filterView, QTreeView* treeView, QObject *parent = 0);

    void setDataView(QTableView* view);
    void setDataView(QTreeView* view);
signals:
    
public slots:
    void adjustVerticalHeaderWidth();
private slots:
    void dataViewHorizontalScrollBarRangeChanged(int min, int max);
    void dataViewHorizontalScrollBarSilderMoved(int value);
    void dataViewHorizontalSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void dataViewHorizontalSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
    void dataViewHorizontalSectionResized(int logicalIndex, int oldSize, int newSize);
    void dataViewVerticalScrollBarRangeChanged(int min, int max);

    void filterViewHorizontalSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
    void filterViewHorizontalSectionResized(int logicalIndex, int oldSize, int newSize);
    void filterViewHorizontalSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
private:
    QFilterView* cFilterView;
    QTableView* cTableView;
    QTreeView* cTreeView;
};

#endif // QFILTERVIEWCONNECTOR_H
