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
#ifndef QTABLEMODELWRITER_H
#define QTABLEMODELWRITER_H

#include <qaivlib_global.h>

class QAdvancedTableView;
class QTableModelWriterPrivate;
class TableView;

//! The QTableModelWriter class provides a format-independent interface for writing the content of table views to files or other devices.
/**
 * To write the content of a table, construct a QTableModelWriter object with either a file name or a device object, and specify the format to be written.
 * Call writeAll() to write the content to the device. If the content is successfully written, this function returns true. However, if an error occurs when writing the content, it will return false.
 * @code
 * QTableModelWriter writer("c:/temp/qaiv.html");
 * writer.writeAll(ui->filterTableView);
 * @endcode
 * You can use writeSelection() write the table view's current selection to the device.
 * Additionally, you can construct a writer and set the format using setFormat() later.
 * Call supportedDocumentFormats() for a list of formats that QTableModelWriter can write.
 */
class QAIVLIBSHARED_EXPORT QTableModelWriter
{
public:
	/**
	 * Constructs an empty QTableModelWriter object. Before writing, you must call setFormat() to set a document format, then setDevice() or setFileName().
	 */
	QTableModelWriter();
	/**
	 * Constructs a QTableModelWriter object to write to the given @p device in the document format specified by @p format.
	 */
	QTableModelWriter(QIODevice* device, const QByteArray & format);
	/**
	 * Constructs an QTableModelWriter object that will write to a file with the name fileName, using the document format specified by format. If format is not provided, QTableModelWriter will detect the document format by inspecting the extension of @p fileName.
	 */
	QTableModelWriter(const QString & fileName, const QByteArray & format = QByteArray());

	~QTableModelWriter();

	QIODevice* device() const;

	QString fileName() const;

	QByteArray format() const;

	void setDevice(QIODevice* device);

	void setFormat(const QByteArray & format);

	void setIncludeHeader(bool on);

	static QList<QByteArray> supportedFormats();

	bool writeAll(QAdvancedTableView* view);

	bool writeAll(QTableView* view);

	bool writeSelection(QAdvancedTableView* view);
private:
	Q_DISABLE_COPY(QTableModelWriter);
	bool write(QAdvancedTableView* view, bool all);
	bool write(QTableView* view, bool all);

	QTableModelWriterPrivate* d;
};

#endif // QTABLEMODELWRITER_H
