#include "qtablemodelexportpushbutton.h"

#include <QIcon>
#include <QAction>
#include <QMenu>

QTableModelExportPushButton::QTableModelExportPushButton(QWidget *parent)
    : QPushButton(parent)
{
    setIcon(QIcon(":/qaiv/table.export"));

    QAction *action = nullptr;
    QMenu* menu = new QMenu(this);
    action = menu->addAction(tr("Excel XML (Selection)"));
    //a->setIcon(QIcon(":/qaiv/export.excel"));
    action->setData(QByteArray("SpreadsheetML"));
    action->setToolTip(tr("Exports the selected items into an Excel XML spreadsheet"));
    connect(action, &QAction::triggered, [this](){ exportSelectionTriggeredImpl(); });

    action = menu->addAction(tr("Word XML (Selection)"));
    //a->setIcon(QIcon(":/qaiv/export.word"));
    action->setData(QByteArray("WordML"));
    action->setToolTip(tr("Exports the selected items into an Word XML document"));
    connect(action, &QAction::triggered, [this](){ exportSelectionTriggeredImpl(); });

    action = menu->addAction(tr("CSV (Selection)"));
    action->setData(QByteArray("CSV"));
    connect(action, &QAction::triggered, [this](){ exportSelectionTriggeredImpl(); });

    action = menu->addAction(tr("HTML (Selection)"));
    action->setData(QByteArray("HTML"));
    connect(action, &QAction::triggered, [this](){ exportSelectionTriggeredImpl(); });

    menu->addSeparator();

    action = menu->addAction(tr("Excel XML (All)"));
    action->setData(QByteArray("SpreadsheetML"));
    //a->setIcon(QIcon(":/qaiv/export.excel"));
    connect(action, &QAction::triggered, [this](){ exportSelectionTriggeredImpl(); });

    action = menu->addAction(tr("Word XML (All)"));
    //a->setIcon(QIcon(":/qaiv/export.word"));
    action->setData(QByteArray("WordML"));
    connect(action, &QAction::triggered, [this](){ exportSelectionTriggeredImpl(); });

    action = menu->addAction(tr("CSV (All)"));
    action->setData(QByteArray("CSV"));
    connect(action, &QAction::triggered, [this](){ exportSelectionTriggeredImpl(); });

    action = menu->addAction(tr("HTML (All)"));
    action->setData(QByteArray("HTML"));
    connect(action, &QAction::triggered, [this](){ exportSelectionTriggeredImpl(); });

    setMenu(menu);
}

QTableModelExportPushButton::~QTableModelExportPushButton()
{

}

void QTableModelExportPushButton::exportAllTriggeredImpl()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action) {
        emit exportAllTriggered(action->data().toByteArray());
    }
}

void QTableModelExportPushButton::exportSelectionTriggeredImpl()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action) {
        emit exportSelectionTriggered(action->data().toByteArray());
    }
}
