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

#include "qtablemodelwriter.h"

#include "qtablemodelcsvwriter_p.h"
#include "qtablemodelexcelmlwriter_p.h"
#include "qtablemodelhtmlwriter_p.h"
#include "qtablemodelwordmlwriter_p.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QTableView>

class QTableModelWriterPrivate {
public:
    explicit QTableModelWriterPrivate(QTableModelWriter *pc);

    bool deleteDevice { true };
    bool includeHeader { true };
    int role { Qt::DisplayRole };
    QIODevice *device { nullptr };
    QTableModelWriter *c { nullptr };
    QByteArray format;
};

QTableModelWriterPrivate::QTableModelWriterPrivate(QTableModelWriter *pc) :
    c(pc)
{
}

QTableModelWriter::QTableModelWriter() :
    d(new QTableModelWriterPrivate(this))
{
}

QTableModelWriter::QTableModelWriter(QIODevice *device, const QByteArray &format) :
    d(new QTableModelWriterPrivate(this))
{
    d->deleteDevice = false;
    d->device = device;
    d->format = format;
}

QTableModelWriter::QTableModelWriter(const QString &fileName, const QByteArray &format) :
    d(new QTableModelWriterPrivate(this))
{
    d->device = new QFile(fileName);
    d->deleteDevice = true;
    d->format = format;
}

/**
 * Destroys the QTableModelWriter object.
 */
QTableModelWriter::~QTableModelWriter()
{
    if (d->deleteDevice) {
        delete d->device;
    }
    delete d;
}

/**
 * Returns the device currently assigned, or 0 if no device has been assigned.
 */
QIODevice *QTableModelWriter::device() const
{
    return d->device;
}

/**
 * If the currently assigned device is a QFile, or if setFileName() has been called, this function returns the name of the file to be written to. In all other cases, it returns an empty string.
 */
QString QTableModelWriter::fileName() const
{
    auto *f = qobject_cast<QFile *>(d->device);
    if (f) {
        return f->fileName();
    }
    return QString();
}

/**
 * Returns the format used for writing documents.
 */
QByteArray QTableModelWriter::format() const
{
    return d->format;
}

/**
 * Sets the writer's device to the device specified. If a device has already been set, the old device is removed but otherwise left unchanged.
 * If the device is not already open, QTableModelWriter will attempt to open the device in QIODevice::WriteOnly mode by calling open().
 * @Note This will not work for certain devices, such as QProcess, QTcpSocket and QUdpSocket, where some configuration is required before the device can be opened.
 */
void QTableModelWriter::setDevice(QIODevice *device)
{
    if (d->device && d->deleteDevice) {
        delete d->device;
    }

    d->device = device;
    d->deleteDevice = false;
}

/**
 * Sets the format used to write documents to the format specified. format is a case insensitive text string. For example:
 * @code
 * QTableModelWriter writer;
 * writer.setFormat("html"); // same as writer.setFormat("HTML");
 * @endcode
 * You can call supportedFormats() for the full list of formats QTableModelWriter supports.
 * @see format().
 */
void QTableModelWriter::setFormat(const QByteArray &format)
{
    d->format = format;
}

void QTableModelWriter::setRole(int role)
{
    d->role = role;
}

/**
 * Returns the list of document formats supported by QTextDocumentWriter.
 * By default, you can write the following formats:
 * <table>
 * <tr><th>Format</th><th>Description</th></tr>
 * <tr><td>CSV</td><td>Comma-separated values</td></tr>
 * <tr><td>HTML</td><td>HyperText Markup Language</td></tr>
 * <tr><td>SpreadsheetML</td><td>MS Excel XML Spreadsheer</td></tr>
 * <tr><td>WordML</td><td>MS Word XML Document</td></tr>
 * </table>
 * @sa setFormat()
 */
QList<QByteArray> QTableModelWriter::supportedFormats()
{
    QList<QByteArray> l;

    l << "CSV";
    l << "HTML";
    l << "SpreadsheetML";
    l << "WordML";
    return l;
}

/**
 * Writes all rows and colums of the specified @p view.
 */
bool QTableModelWriter::writeAll(QAdvancedTableView *view)
{
    return write(view, true);
}

/**
 * Writes all rows and colums of the specified @p view.
 */
bool QTableModelWriter::writeAll(QTableView *view)
{
    return write(view, true);
}

/**
 * Writes the selected cells of the specified @p view.
 */
bool QTableModelWriter::writeSelection(QAdvancedTableView *view)
{
    return write(view, false);
}

/**
 * Writes the selected cells of the specified @p view.
 */
bool QTableModelWriter::writeSelection(QTableView *view)
{
    return write(view, false);
}

/**
 * Writes the selected cells of the specified @p view.
 */
bool QTableModelWriter::write(QAdvancedTableView *view, bool all)
{
    QByteArray suffix;

    if (!d->device) {
        return false;
    }

    if (d->format.isEmpty()) {
        if (auto *file = qobject_cast<QFile *>(d->device)) {
            suffix = QFileInfo(file->fileName()).suffix().toLower().toLatin1();
        }
    }

    QByteArray fmt = !d->format.isEmpty() ? d->format.toLower() : suffix;
    if (fmt == "csv") {
        if (!d->device->isWritable() && !d->device->open(QIODevice::WriteOnly)) {
            qWarning() << "QTableModelWriter::write: the device can not be opened for writing";
            return false;
        }
        QTableModelCsvWriter w(d->device);
        return w.writeAll(view, all);
    }
    if (fmt == "html") {
        if (!d->device->isWritable() && !d->device->open(QIODevice::WriteOnly)) {
            qWarning() << "QTableModelHtmlWriter::write: the device can not be opened for writing";
            return false;
        }
        QTableModelHtmlWriter w(d->device);
        return w.write(view, all);
    }
    if (fmt == "wordml") {
        if (!d->device->isWritable() && !d->device->open(QIODevice::WriteOnly)) {
            qWarning() << "QTableModelWriter::write: the device can not be opened for writing";
            return false;
        }
        QTableModelWordMLWriter w(d->device);
        return w.write(view, all);
    }
    if (fmt == "spreadsheetml") {
        if (!d->device->isWritable() && !d->device->open(QIODevice::WriteOnly)) {
            qWarning() << "QTableModelWriter::write: the device can not be opened for writing";
            return false;
        }
        QTableModelExcelMLWriter w(d->device);
        w.setIncludeHeader(d->includeHeader);
        w.setRole(d->role);
        return w.write(view, all);
    }

    return false;
}

bool QTableModelWriter::write(QTableView *view, bool all)
{
    QByteArray suffix;
    if (!d->device) {
        return false;
    }

    if (d->format.isEmpty()) {
        if (auto *file = qobject_cast<QFile *>(d->device)) {
            suffix = QFileInfo(file->fileName()).suffix().toLower().toLatin1();
        }
    }

    QByteArray fmt = !d->format.isEmpty() ? d->format.toLower() : suffix;
    if (fmt == "csv") {
        if (!d->device->isWritable() && !d->device->open(QIODevice::WriteOnly)) {
            qWarning() << "QTableModelWriter::write: the device can not be opened for writing";
            return false;
        }
        QTableModelCsvWriter w(d->device);
        return w.writeAll(view, all);
    }
    if (fmt == "html") {
        if (!d->device->isWritable() && !d->device->open(QIODevice::WriteOnly)) {
            qWarning() << "QTableModelHtmlWriter::write: the device can not be opened for writing";
            return false;
        }
        QTableModelHtmlWriter w(d->device);
        return w.write(view, all);
    }
    if (fmt == "wordml") {
        if (!d->device->isWritable() && !d->device->open(QIODevice::WriteOnly)) {
            qWarning() << "QTableModelWriter::write: the device can not be opened for writing";
            return false;
        }
        QTableModelWordMLWriter w(d->device);
        return w.write(view, all);
    }
    if (fmt == "spreadsheetml") {
        if (!d->device->isWritable() && !d->device->open(QIODevice::WriteOnly)) {
            qWarning() << "QTableModelWriter::write: the device can not be opened for writing";
            return false;
        }
        QTableModelExcelMLWriter w(d->device);
        w.setIncludeHeader(d->includeHeader);
        w.setRole(d->role);
        return w.write(view, all);
    }

    return false;
}

/**
 * If @p on is true, the table header is included in the output.
 */
void QTableModelWriter::setIncludeHeader(bool on)
{
    d->includeHeader = on;
}
