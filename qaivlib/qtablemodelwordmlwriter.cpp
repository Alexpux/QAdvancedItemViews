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
#include "qtablemodelwordmlwriter_p.h"

#include "qabstractfilterproxymodel.h"
#include "qadvancedtableview.h"
#include "qmimedatautil.h"

QTableModelWordMLWriter::QTableModelWordMLWriter(QIODevice* device)
: m_device(device), m_filterProxy(0), m_imageId(1)
{
}

QTableModelWordMLWriter::~QTableModelWordMLWriter()
{
}

bool QTableModelWordMLWriter::write(QTableView* view, bool all)
{
	return write(view->model(), view->selectionModel(), view->horizontalHeader(), all);
}

bool QTableModelWordMLWriter::write(QAdvancedTableView* view, bool all)
{
	m_filterProxy = view->filterProxyModel();
	return write(view->filterProxyModel(), view->selectionModel(), view->horizontalHeader(), all);
}

bool QTableModelWordMLWriter::write(QAbstractItemModel* model, QItemSelectionModel* selectionModel, QHeaderView* headerView, bool all)
{
    if (!m_device->isWritable() && ! m_device->open(QIODevice::WriteOnly)) {
        qWarning() << "QTableModelXmlWriter::writeAll: the device can not be opened for writing";
        return false;
    }
	QXmlStreamWriter stream(m_device);
	stream.setAutoFormatting(true);
	stream.setAutoFormattingIndent(2);

	stream.writeStartDocument("1.0", true);
	stream.writeProcessingInstruction("mso-application progid=\"Word.Document\"");
	stream.writeStartElement("w:wordDocument");
	//
	stream.writeAttribute("xmlns:w", "http://schemas.microsoft.com/office/word/2003/wordml");
	stream.writeAttribute("xmlns:v", "urn:schemas-microsoft-com:vml");
	//stream.writeAttribute("xmlns:w10", "urn:schemas-microsoft-com:office:word");
	//stream.writeAttribute("xmlns:sl", "http://schemas.microsoft.com/schemaLibrary/2003/core");
	//stream.writeAttribute("xmlns:aml", "http://schemas.microsoft.com/aml/2001/core");
	stream.writeAttribute("xmlns:wx", "http://schemas.microsoft.com/office/word/2003/auxHint");
	stream.writeAttribute("xmlns:o", "urn:schemas-microsoft-com:office:office");
	//stream.writeAttribute("xmlns:dt", "uuid:C2F41010-65B3-11d1-A29F-00AA00C14882");
	//stream.writeAttribute("w:macrosPresent", "no");
	//stream.writeAttribute("w:embeddedObjPresent", "no");
	//stream.writeAttribute("w:ocxPresent", "no");
	//stream.writeAttribute("xml:space","preserve");

	//stream.writeEmptyElement("w:ignoreSubtree");
	//stream.writeAttribute("w:val", "http://schemas.microsoft.com/office/word/2003/wordml/sp2");
	// document properties
	stream.writeStartElement("o:DocumentProperties");
	stream.writeTextElement("o:Author", "qaiv");
	stream.writeTextElement("o:LastAuthor", "qaiv");
		//<o:Revision>2</o:Revision>
		//<o:TotalTime>0</o:TotalTime>
		//<o:Created>2013-08-23T16:21:00Z</o:Created>
		//<o:LastSaved>2013-08-23T16:21:00Z</o:LastSaved>
	//stream.writeTextElement("o:Pages", "1");
		//<o:Words>1</o:Words>
		//<o:Characters>12</o:Characters>
		//<o:Lines>1</o:Lines>
		//<o:Paragraphs>1</o:Paragraphs>
		//<o:CharactersWithSpaces>12</o:CharactersWithSpaces>
	stream.writeTextElement("o:Version", "14");

	stream.writeEndElement();
	//// fonts
	//stream.writeStartElement("w:fonts");
	//stream.writeStartElement("w:defaultFonts");
 //   stream.writeAttribute("w:ascii", "Times New Roman");
 //   stream.writeAttribute("w:fareast", "Times New Roman");
 //   stream.writeAttribute("w:h-ansi", "Times New Roman");
 //   stream.writeAttribute("w:cs", "Times New Roman");
	//stream.writeEndElement();
	//stream.writeEndElement();
	//// lists
	//stream.writeStartElement("lists");

	//stream.writeEndElement();
	//// styles
	//stream.writeStartElement("styles");

	//stream.writeEndElement();
	// start body
	stream.writeStartElement("w:body");

	stream.writeStartElement("w:tbl");

	stream.writeStartElement("w:tblPr");
	// define table borders
	stream.writeStartElement("w:tblBorders");
	stream.writeEmptyElement("w:top");
	stream.writeAttribute("w:val", "single");
	stream.writeAttribute("w:sz", "4");
	stream.writeAttribute("wx:bdrwidth", "10");
	stream.writeAttribute("w:space", "0");
	stream.writeAttribute("w:color", "auto");

	stream.writeEmptyElement("w:left");
	stream.writeAttribute("w:val", "single");
	stream.writeAttribute("w:sz", "4");
	stream.writeAttribute("wx:bdrwidth", "10");
	stream.writeAttribute("w:space", "0");
	stream.writeAttribute("w:color", "auto");


	stream.writeEmptyElement("w:bottom");
	stream.writeAttribute("w:val", "single");
	stream.writeAttribute("w:sz", "4");
	stream.writeAttribute("wx:bdrwidth", "10");
	stream.writeAttribute("w:space", "0");
	stream.writeAttribute("w:color", "auto");

	stream.writeEmptyElement("w:right");
	stream.writeAttribute("w:val", "single");
	stream.writeAttribute("w:sz", "4");
	stream.writeAttribute("wx:bdrwidth", "10");
	stream.writeAttribute("w:space", "0");
	stream.writeAttribute("w:color", "auto");

	stream.writeEmptyElement("w:insideH");
	stream.writeAttribute("w:val", "single");
	stream.writeAttribute("w:sz", "4");
	stream.writeAttribute("wx:bdrwidth", "10");
	stream.writeAttribute("w:space", "0");
	stream.writeAttribute("w:color", "auto");

	stream.writeEmptyElement("w:insideV");
	stream.writeAttribute("w:val", "single");
	stream.writeAttribute("w:sz", "4");
	stream.writeAttribute("wx:bdrwidth", "10");
	stream.writeAttribute("w:space", "0");
	stream.writeAttribute("w:color", "auto");

	stream.writeEndElement();
	//

	stream.writeEmptyElement("w:tblStyle");
	stream.writeAttribute("w:val", "TableGrid");
	stream.writeEmptyElement("w:tblW");
	stream.writeAttribute("w:w", "0");
	stream.writeAttribute("w:type", "auto");
    stream.writeEmptyElement("w:tblLook");
	stream.writeAttribute("w:val", "01E0");
	stream.writeEndElement();

	// get selection range
	QPair<QModelIndex, QModelIndex> e;
	if (!all){
		e = selectionEdges(selectionModel->selection());
	} else {
		e.first = model->index(0, 0);
		e.second = model->index(model->rowCount() - 1, model->columnCount() - 1);
	}

	QMap<int,int> width;

	stream.writeStartElement("w:tblGrid");
	for (int c = e.first.column(); c <= e.second.column(); c++){
		if (all || !headerView->isSectionHidden(c)){
			int w = 1440 / headerView->logicalDpiX() * headerView->sectionSize(c);
			stream.writeEmptyElement("w:gridCol");
			stream.writeAttribute("w:w", QString("%1").arg(w));
			width[c] = w;
		}
	}
	stream.writeEndElement();
	// write column header
	stream.writeStartElement("w:tr");
	// write column header
	for (int c = e.first.column(); c <= e.second.column(); c++){
		if (all || !headerView->isSectionHidden(c)){
			stream.writeStartElement("w:trPr");
			stream.writeEmptyElement("w:tblHeader");
			stream.writeEndElement();
			stream.writeStartElement("w:tc");

			stream.writeStartElement("w:tcPr");
			stream.writeEmptyElement("w:tcW");
			stream.writeAttribute("w:w", QString("%1").arg(width.value(c)));
			stream.writeAttribute("w:type", "dxa");
			stream.writeEmptyElement("w:shd");
			stream.writeAttribute("w:val", "clear");
			stream.writeAttribute("w:color", "auto");
			writeBackgroundColor(stream, qvariant_cast<QBrush>(model->headerData(headerView->visualIndex(c), Qt::Horizontal, Qt::BackgroundRole)));
			// end tag <w:tcPr>
			stream.writeEndElement();

			stream.writeStartElement("w:p");
			stream.writeStartElement("w:r");
			stream.writeStartElement("w:rPr");
			writeFont(stream, qvariant_cast<QFont>(model->headerData(headerView->visualIndex(c), Qt::Horizontal, Qt::FontRole)));
			// end tag <w:rPr>
			stream.writeEndElement();
			stream.writeTextElement("w:t", model->headerData(headerView->visualIndex(c), Qt::Horizontal, Qt::DisplayRole).toString());
			// end tag <w:r>
			stream.writeEndElement();
			// end tag <w:p>
			stream.writeEndElement();
			// end tag <tc>
			stream.writeEndElement();
		}
	}
	// end tag <w:tr>
	stream.writeEndElement();
	// write table content
	for (int r = e.first.row(); r <= e.second.row(); r++){
		stream.writeStartElement("w:tr");
		for (int c = e.first.column(); c <= e.second.column(); c++){
			if (all || !headerView->isSectionHidden(c)){
				stream.writeStartElement("w:tc");

				stream.writeStartElement("w:tcPr");
				stream.writeEmptyElement("w:tcW");
				stream.writeAttribute("w:w", QString("%1").arg(width.value(c)));
				stream.writeAttribute("w:type", "dxa");
				stream.writeEmptyElement("w:shd");
				stream.writeAttribute("w:val", "clear");
				stream.writeAttribute("w:color", "auto");
				//stream.writeAttribute("w:fill", "auto");
				if (m_filterProxy){
					writeBackgroundColor(stream, qvariant_cast<QBrush>(m_filterProxy->index(r, headerView->visualIndex(c)).data(Qt::BackgroundRole)));
				} else {
					writeBackgroundColor(stream, qvariant_cast<QBrush>(model->index(r, headerView->visualIndex(c)).data(Qt::BackgroundRole)));
				}
				// end tag <w:tcPr>
				stream.writeEndElement();

				stream.writeStartElement("w:p");
				stream.writeStartElement("w:r");
				stream.writeStartElement("w:rPr");
				if (m_filterProxy){
					writeFont(stream, qvariant_cast<QFont>(m_filterProxy->index(r, headerView->visualIndex(c)).data(Qt::FontRole)));
				} else {
					writeFont(stream, qvariant_cast<QFont>(model->index(r, headerView->visualIndex(c)).data(Qt::FontRole)));
				}
				// end tag <w:rPr>
				stream.writeEndElement();

				writeDecoration(stream, model->index(r, headerView->visualIndex(c)).data(Qt::DecorationRole));
				// write cell content
				stream.writeTextElement("w:t", model->index(r, headerView->visualIndex(c)).data(Qt::DisplayRole).toString());
				//writeAlignment(stream, static_cast<Qt::AlignmentFlag>(view->model()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::TextAlignmentRole).toInt()));
				//writeBorderStyle(stream, view->gridStyle());
				//writeBackgroundColor(stream, qvariant_cast<QBrush>(view->filterProxyModel()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::BackgroundRole)));
				//writeDecoration(stream, view->model()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::DecorationRole));
				//stream.writeStartElement("font");
				//writeFontAttributes(stream, qvariant_cast<QFont>(view->model()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::FontRole)));
				//writeCharacters(stream, view->model()->index(r, view->horizontalHeader()->visualIndex(c)).data(Qt::DisplayRole).toString());
				// end tag <w:r>
				stream.writeEndElement();
				// end tag <w:p>
				stream.writeEndElement();
				// end tag <tc>
				stream.writeEndElement();
			}
		}
		// end tag <tr>
		stream.writeEndElement();
	}
	// end tag <tbl>
	stream.writeEndElement();
	// end tag <sect>
	//stream.writeEndElement();
	// end tag <body>
	stream.writeEndElement();
	// end tag <wordDocument>
	stream.writeEndElement();
	return true;
}

void QTableModelWordMLWriter::writeBackgroundColor(QXmlStreamWriter & stream, const QBrush & b)
{
	if (b.style() != Qt::NoBrush){
		//stream.writeEmptyElement("w:color");
		stream.writeAttribute("w:fill", b.color().name());
	}
}

void QTableModelWordMLWriter::writeDecoration(QXmlStreamWriter & stream, const QVariant & decoration)
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
	stream.writeStartElement("w:pict");
	// export image data
	stream.writeStartElement("w:binData");
	stream.writeAttribute("w:name", QString("wordml://%1.png").arg(m_imageId, 8, 10, QLatin1Char('0')));
	stream.writeAttribute("xml:space", "preserve");
	stream.writeCharacters(bytes.toBase64());
	stream.writeEndElement();
	//
	stream.writeStartElement("v:shape");
	stream.writeAttribute("style", QString("width:%1px;height:%2px").arg(pixmap.size().width()).arg(pixmap.size().height()));
	stream.writeEmptyElement("v:imageData");
	stream.writeAttribute("src", QString("wordml://%1.png").arg(m_imageId, 8, 10, QLatin1Char('0')));

	// end tag <w:shape>
	stream.writeEndElement();
	// end tag <w:pict>
	stream.writeEndElement();

	m_imageId++;
}

void QTableModelWordMLWriter::writeFont(QXmlStreamWriter & stream, const QFont & font)
{
	QFont f(font);
	if (f.family() == "MS Shell Dlg 2"){
		f.setFamily("Arial");
	}
	stream.writeEmptyElement("w:rFonts");
	stream.writeAttribute("w:ascii", f.family());
	stream.writeAttribute("w:h-ansi", f.family());
	stream.writeAttribute("w:cs", f.family());
	stream.writeEmptyElement("wx:font");
	stream.writeAttribute("wx:val", f.family());

	stream.writeEmptyElement("w:sz");
	stream.writeAttribute("w:val", QString("%1").arg(f.pointSize() * 2));
	stream.writeEmptyElement("w:sz-cs");
	stream.writeAttribute("w:val", QString("%1").arg(f.pointSize() * 2));
}
