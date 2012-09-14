#include <QtTest/QtTest>

#include <QStandardItemModel>

#include <qaiv.h>
#include <qadvancedtableview.h>
#include <quniquevaluesproxymodel.h>

class QAIVTest: public QObject
{
	Q_OBJECT
private slots:
	void qsourcemodel_cast();

	void qSourceIndex();
	
	void qSourceModel();

	void initTestCase();
private:
	QAdvancedTableView* m_view;
	QStandardItemModel* m_model;
	QUniqueValuesProxyModel* m_proxy;
};

void QAIVTest::qsourcemodel_cast()
{
	QCOMPARE(m_model, ::qsourcemodel_cast<QStandardItemModel*>(m_view->model()));
}

void QAIVTest::qSourceIndex()
{
	QModelIndex sindex = m_model->index(0 ,0);
	QModelIndex pindex = ::qSourceIndex(m_proxy->index(0, 0));
	QCOMPARE(sindex, pindex);
}

void QAIVTest::qSourceModel()
{
	QCOMPARE(m_model, ::qSourceModel(m_proxy->index(0, 0)));
}

void QAIVTest::initTestCase()
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

	m_proxy = new QUniqueValuesProxyModel(this);
	m_proxy->setSourceModel(m_model);
	m_view->setModel(m_proxy);
}

QTEST_MAIN(QAIVTest)
#include "main.moc"
