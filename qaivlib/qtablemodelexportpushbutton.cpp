#include "qtablemodelexportpushbutton.h"

#include <QAction>
#include <QIcon>
#include <QMenu>

QTableModelExportPushButton::QTableModelExportPushButton(QWidget *parent) :
    QPushButton(parent)
{
    setIcon(QIcon(":/qaiv/table.export"));

    QAction *action = nullptr;
    auto *menu = new QMenu(this);
    action = menu->addAction(tr("Excel XML (Selection)"));
    // a->setIcon(QIcon(":/qaiv/export.excel"));
    action->setData(QByteArray("SpreadsheetML"));
    action->setToolTip(tr("Exports the selected items into an Excel XML spreadsheet"));
    connect(action, &QAction::triggered, this, &QTableModelExportPushButton::exportSelectionTriggeredImpl);

    action = menu->addAction(tr("Word XML (Selection)"));
    // a->setIcon(QIcon(":/qaiv/export.word"));
    action->setData(QByteArray("WordML"));
    action->setToolTip(tr("Exports the selected items into an Word XML document"));
    connect(action, &QAction::triggered, this, &QTableModelExportPushButton::exportSelectionTriggeredImpl);

    action = menu->addAction(tr("CSV (Selection)"));
    action->setData(QByteArray("CSV"));
    connect(action, &QAction::triggered, this, &QTableModelExportPushButton::exportSelectionTriggeredImpl);

    action = menu->addAction(tr("HTML (Selection)"));
    action->setData(QByteArray("HTML"));
    connect(action, &QAction::triggered, this, &QTableModelExportPushButton::exportSelectionTriggeredImpl);

    menu->addSeparator();

    action = menu->addAction(tr("Excel XML (All)"));
    action->setData(QByteArray("SpreadsheetML"));
    // a->setIcon(QIcon(":/qaiv/export.excel"));
    connect(action, &QAction::triggered, this, &QTableModelExportPushButton::exportSelectionTriggeredImpl);

    action = menu->addAction(tr("Word XML (All)"));
    // a->setIcon(QIcon(":/qaiv/export.word"));
    action->setData(QByteArray("WordML"));
    connect(action, &QAction::triggered, this, &QTableModelExportPushButton::exportSelectionTriggeredImpl);

    action = menu->addAction(tr("CSV (All)"));
    action->setData(QByteArray("CSV"));
    connect(action, &QAction::triggered, this, &QTableModelExportPushButton::exportSelectionTriggeredImpl);

    action = menu->addAction(tr("HTML (All)"));
    action->setData(QByteArray("HTML"));
    connect(action, &QAction::triggered, this, &QTableModelExportPushButton::exportSelectionTriggeredImpl);

    setMenu(menu);
}

void QTableModelExportPushButton::exportAllTriggeredImpl()
{
    auto *action = qobject_cast<QAction *>(sender());
    if (action) {
        emit exportAllTriggered(action->data().toByteArray());
    }
}

void QTableModelExportPushButton::exportSelectionTriggeredImpl()
{
    auto *action = qobject_cast<QAction *>(sender());
    if (action) {
        emit exportSelectionTriggered(action->data().toByteArray());
    }
}
