#include "stdafx.h"
#include "qmimedatautil.h"

#include "qadvancedtableview.h"


void qMimeDataAddCsv(QMimeData* mimeData, QAbstractItemModel* model, QItemSelectionModel* selectionModel, QHeaderView* horizontalHeader, Qt::ItemDataRole role)
{
	if (mimeData == 0 || model == 0 || horizontalHeader == 0){
		return;
	}
	QByteArray d;
	QTextStream stream(&d);
	QPair<QModelIndex, QModelIndex> e = selectionEdges(selectionModel->selection());
	for (int r = e.first.row(); r <= e.second.row(); r++){
		QStringList l;
		for (int c = e.first.column(); c <= e.second.column(); c++){
			if (!horizontalHeader->isSectionHidden(c)){
				l << "\"" + model->index(r, horizontalHeader->visualIndex(c)).data(role).toString() + "\"";
			}
		}
		stream << l.join(";") << endl;
	}
	mimeData->setData("text/csv", d);
}

void qMimeDataAddHtml(QMimeData* mimeData, QAbstractItemModel* model, QItemSelectionModel* selectionModel, QHeaderView* horizontalHeader, Qt::ItemDataRole role)
{
	if (mimeData == 0 || model == 0 || horizontalHeader == 0){
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
	for (int r = e.first.row(); r <= e.second.row(); r++){
		stream.writeStartElement("tr");
		for (int c = e.first.column(); c <= e.second.column(); c++){
			if (!horizontalHeader->isSectionHidden(c)){
				stream.writeTextElement("td", model->index(r, horizontalHeader->visualIndex(c)).data(role).toString());
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

void qMimeDataAddPlainText(QMimeData* mimeData, QAbstractItemModel* model, QItemSelectionModel* selectionModel, QHeaderView* horizontalHeader, Qt::ItemDataRole role)
{
	if (mimeData == 0 || model == 0 || horizontalHeader == 0){
		return;
	}
	QByteArray d;
	QTextStream stream(&d);
	QPair<QModelIndex, QModelIndex> e = selectionEdges(selectionModel->selection());
	for (int r = e.first.row(); r <= e.second.row(); r++){
		QStringList l;
		for (int c = e.first.column(); c <= e.second.column(); c++){
			if (!horizontalHeader->isSectionHidden(c)){
				l << model->index(r, horizontalHeader->visualIndex(c)).data(role).toString();
			}
		}
		stream << l.join("\t") << endl;
	}
	mimeData->setData("text/plain", d);
}

/**
 * @ingroup utils
 * Adds the @view's selection as CSV data to the given QMimeData container @p mimeData.
 */
void qMimeDataAddCsv(QMimeData* mimeData, QAdvancedTableView* view, Qt::ItemDataRole role)
{
	qMimeDataAddCsv(mimeData, view->model(), view->selectionModel(), view->horizontalHeader(), role);
}
/**
 * @ingroup utils
 * Adds the @view's selection as CSV data to the given QMimeData container @p mimeData.
 */
void qMimeDataAddCsv(QMimeData* mimeData, QTableView* view, Qt::ItemDataRole role)
{
	qMimeDataAddCsv(mimeData, view->model(), view->selectionModel(), view->horizontalHeader(), role);
}

/**
 * @ingroup utils
 * Adds the current @view's selection as a HTML table to the given QMimeData container @p mimeData.
 */
void qMimeDataAddHtml(QMimeData* mimeData, QAdvancedTableView* view, Qt::ItemDataRole role)
{
	qMimeDataAddHtml(mimeData, view->model(), view->selectionModel(), view->horizontalHeader(), role);
}

/**
 * @ingroup utils
 * Adds the current @view's selection as a HTML table to the given QMimeData container @p mimeData.
 */
void qMimeDataAddHtml(QMimeData* mimeData, QTableView* view, Qt::ItemDataRole role)
{
	qMimeDataAddHtml(mimeData, view->model(), view->selectionModel(), view->horizontalHeader(), role);
}

/**
 * @ingroup utils
 * Adds the current @view's selection as plain text the given QMimeData container @p mimeData.
 */
void qMimeDataAddPlainText(QMimeData* mimeData, QAdvancedTableView* view, Qt::ItemDataRole role)
{
	qMimeDataAddPlainText(mimeData, view->model(), view->selectionModel(), view->horizontalHeader(), role);
}

/**
 * @ingroup utils
 * Adds the current @view's selection as plain text the given QMimeData container @p mimeData.
 */
void qMimeDataAddPlainText(QMimeData* mimeData, QTableView* view, Qt::ItemDataRole role)
{
	qMimeDataAddPlainText(mimeData, view->model(), view->selectionModel(), view->horizontalHeader(), role);
}

/**
 * @ingroup utils
 * Returns a QModelIndex pair describing the top left and bottom right edges og the given @p selection.
 */
QPair<QModelIndex, QModelIndex> selectionEdges(QItemSelection selection)
{
	QPair<QModelIndex, QModelIndex> p;
	Q_FOREACH(QItemSelectionRange range, selection){
		if (!p.first.isValid()){
			p.first = range.topLeft();
		} else {
			if (range.topLeft() < p.first){
				p.first = range.topLeft();
			}
		}
		if (!p.second.isValid()){
			p.second = range.bottomRight();
		} else if (p.second < range.bottomRight()){
			p.second = range.bottomRight();
		}
	}
	return p;
}
