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
#include "qtablemodelhtmlwriter_p.h"

#include "qabstractfilterproxymodel.h"
#include "qadvancedtableview.h"
#include "qmimedatautil.h"

QTableModelHtmlWriter::QTableModelHtmlWriter(QIODevice* device)
: m_device(device), m_includeHeader(true)
{
}

QTableModelHtmlWriter::~QTableModelHtmlWriter()
{
}

void QTableModelHtmlWriter::setIncludeHeader(bool on)
{
	m_includeHeader = on;
}

bool QTableModelHtmlWriter::write(QAdvancedTableView* view, bool all)
{
    if (!m_device->isWritable() && ! m_device->open(QIODevice::WriteOnly)) {
        qWarning() << "QTableModelHtmlWriter::writeAll: the device can not be opened for writing";
        return false;
    }
	QXmlStreamWriter stream(m_device);
	stream.setAutoFormatting(true);
	stream.setAutoFormattingIndent(2);

	stream.writeStartElement("html");
	stream.writeStartElement("head");
	stream.writeEndElement();

	stream.writeStartElement("body");
	stream.writeStartElement("table");
	stream.writeAttribute("border", view->showGrid()?"1":"0");
	if (view->showGrid()){
		stream.writeAttribute("style", "border-style:none");
	}
	if (m_includeHeader){
		// start tag <tr>
		stream.writeStartElement("tr");
		writeBorderStyle(stream, view->gridStyle());
		for (int c = 0; c < view->horizontalHeader()->count(); c++){
			if (all || !view->horizontalHeader()->isSectionHidden(c)){
				stream.writeStartElement("th");
				writeAlignment(stream, static_cast<Qt::AlignmentFlag>(view->model()->headerData(view->horizontalHeader()->visualIndex(c), Qt::Horizontal, Qt::TextAlignmentRole).toInt()));
				stream.writeStartElement("font");
				writeFontAttributes(stream, qvariant_cast<QFont>(view->model()->headerData(view->horizontalHeader()->visualIndex(c), Qt::Horizontal, Qt::FontRole)));
				stream.writeCharacters(view->model()->headerData(view->horizontalHeader()->visualIndex(c), Qt::Horizontal, Qt::DisplayRole).toString());
				stream.writeEndElement();
				// end tag <th>
				stream.writeEndElement();
			}
		}
		// end tag <tr>
		stream.writeEndElement();
	}
	//
	QPair<QModelIndex, QModelIndex> e;
	if (!all){
		e = selectionEdges(view->selectionModel()->selection());
	} else {
		e.first = view->model()->index(0, 0);
		e.second = view->model()->index(view->model()->rowCount() - 1, view->model()->columnCount() - 1);
	}
	for (int r = e.first.row(); r <= e.second.row(); r++){
		stream.writeStartElement("tr");
		for (int c = e.first.column(); c <= e.second.column(); c++){
			if (!view->horizontalHeader()->isSectionHidden(c)){
				stream.writeStartElement("td");
				writeAlignment(stream, static_cast<Qt::AlignmentFlag>(view->model()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::TextAlignmentRole).toInt()));
				writeBorderStyle(stream, view->gridStyle());
				writeBackgroundColor(stream, qvariant_cast<QBrush>(view->filterProxyModel()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::BackgroundRole)));
				writeDecoration(stream, view->model()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::DecorationRole));
				stream.writeStartElement("font");
				writeFontAttributes(stream, qvariant_cast<QFont>(view->model()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::FontRole)));
				writeCharacters(stream, view->model()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::DisplayRole).toString());
				stream.writeEndElement();
				// end tag <td>
				stream.writeEndElement();
			}
		}
		// end tag <tr>
		stream.writeEndElement();
	}

	// end tag <table>
	stream.writeEndElement();
	// end tag <body>
	stream.writeEndElement();
	// end tag <html>
	stream.writeEndElement();
	return true;
}

bool QTableModelHtmlWriter::write(QTableView* view, bool all)
{
    if (!m_device->isWritable() && ! m_device->open(QIODevice::WriteOnly)) {
        qWarning() << "QTableModelHtmlWriter::writeAll: the device can not be opened for writing";
        return false;
    }
	QXmlStreamWriter stream(m_device);
	stream.setAutoFormatting(true);
	stream.setAutoFormattingIndent(2);

	stream.writeStartElement("html");
	stream.writeStartElement("head");
	stream.writeEndElement();

	stream.writeStartElement("body");
	stream.writeStartElement("table");
	stream.writeAttribute("border", view->showGrid()?"1":"0");
	if (view->showGrid()){
		stream.writeAttribute("style", "border-style:none");
	}
	if (m_includeHeader){
		// start tag <tr>
		stream.writeStartElement("tr");
		writeBorderStyle(stream, view->gridStyle());
		for (int c = 0; c < view->horizontalHeader()->count(); c++){
			if (all || !view->horizontalHeader()->isSectionHidden(c)){
				stream.writeStartElement("th");
				writeAlignment(stream, static_cast<Qt::AlignmentFlag>(view->model()->headerData(view->horizontalHeader()->visualIndex(c), Qt::Horizontal, Qt::TextAlignmentRole).toInt()));
				stream.writeStartElement("font");
				writeFontAttributes(stream, qvariant_cast<QFont>(view->model()->headerData(view->horizontalHeader()->visualIndex(c), Qt::Horizontal, Qt::FontRole)));
				stream.writeCharacters(view->model()->headerData(view->horizontalHeader()->visualIndex(c), Qt::Horizontal, Qt::DisplayRole).toString());
				stream.writeEndElement();
				// end tag <th>
				stream.writeEndElement();
			}
		}
		// end tag <tr>
		stream.writeEndElement();
	}
	//
	QPair<QModelIndex, QModelIndex> e;
	if (!all){
		e = selectionEdges(view->selectionModel()->selection());
	} else {
		e.first = view->model()->index(0, 0);
		e.second = view->model()->index(view->model()->rowCount() - 1, view->model()->columnCount() - 1);
	}
	for (int r = e.first.row(); r <= e.second.row(); r++){
		stream.writeStartElement("tr");
		for (int c = e.first.column(); c <= e.second.column(); c++){
			if (!view->horizontalHeader()->isSectionHidden(c)){
				stream.writeStartElement("td");
				writeAlignment(stream, static_cast<Qt::AlignmentFlag>(view->model()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::TextAlignmentRole).toInt()));
				writeBorderStyle(stream, view->gridStyle());
				writeBackgroundColor(stream, qvariant_cast<QBrush>(view->model()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::BackgroundRole)));
				writeDecoration(stream, view->model()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::DecorationRole));
				stream.writeStartElement("font");
				writeFontAttributes(stream, qvariant_cast<QFont>(view->model()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::FontRole)));
				writeCharacters(stream, view->model()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::DisplayRole).toString());
				stream.writeEndElement();

				// end tag <td>
				stream.writeEndElement();
			}
		}
		// end tag <tr>
		stream.writeEndElement();
	}

	// end tag <table>
	stream.writeEndElement();
	// end tag <body>
	stream.writeEndElement();
	// end tag <html>
	stream.writeEndElement();
	return true;
}

void QTableModelHtmlWriter::writeAlignment(QXmlStreamWriter & stream, int f)
{
	if (f & Qt::AlignBottom){
		stream.writeAttribute("valign", "bottom");
	} else if (f & Qt::AlignHCenter){
		stream.writeAttribute("align", "center");
	} else if (f & Qt::AlignJustify){
		stream.writeAttribute("align", "justify");
	} else if (f & Qt::AlignLeft){
		stream.writeAttribute("align", "left");
	} else if (f & Qt::AlignRight){
		stream.writeAttribute("align", "right");
	} else if (f & Qt::AlignTop){
		stream.writeAttribute("valign", "top");
	} else if (f & Qt::AlignVCenter){
		stream.writeAttribute("valign", "middle");
	}
}

void QTableModelHtmlWriter::writeBackgroundColor(QXmlStreamWriter & stream, const QBrush & b)
{
	if (b.style() != Qt::NoBrush){
		stream.writeAttribute("bgcolor", b.color().name());
	}
}

void QTableModelHtmlWriter::writeBorderStyle(QXmlStreamWriter & stream, Qt::PenStyle p)
{
	if (p == Qt::DashLine){
		stream.writeAttribute("style", "border-style:dashed");
	} else if (p == Qt::DotLine){
		stream.writeAttribute("style", "border-style:dotted");
	} else if (p == Qt::NoPen){
		stream.writeAttribute("style", "border-style:none");
	} else {
		stream.writeAttribute("style", "border-style:solid");
	}
}

void QTableModelHtmlWriter::writeCharacters(QXmlStreamWriter & stream, const QString & text)
{
	if (text.isEmpty()){
		stream.writeCharacters(QChar(QChar::Nbsp));
	} else {
		stream.writeCharacters(text);
	}
}

void QTableModelHtmlWriter::writeDecoration(QXmlStreamWriter & stream, const QVariant & decoration)
{
	QIcon icon = qvariant_cast<QIcon>(decoration);
	QPixmap pixmap;
	if (icon.isNull()){
		pixmap = qvariant_cast<QPixmap>(decoration);
		if (pixmap.isNull()){
			return;
		}
	} else {
		pixmap = icon.pixmap(16, 16);
	}
	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	pixmap.save(&buffer, "PNG"); // writes pixmap into bytes in PNG format
	stream.writeStartElement("img");
	stream.writeAttribute("src", QString("data:image/png;base64,%1").arg(bytes.toBase64().data()));
	stream.writeEndElement();

}

void QTableModelHtmlWriter::writeFontAttributes(QXmlStreamWriter & stream, const QFont & font)
{
	QFontInfo i(font);
	stream.writeAttribute("face", i.family());
	//stream.writeAttribute("size", QString("%1").arg(i.pixelSize()));
}
