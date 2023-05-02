#include "qmimedatautil.h"

#include "qaiv.h"
#include "qadvancedtableview.h"
#include "qabstractfilterproxymodel.h"

#include <QMimeData>
#include <QTableView>
#include <QTextStream>
#include <QXmlStreamWriter>

void qMimeDataAddCsv(QMimeData* mimeData, QAbstractItemModel* model, QItemSelectionModel* selectionModel, QHeaderView* horizontalHeader, QAbstractFilterProxyModel* proxy, int role)
{
    if (mimeData == 0 || model == 0 || horizontalHeader == 0) {
        return;
    }
    QByteArray d;
    QTextStream stream(&d);
    QPair<QModelIndex, QModelIndex> e = selectionEdges(selectionModel->selection());
    for (int r = e.first.row(); r <= e.second.row(); r++) {
        QStringList l;
        for (int c = e.first.column(); c <= e.second.column(); c++) {
            if (!horizontalHeader->isSectionHidden(c)) {
                QModelIndex index;
                if (proxy == 0) {
                    index = qSourceIndex(model->index(r, horizontalHeader->visualIndex(c)));
                } else {
                    index = proxy->mapToSource(proxy->index(r, horizontalHeader->visualIndex(c)));
                }
                l << "\"" + index.data(role).toString() + "\"";
            }
        }
        stream << l.join(";") << Qt::endl;
    }
    mimeData->setData("text/csv", d);
}

void qMimeDataAddHtml(QMimeData* mimeData, QAbstractItemModel* model, QItemSelectionModel* selectionModel, QHeaderView* horizontalHeader, QAbstractFilterProxyModel* proxy, int role)
{
    if (mimeData == 0 || model == 0 || horizontalHeader == 0) {
        return;
    }
    QByteArray d;
    QXmlStreamWriter stream(&d);
    stream.setAutoFormatting(true);
    // start html
    stream.writeStartElement("html");
    // start html header
    stream.writeStartElement("head");
    // close tag head
    stream.writeEndElement();
    // start html body
    stream.writeStartElement("body");
    stream.writeStartElement("table");

    QPair<QModelIndex, QModelIndex> e = selectionEdges(selectionModel->selection());
    for (int r = e.first.row(); r <= e.second.row(); r++) {
        stream.writeStartElement("tr");
        for (int c = e.first.column(); c <= e.second.column(); c++) {
            if (!horizontalHeader->isSectionHidden(c)) {
                QModelIndex index;
                if (proxy == 0) {
                    index = qSourceIndex(model->index(r, horizontalHeader->visualIndex(c)));
                } else {
                    index = proxy->mapToSource(proxy->index(r, horizontalHeader->visualIndex(c)));
                }
                stream.writeStartElement("td");
                QString fragment = index.data(role).toString();
                if (fragment.left(6) != "<html>") {
                    fragment = QString("<html>%1</html>").arg(fragment);
                }
                QXmlStreamReader fr(fragment);
                bool inhibit = false;
                while (!fr.atEnd()) {
                    fr.readNext();
                    if (fr.tokenType() == QXmlStreamReader::Characters) {
                        if (!inhibit) {
                            if (fr.isCDATA()) {
                                stream.writeCDATA(fr.text().toString());
                            } else if (fr.isComment()) {
                                stream.writeComment(fr.text().toString());
                            } else {
                                stream.writeCharacters(fr.text().toString());
                            }
                        }
                    } else if (fr.tokenType() == QXmlStreamReader::Comment) {
                        stream.writeComment(fr.text().toString());
                    } else if (fr.tokenType() == QXmlStreamReader::EndElement) {
                        if (fr.name() == QLatin1String("html")) {
                        } else if (fr.name() == QLatin1String("body")) {
                        } else {
                            if (fr.name() == QLatin1String("head")) {
                                inhibit = false;
                            } else {
                                stream.writeEndElement();
                            }
                        }
                    } else if (fr.tokenType() == QXmlStreamReader::StartElement) {
                        if (fr.name() == QLatin1String("html")) {
                        } else if (fr.name() == QLatin1String("body")) {
                        } else {
                            if (fr.name() == QLatin1String("head")) {
                                inhibit = true;
                            }
                            if (!inhibit) {
                                stream.writeStartElement(fr.name().toString());
                                stream.writeAttributes(fr.attributes());
                            }
                        }
                    }
                }
                // close tag <td>
                stream.writeEndElement();
                //stream.writeTextElement("td", model->index(r, horizontalHeader->visualIndex(c)).data(role).toString());
            }
        }
        // close tag tr
        stream.writeEndElement();
    }
    // </table>
    stream.writeEndElement();
    // </body>
    stream.writeEndElement();
    // </html>
    stream.writeEndElement();
    mimeData->setData("text/html", d);
}

void qMimeDataAddPlainText(QMimeData* mimeData, QAbstractItemModel* model, QItemSelectionModel* selectionModel, QHeaderView* horizontalHeader, QAbstractFilterProxyModel* proxy, int role)
{
    if (mimeData == 0 || model == 0 || horizontalHeader == 0) {
        return;
    }
    QByteArray d;
    QTextStream stream(&d);
    QPair<QModelIndex, QModelIndex> e = selectionEdges(selectionModel->selection());
    for (int r = e.first.row(); r <= e.second.row(); r++) {
        QStringList l;
        for (int c = e.first.column(); c <= e.second.column(); c++) {
            if (!horizontalHeader->isSectionHidden(c)) {
                QModelIndex index;
                if (proxy == 0) {
                    index = qSourceIndex(model->index(r, horizontalHeader->visualIndex(c)));
                } else {
                    index = proxy->mapToSource(proxy->index(r, horizontalHeader->visualIndex(c)));
                }
                l << index.data(role).toString();
            }
        }
        stream << l.join("\t") << Qt::endl;
    }
    mimeData->setData("text/plain", d);
}

/**
 * @ingroup utils
 * Adds the @view's selection as CSV data to the given QMimeData container @p mimeData.
 */
void qMimeDataAddCsv(QMimeData* mimeData, QAdvancedTableView* view, int role)
{
    qMimeDataAddCsv(mimeData, view->model(), view->selectionModel(), view->horizontalHeader(), view->filterProxyModel(), role);
}
/**
 * @ingroup utils
 * Adds the @view's selection as CSV data to the given QMimeData container @p mimeData.
 */
void qMimeDataAddCsv(QMimeData* mimeData, QTableView* view, int role)
{
    qMimeDataAddCsv(mimeData, view->model(), view->selectionModel(), view->horizontalHeader(), 0, role);
}

/**
 * @ingroup utils
 * Adds the current @view's selection as a HTML table to the given QMimeData container @p mimeData.
 */
void qMimeDataAddHtml(QMimeData* mimeData, QAdvancedTableView* view, int role)
{
    qMimeDataAddHtml(mimeData, view->model(), view->selectionModel(), view->horizontalHeader(), view->filterProxyModel(), role);
}

/**
 * @ingroup utils
 * Adds the current @view's selection as a HTML table to the given QMimeData container @p mimeData.
 */
void qMimeDataAddHtml(QMimeData* mimeData, QTableView* view, int role)
{
    qMimeDataAddHtml(mimeData, view->model(), view->selectionModel(), view->horizontalHeader(), 0, role);
}

/**
 * @ingroup utils
 * Adds the current @view's selection as plain text the given QMimeData container @p mimeData.
 */
void qMimeDataAddPlainText(QMimeData* mimeData, QAdvancedTableView* view, int role)
{
    qMimeDataAddPlainText(mimeData, view->model(), view->selectionModel(), view->horizontalHeader(), view->filterProxyModel(), role);
}

/**
 * @ingroup utils
 * Adds the current @view's selection as plain text the given QMimeData container @p mimeData.
 */
void qMimeDataAddPlainText(QMimeData* mimeData, QTableView* view, int role)
{
    qMimeDataAddPlainText(mimeData, view->model(), view->selectionModel(), view->horizontalHeader(), 0, role);
}

/**
 * @ingroup utils
 * Returns a QModelIndex pair describing the top left and bottom right edges og the given @p selection.
 */
QPair<QModelIndex, QModelIndex> selectionEdges(QItemSelection selection)
{
    QPair<QModelIndex, QModelIndex> p;
    for (QItemSelectionRange range : selection) {
        if (!p.first.isValid()) {
            p.first = range.topLeft();
        } else {
            if (range.topLeft() < p.first) {
                p.first = range.topLeft();
            }
        }
        if (!p.second.isValid()) {
            p.second = range.bottomRight();
        } else if (p.second < range.bottomRight()) {
            p.second = range.bottomRight();
        }
    }
    return p;
}
