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
#ifndef QTABLEMODELHTMLWRITER_P_H
#define QTABLEMODELHTMLWRITER_P_H

class QAdvancedTableView;
class QTableView;

class QTableModelHtmlWriter
{
public:
	QTableModelHtmlWriter(QIODevice* device);
	~QTableModelHtmlWriter();

	void setIncludeHeader(bool on);

	bool write(QAdvancedTableView* view, bool all);
	bool write(QTableView* view, bool all);
private:
	void writeAlignment(QXmlStreamWriter & stream, int f);
	void writeBackgroundColor(QXmlStreamWriter & stream, const QBrush & b);
	void writeBorderStyle(QXmlStreamWriter & stream, Qt::PenStyle p);
	void writeCharacters(QXmlStreamWriter & stream, const QString & text);
	void writeDecoration(QXmlStreamWriter & stream, const QVariant & decoration);
	void writeFontAttributes(QXmlStreamWriter & stream, const QFont & font);
	QIODevice* m_device;
	bool m_includeHeader;
};

#endif // QTABLEMODELHTMLWRITER_P_H

