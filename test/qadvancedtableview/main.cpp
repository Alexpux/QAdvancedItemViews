#include <QtTest/QtTest>

#include <QStandardItemModel>

#include <qadvancedtableview.h>
#include <qabstractfilter.h>
#include <qtextfilter.h>

class QAdvancedTableViewTest: public QObject
{
	Q_OBJECT
private slots:
	void clicked();
	void currentIndex();
	void doubleClicked();
	void indexAt();
	void initTestCase();
	void model();
	void pressed();
	void selectionModel();
	void setFilterType();
	void viewport();

	void hideColumn();
private:
	QAdvancedTableView* m_view;
	QStandardItemModel* m_model;
};

void QAdvancedTableViewTest::clicked()
{
	QSignalSpy spy(m_view, SIGNAL(clicked(QModelIndex)));
	QTest::mouseClick(m_view->viewport(), Qt::LeftButton, 0, QPoint(20, 20));
	QCOMPARE(spy.count(), 1);
}

void QAdvancedTableViewTest::currentIndex()
{
	//QModelIndex i = m_view->model()->index(0, 1);
	//m_view->setCurrentIndex(i);
	//QModelIndex c = m_view->currentIndex();
	//QCOMPARE(m_view->currentIndex(), i);
}

void QAdvancedTableViewTest::doubleClicked()
{
	QSignalSpy spy(m_view, SIGNAL(doubleClicked(QModelIndex)));
	QTest::mouseDClick(m_view->viewport(), Qt::LeftButton, 0, QPoint(20, 20));
	QCOMPARE(spy.count(), 1);
}

void QAdvancedTableViewTest::indexAt()
{
	QModelIndex i = m_view->indexAt(QPoint(20,20));
	QCOMPARE(i, m_view->model()->index(0, 0));
}

void QAdvancedTableViewTest::model()
{
	QCOMPARE(m_view->model(), m_model);
}

void QAdvancedTableViewTest::pressed()
{
	QSignalSpy spy(m_view, SIGNAL(pressed(QModelIndex)));
	QTest::mousePress(m_view->viewport(), Qt::LeftButton, 0, QPoint(20, 20));
	QCOMPARE(spy.count(), 1);
}

void QAdvancedTableViewTest::selectionModel()
{
	m_view->selectAll();
	QCOMPARE(m_view->selectionModel()->selectedRows().size(), m_view->model()->rowCount());
	for (int i = 0; i < m_view->model()->rowCount(); i++){
		m_view->selectionModel()->select(m_view->selectionModel()->model()->index(0,0), QItemSelectionModel::Deselect);
		QCOMPARE(m_view->selectionModel()->selectedRows().size(), m_view->model()->rowCount() - 1);
	}
}

void QAdvancedTableViewTest::setFilterType()
{
    m_view->setFilterType(QTextFilter::Type, 1);
    QTextFilter* f = qabstractfilter_cast<QTextFilter*>(m_view->filterAt(0, 1));
	QVERIFY(f != 0);
}

void QAdvancedTableViewTest::viewport()
{
	m_view->viewport()->setMouseTracking(true);
	QSignalSpy spy(m_view, SIGNAL(viewportEntered()));
	QTest::mouseMove(m_view->viewport());
	QCOMPARE(spy.count(), 1);
	m_view->viewport()->setMouseTracking(false);
}

void QAdvancedTableViewTest::hideColumn()
{
	m_view->hideColumn(0);
	QCOMPARE(m_view->isColumnHidden(0), true);
}

void QAdvancedTableViewTest::initTestCase()
{
	m_view = new QAdvancedTableView(0);
	m_view->show();

	m_model = new QStandardItemModel(this);
	m_model->setColumnCount(2);

	QStandardItem* item;
	QList<QStandardItem*> items;

	item = new QStandardItem("0, 0");
	item->setEnabled(true);
	item->setSelectable(true);
	items << item;
	item = new QStandardItem("0, 1");
	item->setEnabled(true);
	item->setSelectable(true);
	items << item;
	m_model->appendRow(items);

	items.clear();
	item = new QStandardItem("1, 0");
	item->setEnabled(true);
	item->setSelectable(true);
	items << item;
	item = new QStandardItem("1, 1");
	item->setEnabled(true);
	item->setSelectable(true);
	items << item;
	m_model->appendRow(items);

	m_view->setModel(m_model);
}

QTEST_MAIN(QAdvancedTableViewTest)
#include "main.moc"
