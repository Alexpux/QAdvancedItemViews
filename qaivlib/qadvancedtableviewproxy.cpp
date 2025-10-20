#include "qadvancedtableviewproxy.h"

QAdvancedTableViewProxy::QAdvancedTableViewProxy(QWidget *parent) :
    QTableView(parent)
{
}

void QAdvancedTableViewProxy::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    if (editor->parent()->parent() == this) {
        QTableView::closeEditor(editor, hint);
    }
}

void QAdvancedTableViewProxy::focusInEvent(QFocusEvent *event)
{
    QTableView::focusInEvent(event);
    emit focusReceived();
}

/*void QAdvancedTableViewProxy::mousePressEvent(QMouseEvent *event) {
    QTableView::mousePressEvent(event);
    emit focusReceived();
}*/
