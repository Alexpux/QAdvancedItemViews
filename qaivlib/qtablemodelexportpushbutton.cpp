#include "stdafx.h"
#include "qtablemodelexportpushbutton.h"

QTableModelExportPushButton::QTableModelExportPushButton(QWidget *parent)
	: QPushButton(parent)
{
	setIcon(QIcon(":/qaiv/table.export"));

	QAction *a = 0;
	QMenu* m = new QMenu(this);
	a = m->addAction(tr("Excel XML (Selection)"));
	//a->setIcon(QIcon(":/qaiv/export.excel"));
	a->setData(QByteArray("SpreadsheetML"));
	a->setToolTip(tr("Exports the selected items into an Excel XML spreadsheet"));
	connect(a, SIGNAL(triggered()), this, SLOT(exportSelectionTriggered()));

	a = m->addAction(tr("Word XML (Selection)"));
	//a->setIcon(QIcon(":/qaiv/export.word"));
	a->setData(QByteArray("WordML"));
	a->setToolTip(tr("Exports the selected items into an Word XML document"));
	connect(a, SIGNAL(triggered()), this, SLOT(exportSelectionTriggered()));

	a = m->addAction(tr("CSV (Selection)"));
	a->setData(QByteArray("CSV"));
	connect(a, SIGNAL(triggered()), this, SLOT(exportSelectionTriggered()));

	m->addSeparator();

	a = m->addAction(tr("Excel XML (All)"));
	a->setData(QByteArray("SpreadsheetML"));
	//a->setIcon(QIcon(":/qaiv/export.excel"));
	connect(a, SIGNAL(triggered()), this, SLOT(exportAllTriggered()));

	a = m->addAction(tr("Word XML (All)"));
	//a->setIcon(QIcon(":/qaiv/export.word"));
	a->setData(QByteArray("WordML"));
	connect(a, SIGNAL(triggered()), this, SLOT(exportAllTriggered()));

	a = m->addAction(tr("CSV (All)"));
	a->setData(QByteArray("CSV"));
	connect(a, SIGNAL(triggered()), this, SLOT(exportAllTriggered()));

	setMenu(m);
}

QTableModelExportPushButton::~QTableModelExportPushButton()
{

}

void QTableModelExportPushButton::exportAllTriggered()
{
	QAction* a = qobject_cast<QAction*>(sender());
	if (a){
		emit exportAllTriggered(a->data().toByteArray());
	}
}

void QTableModelExportPushButton::exportSelectionTriggered()
{
	QAction* a = qobject_cast<QAction*>(sender());
	if (a){
		emit exportSelectionTriggered(a->data().toByteArray());
	}
}
