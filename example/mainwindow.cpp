/*
 * This file is part of qadvanceditemviews.
 *
 * Copyright (c) 2011-2012 Martin Hoppe martin@2x2hoppe.de
 *
 * qadvanceditemviews is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any
 * later version.
 *
 * qadvanceditemviews is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with qadvanceditemviews.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QProgressDialog>
#include <QStandardItemModel>
#include <QTime>

#include <qadvancedheaderview.h>
#include <qconditionaldecorationdialog.h>
#include <qconditionaldecorationproxymodel.h>
#include <qfiltermodel.h>
#include <qfiltermodelproxy.h>
#include <qfilterviewconnector.h>
#include <qgroupingproxymodel.h>
#include <qconditionaldecoration.h>
#include <qfixedrowstableview.h>
#include <qrangefilter.h>
#include <quniquevaluesproxymodel.h>
#include <qselectionlistfilter.h>
#include <qtextfilter.h>
#include <qvaluefilter.h>

#include "spinboxitemdelegate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMenu* m = new QMenu(this);
    m->addAction(ui->singleViewAction);
    m->addAction(ui->horizontalSplitAction);
    ui->splitModeToolButton->setMenu(m);
    ui->splitModeToolButton->setDefaultAction(ui->singleViewAction);

    initModel();

    initTabAdvancedTableView();

//    initTabTableView();
    // Tab 3
    initTabUniqueValuesProxyModel();
    // Tab 4
    initTabGroupingProxyModel();
    // Tab 5
    initTabConditionalDecorationProxyModel();
    //
    initTabConditionalDecorationAndGrouping();
    //
    initTabPinTableView();
    //
    initTabLargeTableView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::aboutQtActionTriggered()
{
    QApplication::aboutQt();
}

void MainWindow::decoratedTableViewCustomContextMenuRequested(const QPoint & point)
{
    QModelIndex mIndex = ui->decorationProxyModelTableView->currentIndex();
    QConditionalDecorationDialog* mDlg = new QConditionalDecorationDialog(mIndex, this);
    if (mDlg->exec()){
        QAbstractItemModel* mModel = (QAbstractItemModel*)mIndex.model();
        mModel->setData(mIndex, mDlg->properties(), QConditionalDecorationProxyModel::ConditionalDecorationRole);
    }
    delete mDlg;
}

void MainWindow::decoratedGroupingTreeViewCustomContextMenuRequested(const QPoint &point)
{
    QModelIndex mIndex = ui->decoratedGroupingTreeView->selectionModel()->currentIndex();
    QConditionalDecorationDialog* mDlg = new QConditionalDecorationDialog(mIndex, this);
    if (mDlg->exec()){
        QAbstractItemModel* mModel = (QAbstractItemModel*)mIndex.model();
        mModel->setData(mIndex, mDlg->properties(), QConditionalDecorationProxyModel::ConditionalDecorationRole);
    }
    delete mDlg;
}

void MainWindow::exitActionTriggered()
{
    qApp->exit();
}

void MainWindow::groupWindowsCheckBoxToggled(bool on)
{
    if (on){
        m_groupingProxy->addGroup(QIcon(":/icons/folder"), tr("Windows Developers"), "Windows developers");
    } else {
        m_groupingProxy->removeGroup(m_groupingProxy->findText(tr("Windows Developers")));
    }
}

void MainWindow::groupUnixCheckBoxToggled(bool on)
{
    if (on){
        m_groupingProxy->addGroup(QIcon(":/icons/folder"), tr("Unix Developers"), "Unix developers");
    } else {
        m_groupingProxy->removeGroup(m_groupingProxy->findText(tr("Unix Developers")));
    }
}

void MainWindow::pinRowsToolButtonClicked()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (ui->filterTableView->isFixedRowsModeEnabled()){
        ui->filterTableView->setFixedRowsModeEnabled(false);
        ui->pinRowsToolButton->setIcon(QIcon(":/qaiv/pin/enabled"));
    } else {
        ui->filterTableView->setFixedRowsModeEnabled(true);
        ui->pinRowsToolButton->setIcon(QIcon(":/qaiv/pin/disabled"));
    }
    QApplication::restoreOverrideCursor();
}

void MainWindow::populatePushButtonClicked()
{
    QList<QByteArray> w;
    QFile f(QApplication::applicationDirPath() + "/../lorem_ipsum.txt");
    if (f.open(QIODevice::ReadOnly)){
        w = f.readAll().split(' ');
        f.close();
    }
    QProgressDialog* d = new QProgressDialog(tr("Reading file..."), tr("Cancel"), 0, 30000, this);
    d->setMinimumDuration(0);
    d->setWindowModality(Qt::WindowModal);

    QStandardItemModel* m = new QStandardItemModel(this);
    m->setColumnCount(10);
    QStringList l;
    l << "Lorem ipsum" << "Lorem ipsum"<< "Lorem ipsum"<< "Lorem ipsum"<< "Lorem ipsum"<< "Lorem ipsum"<< "Lorem ipsum"<< "Lorem ipsum"<< "Lorem ipsum"<< "Lorem ipsum";
    m->setHorizontalHeaderLabels(l);
    int wi = 0;
    for (int i = 0; i < 3000 && !d->wasCanceled(); i++){
        d->setValue(wi);
        QList<QStandardItem*> items;
        for (int c = 0; c < 10; c++){
            items << new QStandardItem(QString(w.at(wi)));
            wi++;
        }
        m->appendRow(items);
    }
    delete d;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->populatePushButton->setEnabled(false);
    ui->largeTableView->setModel(m);
    QApplication::restoreOverrideCursor();
}

void MainWindow::splitActionTriggered()
{
    if (sender() == ui->singleViewAction){
        ui->filterTableView->splitView(false);
        ui->splitModeToolButton->setIcon(ui->singleViewAction->icon());
    } else if (sender() == ui->horizontalSplitAction){
        ui->filterTableView->splitView(true);
        ui->splitModeToolButton->setIcon(ui->horizontalSplitAction->icon());
    }
}

void MainWindow::initModel()
{
    m_model = new QStandardItemModel(26,12, this);

    m_model->setHeaderData(0, Qt::Horizontal, tr("Int"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("Qt"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("Qt Group"));
    m_model->setHeaderData(3, Qt::Horizontal, tr("Maturity Level"));
    m_model->setHeaderData(4, Qt::Horizontal, tr("Char (UC)"));
    m_model->setHeaderData(5, Qt::Horizontal, tr("Char (LC)"));
    m_model->setHeaderData(6, Qt::Horizontal, tr("Time"));
    m_model->setHeaderData(7, Qt::Horizontal, tr("Date & Time"));
    m_model->setHeaderData(8, Qt::Horizontal, tr("Duplicate Values"));
    m_model->setHeaderData(9, Qt::Horizontal, tr("Release Date"));
    m_model->setHeaderData(10, Qt::Horizontal, tr("Qt Versions"));
    m_model->setHeaderData(11, Qt::Horizontal, tr("OS"));

    for(int iRows = 0; iRows < m_model->rowCount(); iRows++){
        m_model->setData(m_model->index(iRows, 0), iRows + 1);
        m_model->setData(m_model->index(iRows, 4), QChar(65 + iRows));
        m_model->setData(m_model->index(iRows, 5), QChar(97 + iRows));
        m_model->setData(m_model->index(iRows, 6), QTime::currentTime().addSecs(iRows * 10));
        m_model->setData(m_model->index(iRows, 7), QDateTime::currentDateTime().addDays(iRows));
    }

    for(int iRows = 0; iRows < m_model->rowCount(); iRows += 2){
        m_model->setData(m_model->index(iRows, 8), QChar(65 + iRows));
        m_model->setData(m_model->index(iRows + 1, 8), QChar(65 + iRows));
    }
    m_model->setData(m_model->index(0, 1), "QtCore");
    m_model->setData(m_model->index(0, 2), "general software development");
    m_model->setData(m_model->index(0, 3), "Active");

    m_model->setData(m_model->index(1, 1), "QtGui");
    m_model->setData(m_model->index(1, 2), "general software development");
    m_model->setData(m_model->index(1, 3), "Active");

    m_model->setData(m_model->index(2 ,1), "QtMultimedia");
    m_model->setData(m_model->index(2, 2), "general software development");
    m_model->setData(m_model->index(2, 3), "Deprecated");

    m_model->setData(m_model->index(3, 1), "QtNetwork");
    m_model->setData(m_model->index(3, 2), "general software development");
    m_model->setData(m_model->index(3, 3), "Active");

    m_model->setData(m_model->index(4, 1), "QtOpenGL");
    m_model->setData(m_model->index(4, 2), "general software development");
    m_model->setData(m_model->index(4, 3), "Maintained");

    m_model->setData(m_model->index(5, 1), "QtOpenVG");
    m_model->setData(m_model->index(5, 2), "general software development");
    m_model->setData(m_model->index(6, 1), "QtScript");
    m_model->setData(m_model->index(6, 2), "general software development");
    m_model->setData(m_model->index(6, 3), "Active");

    m_model->setData(m_model->index(7, 1), "QtScriptTools");
    m_model->setData(m_model->index(7, 2), "general software development");
    m_model->setData(m_model->index(8, 1), "QtSql");
    m_model->setData(m_model->index(8, 3), "Done");

    m_model->setData(m_model->index(8, 2), "general software development");
    m_model->setData(m_model->index(9, 1), "QtSvg");
    m_model->setData(m_model->index(9, 2), "general software development");
    m_model->setData(m_model->index(9, 3), "Deprecated");

    m_model->setData(m_model->index(10, 1), "QtWebKit");
    m_model->setData(m_model->index(10, 2), "general software development");
    m_model->setData(m_model->index(10, 3), "Active");

    m_model->setData(m_model->index(11, 1), "QtXml");
    m_model->setData(m_model->index(11, 2), "general software development");
    m_model->setData(m_model->index(11, 3), "Done");

    m_model->setData(m_model->index(12, 1), "QtXmlPatterns");
    m_model->setData(m_model->index(12, 2), "general software development");
    m_model->setData(m_model->index(12, 3), "Done");

    m_model->setData(m_model->index(13, 1), "QtDeclarative");
    m_model->setData(m_model->index(13, 2), "general software development");
    m_model->setData(m_model->index(14, 1), "Phonon");
    m_model->setData(m_model->index(14, 2), "general software development");

    m_model->setData(m_model->index(15, 1), "Qt3Support");
    m_model->setData(m_model->index(15, 2), "general software development");
    m_model->setData(m_model->index(15, 3), "Deprecated");

    m_model->setData(m_model->index(16, 1), "QtDesigner");
    m_model->setData(m_model->index(16, 2), "Qt's tools");
    m_model->setData(m_model->index(16, 3), "Done");

    m_model->setData(m_model->index(17, 1), "QtUiTools");
    m_model->setData(m_model->index(17, 2), "Qt's tools");
    m_model->setData(m_model->index(18, 1), "QtHelp");
    m_model->setData(m_model->index(18, 2), "Qt's tools");
    m_model->setData(m_model->index(19, 1), "QtTest");
    m_model->setData(m_model->index(19, 2), "Qt's tools");
    m_model->setData(m_model->index(20, 1), "QAxContainer");
    m_model->setData(m_model->index(20, 2), "Windows developers");
    m_model->setData(m_model->index(21, 1), "QAxServer");
    m_model->setData(m_model->index(21, 2), "Windows developers");
    m_model->setData(m_model->index(21, 1), "QtDBus");
    m_model->setData(m_model->index(21, 2), "Unix developers");
    m_model->setData(m_model->index(22, 1), "C++");
    m_model->setData(m_model->index(23, 1), "JavaScript");


    m_model->setData(m_model->index(0, 9), QDate(1998, 7, 10));
    m_model->setData(m_model->index(0, 10), "1.40");

    m_model->setData(m_model->index(1, 9), QDate(1998, 10, 2));
    m_model->setData(m_model->index(1, 10), "1.41");

    m_model->setData(m_model->index(2, 9), QDate(1998, 12, 19));
    m_model->setData(m_model->index(2, 10), "1.42");

    m_model->setData(m_model->index(3, 9), QDate(1999, 3, 13));
    m_model->setData(m_model->index(3, 10), "1.44");

    m_model->setData(m_model->index(4, 9), QDate(1999, 6, 26));
    m_model->setData(m_model->index(4, 10), "2.0");

    m_model->setData(m_model->index(5, 9), QDate(2000, 4, 13));
    m_model->setData(m_model->index(5, 10), "2.1");

    m_model->setData(m_model->index(6, 9), QDate(2000, 12, 7));
    m_model->setData(m_model->index(6, 10), "2.2");

    m_model->setData(m_model->index(7, 9), QDate(2001, 3, 8));
    m_model->setData(m_model->index(7, 10), "2.3");

    m_model->setData(m_model->index(8, 9), QDate(2001, 10, 16));
    m_model->setData(m_model->index(8, 10), "3");

    m_model->setData(m_model->index(9, 9), QDate(2001, 11, 14));
    m_model->setData(m_model->index(9, 10), "3.1");

    m_model->setData(m_model->index(10, 9), QDate(2003, 7, 24));
    m_model->setData(m_model->index(10, 10), "3.2");

    m_model->setData(m_model->index(11, 9), QDate(2004, 2, 5));
    m_model->setData(m_model->index(11, 10), "3.3");

    m_model->setData(m_model->index(12, 9), QDate(2005, 12, 20));
    m_model->setData(m_model->index(12, 10), "4.1");

    m_model->setData(m_model->index(13, 9), QDate(2006, 10, 4));
    m_model->setData(m_model->index(13, 10), "4.2");

    m_model->setData(m_model->index(14, 9), QDate(2007, 5, 30));
    m_model->setData(m_model->index(14, 10), "4.3");

    m_model->setData(m_model->index(15, 9), QDate(2008, 5, 6));
    m_model->setData(m_model->index(15, 10), "4.4");

    m_model->setData(m_model->index(16, 9), QDate(2009, 3, 3));
    m_model->setData(m_model->index(16, 10), "4.5");

    m_model->setData(m_model->index(17, 9), QDate(2009, 12, 1));
    m_model->setData(m_model->index(17, 10), "4.6.0");

    m_model->setData(m_model->index(18, 9), QDate(2010, 9, 21));
    m_model->setData(m_model->index(18, 10), "4.7");

    m_model->setData(m_model->index(19, 9), QDate(2010, 11, 9));
    m_model->setData(m_model->index(19, 10), "4.7.1");

    m_model->setData(m_model->index(20, 9), QDate(2011, 3, 1));
    m_model->setData(m_model->index(20, 10), "4.7.2");

    m_model->setData(m_model->index(21, 9), QDate(2011, 5, 4));
    m_model->setData(m_model->index(21, 10), "4.7.3");

    m_model->setData(m_model->index(22, 9), QDate(2011, 9, 1));
    m_model->setData(m_model->index(22, 10), "4.7.4");

    m_model->setData(m_model->index(23, 9), QDate(2011, 12, 15));
    m_model->setData(m_model->index(23, 10), "4.8");
    // Columns OS
    m_model->setData(m_model->index(0, 11), "Windows");
    m_model->setData(m_model->index(1, 11), "Windows CE");
    m_model->setData(m_model->index(2, 11), "Mac OS X");
    m_model->setData(m_model->index(3, 11), "Linux");
    m_model->setData(m_model->index(4, 11), "Embedded Linux");
}

void MainWindow::initTabAdvancedTableView()
{

    ui->filterTableView->setModel(m_model);
    ui->filterTableView->horizontalHeader()->setClickable(true);

    // ------------------------------------
    // Column 'Int'
    // ------------------------------------
    QVariantList ftypes;
    ftypes << QRangeFilter::Type << QValueFilter::Type;
    ui->filterTableView->setColumnFilterTypes(0, ftypes);
    // ------------------------------------
    // Column 'Qt': All filter types allowed. The default filter type is set to Text Filter
    // ------------------------------------
    ui->filterTableView->setDefaultFilterType(1, QTextFilter::Type);
    // ------------------------------------
    // Column 'Maturity Level'
    // ------------------------------------
    ui->filterTableView->setFilterType(QSelectionListFilter::Type, 3);
    QSelectionListFilter* selectionListFilter = qabstractfilter_cast<QSelectionListFilter*>(ui->filterTableView->filterAt(0, 3));
    if (selectionListFilter){
        QVariantList values;
        values << "Deprecated" << "Done" << "Maintained";
        selectionListFilter->setValues(values);
        selectionListFilter->setEnabled(false);
    }

    ui->filterTableView->setItemDelegateForColumn(0, new SpinBoxDelegate(this));
}

void MainWindow::initTabConditionalDecorationProxyModel()
{
    QConditionalDecorationProxyModel* proxy = new QConditionalDecorationProxyModel(this);
    //
    QConditionalDecoration* decoration = new QConditionalDecoration(3);
    decoration->addCondition(QConditionalDecoration::IsEqual, "Active", "leds", "green (on)");
    decoration->addCondition(QConditionalDecoration::IsEqual, "Deprecated", "leds", "red (on)");
    decoration->addCondition(QConditionalDecoration::IsEqual, "Done", "leds", "yellow (on)");
    decoration->addCondition(QConditionalDecoration::IsEqual, "Maintained", "leds", "orange (on)");
    decoration->setDefaultIcon("leds", "white (on)");
    proxy->addDecoration(3, decoration);

    decoration = new QConditionalDecoration();
    decoration->addCondition(QConditionalDecoration::Contains, "Linux", "emoticon", "happy");
    decoration->addCondition(QConditionalDecoration::Contains, "Mac", "emoticon", "neutral");
    decoration->addCondition(QConditionalDecoration::Contains, "Windows", "emoticon", "unhappy");
    proxy->addDecoration(11, decoration);

    proxy->setSourceModel(m_model);
    ui->decorationProxyModelTableView->setModel(proxy);
}

void MainWindow::initTabGroupingProxyModel()
{
    ui->groupingTableView->setHorizontalHeader(new QAdvancedHeaderView(Qt::Horizontal, ui->groupingTableView));
    ui->groupingTreeView->setHeader(new QAdvancedHeaderView(Qt::Horizontal, ui->groupingTreeView));

    m_groupingProxy = new QGroupingProxyModel(this);
    m_groupingProxy->setModelColumn(2);
    m_groupingProxy->addGroup(QIcon(":/icons/folder"), "general software development");
    m_groupingProxy->addGroup(QIcon(":/icons/folder"), "Qt's Tools", "Qt's tools");
    m_groupingProxy->setSourceModel(m_model);

    m_groupingProxy->setData(m_groupingProxy->index(0, 0), QIcon(":/icons/folder"), Qt::DecorationRole);

    QSortFilterProxyModel* proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(m_groupingProxy);
    //
    ui->groupingTreeView->setModel(proxy);
    ui->groupingTableView->setModel(m_model);
}

void MainWindow::initTabConditionalDecorationAndGrouping()
{
    ui->groupingTableView->setHorizontalHeader(new QAdvancedHeaderView(Qt::Horizontal, ui->groupingTableView));
    ui->groupingTreeView->setHeader(new QAdvancedHeaderView(Qt::Horizontal, ui->groupingTreeView));

    m_groupingProxy = new QGroupingProxyModel(this);
    m_groupingProxy->setModelColumn(2);
    m_groupingProxy->addGroup(QIcon(":/icons/folder"), "general software development");
    m_groupingProxy->addGroup(QIcon(":/icons/folder"), "Qt's Tools", "Qt's tools");
    m_groupingProxy->setSourceModel(m_model);

    QConditionalDecorationProxyModel* decorationProxyModel = new QConditionalDecorationProxyModel(this);
    decorationProxyModel->setSourceModel(m_groupingProxy);
    //
    ui->decoratedGroupingTreeView->setModel(decorationProxyModel);
}

void MainWindow::initTabTableView()
{
//    QFilterModel* filterModel = new QFilterModel(this);
//    QFilterModelProxy* filterModelProxy = new QFilterModelProxy(this);

//    ui->tableFilterView->connectToView(ui->tableView);

//    filterModelProxy->setFilterModel(filterModel);
//    filterModelProxy->setSourceModel(m_model);

//    ui->tableFilterView->setModel(filterModel);
    //    ui->tableView->setModel(filterModelProxy);
}

void MainWindow::initTabPinTableView()
{
//    QPinnedRowsFilterProxyModel* p = new QPinnedRowsFilterProxyModel(this);
//    p->setSourceModel(m_model);
//    ui->pinnedFilterTableView->setModel(p);
    //    p->pinRow(0);
}

void MainWindow::initTabLargeTableView()
{
//    QStandardItemModel* model = new QStandardItemModel(this);
//    ui->largeTableView->setModel(model);
    ui->largeTableView->setFixedRowsModeEnabled(true);
}

void MainWindow::initTabUniqueValuesProxyModel()
{
    QUniqueValuesProxyModel* uniqueValuesProxyModel = new QUniqueValuesProxyModel(this);
    ui->uniqueValuesTableView->setModel(uniqueValuesProxyModel);
    uniqueValuesProxyModel->setModelColumn(8);
    uniqueValuesProxyModel->setSourceModel(m_model);
}
