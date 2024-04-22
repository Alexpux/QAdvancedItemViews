#ifndef QMIMEDATAUTIL_H
#define QMIMEDATAUTIL_H

#include "qaivlib_global.h"

#include <QItemSelection>
#include <Qt>

class QAdvancedTableView;
class QItemSelectionModel;
class QMimeData;
class QTableView;

QAIVLIBSHARED_EXPORT void qMimeDataAddCsv(QMimeData *mimeData, QAdvancedTableView *view, int role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT void qMimeDataAddCsv(QMimeData *mimeData, QTableView *view, int role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT void qMimeDataAddHtml(QMimeData *mimeData, QAdvancedTableView *view, int role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT void qMimeDataAddHtml(QMimeData *mimeData, QTableView *view, int role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT void qMimeDataAddPlainText(QMimeData *mimeData, QAdvancedTableView *view, int role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT void qMimeDataAddPlainText(QMimeData *mimeData, QTableView *view, int role = Qt::DisplayRole);

QAIVLIBSHARED_EXPORT QPair<QModelIndex, QModelIndex> selectionEdges(const QItemSelection &selection);

#endif // QMIMEDATAUTIL_H
