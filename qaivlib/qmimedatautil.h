#ifndef QMIMEDATAUTIL_H
#define QMIMEDATAUTIL_H

#include <Qt>
#include <QItemSelection>

#include "qaivlib_global.h"

class QAdvancedTableView;
class QItemSelectionModel;
class QMimeData;
class QTableView;

QAIVLIBSHARED_EXPORT void qMimeDataAddCsv(QMimeData* mimeData, QAdvancedTableView* view, int role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT void qMimeDataAddCsv(QMimeData* mimeData, QTableView* view, int role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT void qMimeDataAddHtml(QMimeData* mimeData, QAdvancedTableView* view, int role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT void qMimeDataAddHtml(QMimeData* mimeData, QTableView* view, int role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT void qMimeDataAddPlainText(QMimeData* mimeData, QAdvancedTableView* view, int role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT void qMimeDataAddPlainText(QMimeData* mimeData, QTableView* view, int role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT QPair<QModelIndex, QModelIndex> selectionEdges(QItemSelection selection);

#endif // QMIMEDATAUTIL_H
