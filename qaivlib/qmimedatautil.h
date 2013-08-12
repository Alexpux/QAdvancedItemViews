#ifndef QMIMEDATAUTIL_H
#define QMIMEDATAUTIL_H

#include <qaivlib_global.h>

class QItemSelectionModel;
class QMimeData;
class QAdvancedTableView;

QAIVLIBSHARED_EXPORT void qMimeDataAddCsv(QMimeData* mimeData, QAdvancedTableView* view, Qt::ItemDataRole role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT void qMimeDataAddCsv(QMimeData* mimeData, QTableView* view, Qt::ItemDataRole role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT void qMimeDataAddHtml(QMimeData* mimeData, QAdvancedTableView* view, Qt::ItemDataRole role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT void qMimeDataAddHtml(QMimeData* mimeData, QTableView* view, Qt::ItemDataRole role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT void qMimeDataAddPlainText(QMimeData* mimeData, QAdvancedTableView* view, Qt::ItemDataRole role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT void qMimeDataAddPlainText(QMimeData* mimeData, QTableView* view, Qt::ItemDataRole role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT QPair<QModelIndex, QModelIndex> selectionEdges(QItemSelection selection);

#endif // QMIMEDATAUTIL_H
