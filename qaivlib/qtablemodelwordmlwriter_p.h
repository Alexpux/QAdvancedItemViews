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
#ifndef QTABLEMODELWORDMLWRITER_P_H
#define QTABLEMODELWORDMLWRITER_P_H

class QAbstractItemModel;
class QAbstractFilterProxyModel;
class QAdvancedTableView;
class QHeaderView;
class QItemSelectionModel;
class QTableView;

class QTableModelWordMLWriter
{
public:
	QTableModelWordMLWriter(QIODevice* device);
	~QTableModelWordMLWriter();

	bool write(QAdvancedTableView* view, bool all);

	bool write(QTableView* view, bool all);

	bool write(QAbstractItemModel* model, QItemSelectionModel* selectionModel, QHeaderView* headerView, bool all);
private:
	void writeBackgroundColor(QXmlStreamWriter & stream, const QBrush & b);
	void writeDecoration(QXmlStreamWriter & stream, const QVariant & decoration);
	void writeFont(QXmlStreamWriter & stream, const QFont & font);

	QIODevice* m_device;
	QAbstractFilterProxyModel* m_filterProxy;
	int m_imageId;
	bool m_includeHeader;
};

#endif // QTABLEMODELWORDMLWRITER_P_H