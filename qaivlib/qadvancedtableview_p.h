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
#ifndef QADVANCEDTABLEVIEW_P_H
#define QADVANCEDTABLEVIEW_P_H

#include "qadvancedtableview.h"
#include "qfiltermodelproxy.h"
#include "qtextfilter.h"

#include <QAbstractItemModel>
#include <QHeaderView>
#include <QMap>
#include <vector>

class QSharedItemSelectionModel;
class QAbstractFilterProxyModel;
class QFilterModelProxy;
class QAdvancedHeaderView;

// MODERN C++20: Column state structure for batch operations
struct ColumnState {
    int index { -1 };
    int size { 0 };
    bool hidden { false };
    int visualIndex { -1 };

    bool isValid() const { return index >= 0; }
};

class QAdvancedTableViewPrivate {
public:
    explicit QAdvancedTableViewPrivate(QAdvancedTableView *tv) :
        v { tv }
    {
        dataViewProxy = new QFilterModelProxy(tv);
    }

    QAdvancedTableViewPrivate(const QAdvancedTableViewPrivate &other)
    {
        autoResizeRowsToContents = other.autoResizeRowsToContents;
        autoResizeColumnsToFitView = other.autoResizeColumnsToFitView;
        defaultFilterType = other.defaultFilterType;
        dataViewProxy = new QFilterModelProxy(other.v);
        filterModel = other.filterModel;
        model = other.model;
        summaryView = other.summaryView;
        horizontalHeader = other.horizontalHeader;
        horizontalScrollBarPolicy = other.horizontalScrollBarPolicy;
        verticalHeader = other.verticalHeader;
        splittedViewSelectionModel = other.splittedViewSelectionModel;
        columnSpareWidthParts.clear();
        columnSpareWidthParts = other.columnSpareWidthParts;
        columnSpareWidthParts.detach();
        v = other.v;
    }

    QAdvancedTableViewPrivate &operator=(const QAdvancedTableViewPrivate &other)
    {
        if (&other != this) {
            autoResizeRowsToContents = other.autoResizeRowsToContents;
            autoResizeColumnsToFitView = other.autoResizeColumnsToFitView;
            defaultFilterType = other.defaultFilterType;
            dataViewProxy = new QFilterModelProxy(other.v);
            filterModel = other.filterModel;
            model = other.model;
            horizontalHeader = other.horizontalHeader;
            horizontalScrollBarPolicy = other.horizontalScrollBarPolicy;
            verticalHeader = other.verticalHeader;
            summaryView = nullptr;
            splittedViewSelectionModel = other.splittedViewSelectionModel;
            columnSpareWidthParts.clear();
            columnSpareWidthParts = other.columnSpareWidthParts;
            columnSpareWidthParts.detach();
            v = other.v;
        }
        return *this;
    }

    ~QAdvancedTableViewPrivate() = default;

    bool autoResizeRowsToContents { true };
    bool autoResizeColumnsToFitView { true };
    int defaultFilterType { QTextFilter::Type };

    // MODERN C++20: Unified synchronization control
    int batchUpdateDepth { 0 }; // Support nested batch operations

    bool isSyncSuppressed() const { return batchUpdateDepth > 0; }

    QAdvancedTableView *v { nullptr };
    QAbstractFilterProxyModel *dataViewProxy { nullptr };
    QAbstractFilterModel *filterModel { nullptr };
    QAbstractItemModel *model { nullptr };
    QAdvancedHeaderView *horizontalHeader { nullptr };
    Qt::ScrollBarPolicy horizontalScrollBarPolicy { Qt::ScrollBarAsNeeded };
    QAdvancedHeaderView *verticalHeader { nullptr };
    QHeaderView *summaryView { nullptr };
    QSharedItemSelectionModel *splittedViewSelectionModel { nullptr };

    QMap<int, advSummaryFunc> columnsSummaryTypes;
    QMap<int, int> columnSpareWidthParts;
};

// MODERN C++20: RAII guard for batch synchronization operations
class BatchSyncGuard {
public:
    explicit BatchSyncGuard(QAdvancedTableView *view) : v(view), d(view->d)
    {
        ++d->batchUpdateDepth;
    }

    ~BatchSyncGuard()
    {
        --d->batchUpdateDepth;
        // When depth returns to 0, synchronize all columns
        if (d->batchUpdateDepth == 0 && v) {
            v->syncAllColumns();
        }
    }

    // Disable copy and move
    BatchSyncGuard(const BatchSyncGuard &) = delete;
    BatchSyncGuard &operator=(const BatchSyncGuard &) = delete;
    BatchSyncGuard(BatchSyncGuard &&) = delete;
    BatchSyncGuard &operator=(BatchSyncGuard &&) = delete;

private:
    QAdvancedTableView *v;
    QAdvancedTableViewPrivate *d;
};

#endif // QADVANCEDTABLEVIEW_P_H
