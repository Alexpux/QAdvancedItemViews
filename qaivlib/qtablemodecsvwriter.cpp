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
#include "stdafx.h"
#include "qtablemodecsvwriter_p.h"

#include "qadvancedtableview.h"
#include "qmimedatautil.h"

QTableModelCsvWriter::QTableModelCsvWriter(QIODevice* device) :
	m_device(device)
{
}

QTableModelCsvWriter::~QTableModelCsvWriter()
{
}

bool QTableModelCsvWriter::writeAll(QAdvancedTableView* view, bool all)
{
    if (!m_device->isWritable() && ! m_device->open(QIODevice::WriteOnly)) {
        qWarning() << "QTableModelCsvWriter::writeAll: the device can not be opened for writing";
        return false;
    }
	QTextStream stream(m_device);

	QPair<QModelIndex, QModelIndex> e;
	if (!all){
		e = selectionEdges(view->selectionModel()->selection());
	} else {
		e.first = view->model()->index(0, 0);
		e.second = view->model()->index(view->model()->rowCount() - 1, view->model()->columnCount() - 1);
	}
	for (int r = e.first.row(); r <= e.second.row(); r++){
		QStringList l;
		for (int c = e.first.column(); c <= e.second.column(); c++){
			if (!view->horizontalHeader()->isSectionHidden(c)){
				l << "\"" + view->model()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::DisplayRole).toString() + "\"";
			}
		}
		stream << l.join(";") << endl;
	}
	return true;
}

bool QTableModelCsvWriter::writeAll(QTableView* view, bool all)
{
    if (!m_device->isWritable() && ! m_device->open(QIODevice::WriteOnly)) {
        qWarning() << "QTableModelCsvWriter::writeAll: the device can not be opened for writing";
        return false;
    }
	QTextStream stream(m_device);

	QPair<QModelIndex, QModelIndex> e;
	if (!all){
		e = selectionEdges(view->selectionModel()->selection());
	} else {
		e.first = view->model()->index(0, 0);
		e.second = view->model()->index(view->model()->rowCount() - 1, view->model()->columnCount() - 1);
	}
	for (int r = e.first.row(); r <= e.second.row(); r++){
		QStringList l;
		for (int c = e.first.column(); c <= e.second.column(); c++){
			if (!view->horizontalHeader()->isSectionHidden(c)){
				l << "\"" + view->model()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::DisplayRole).toString() + "\"";
			}
		}
		stream << l.join(";") << endl;
	}
	return true;
}
