/******************************************************************************
** This file is part of qadvanceditemviews.
**
** Copyright (c) 2011-2013 Martin Hoppe martin@2x2hoppe.de
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

#include "qabstractfilterproxymodel.h"
#include "qadvancedtableview.h"
#include "qmimedatautil.h"
#include "qtablemodelcsvwriter_p.h"

#include <QDebug>
#include <QIODevice>
#include <QTableView>
#include <QTextStream>

QTableModelCsvWriter::QTableModelCsvWriter(QIODevice *device) :
    m_device(device)
{
}

bool QTableModelCsvWriter::writeAll(QAdvancedTableView *view, bool all)
{
    if (!m_device->isWritable() && !m_device->open(QIODevice::WriteOnly)) {
        qWarning() << "QTableModelCsvWriter::writeAll: the device can not be opened for writing";
        return false;
    }
    QTextStream stream(m_device);

    QPair<QModelIndex, QModelIndex> e;
    auto *proxyModel = view->filterProxyModel();
    auto *hHeader = view->horizontalHeader();
    if (!all) {
        e = selectionEdges(view->selectionModel()->selection());
    } else {
        e.first = proxyModel->index(0, 0);
        e.second = proxyModel->index(proxyModel->rowCount() - 1, proxyModel->columnCount() - 1);
    }
    for (int r = e.first.row(); r <= e.second.row(); r++) {
        QStringList l;
        for (int c = e.first.column(); c <= e.second.column(); c++) {
            if (!hHeader->isSectionHidden(c)) {
                l << "\"" + proxyModel->index(r, hHeader->visualIndex(c)).data(Qt::DisplayRole).toString() + "\"";
            }
        }
        stream << l.join(";") << ENDL;
    }
    return true;
}

bool QTableModelCsvWriter::writeAll(QTableView *view, bool all)
{
    if (!m_device->isWritable() && !m_device->open(QIODevice::WriteOnly)) {
        qWarning() << "QTableModelCsvWriter::writeAll: the device can not be opened for writing";
        return false;
    }
    QTextStream stream(m_device);

    QPair<QModelIndex, QModelIndex> e;
    auto *viewModel = view->model();
    auto *hHeader = view->horizontalHeader();
    if (!all) {
        e = selectionEdges(view->selectionModel()->selection());
    } else {
        e.first = viewModel->index(0, 0);
        e.second = viewModel->index(viewModel->rowCount() - 1, viewModel->columnCount() - 1);
    }
    for (int r = e.first.row(); r <= e.second.row(); r++) {
        QStringList l;
        for (int c = e.first.column(); c <= e.second.column(); c++) {
            if (!hHeader->isSectionHidden(c)) {
                l << "\"" + viewModel->index(r, hHeader->visualIndex(c)).data(Qt::DisplayRole).toString() + "\"";
            }
        }
        stream << l.join(";") << ENDL;
    }
    return true;
}
