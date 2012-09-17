#include <QtTest/QtTest>

#include <QMetaType>
#include <QModelIndex>
#include <QSignalSpy>
#include <QStandardItemModel>

#include <qaiv.h>
#include <qadvancedtableview.h>
#include <qcheckstateproxymodel.h>
#include <quniquevaluesproxymodel.h>

Q_DECLARE_METATYPE(QModelIndex)

class QTestCase : public QObject
{
	Q_OBJECT
public:
	QStandardItemModel* model() const
	{
		return m_model;
	};
protected:
	void initModel();
private:
	QStandardItemModel* m_model;
};

void QTestCase::initModel()
{
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

	items.clear();
	item = new QStandardItem("2, 0");
	item->setEnabled(true);
	item->setSelectable(true);
	items << item;
	item = new QStandardItem("2, 1");
	item->setEnabled(true);
	item->setSelectable(true);
	items << item;
	m_model->appendRow(items);
};

class QCheckStateProxyModelTestCase : public QTestCase
{
	Q_OBJECT
public:
	QCheckStateProxyModel* proxy() const
	{
		return m_proxy;
	};
private slots:
	void cleanup();
	void data();
	void init();
	void initTestCase();
	void isChecked();
	void rowCount();
	void setAllChecked();
	void setChecked();
	void setCheckedIndexes();
	void setColumnCheckable();
	void setSourceModel();
private:
	QAdvancedTableView* m_view;
	QCheckStateProxyModel* m_proxy;
};

void QCheckStateProxyModelTestCase::cleanup()
{
	delete m_proxy;
	m_proxy = 0;
}

void QCheckStateProxyModelTestCase::data()
{
	QSignalSpy spy(proxy(), SIGNAL(dataChanged(QModelIndex, QModelIndex)));
	QCOMPARE(proxy()->index(1, 1).data().toString(), QString("1, 1"));
	proxy()->setColumnCheckable(1);
	proxy()->setData(proxy()->index(1, 1), Qt::Checked, Qt::CheckStateRole);
	QCOMPARE(proxy()->index(1, 1).data(Qt::CheckStateRole).toInt(), int(Qt::Checked));

	proxy()->setData(proxy()->index(1, 1), Qt::Unchecked, Qt::CheckStateRole);
	QCOMPARE(proxy()->index(1, 1).data(Qt::CheckStateRole).toInt(), int(Qt::Unchecked));

	QCOMPARE(spy.count(), 2);
}

void QCheckStateProxyModelTestCase::init()
{
	m_proxy = new QCheckStateProxyModel(this);
	m_proxy->setSourceModel(model());
}

void QCheckStateProxyModelTestCase::initTestCase()
{
	qRegisterMetaType<QModelIndex>("QModelIndex");

	initModel();

	m_view = new QAdvancedTableView(0);
	m_view->show();
}

void QCheckStateProxyModelTestCase::isChecked()
{
	proxy()->setColumnCheckable(0);
	proxy()->setChecked(proxy()->index(0, 0), true);
	QVERIFY(proxy()->isChecked(proxy()->index(0, 0))); 
}

void QCheckStateProxyModelTestCase::rowCount()
{
	QCOMPARE(proxy()->rowCount(), model()->rowCount());
}

void QCheckStateProxyModelTestCase::setAllChecked()
{
	QSignalSpy spy(proxy(), SIGNAL(dataChanged(QModelIndex, QModelIndex)));

	proxy()->setColumnCheckable(0);
	proxy()->setColumnCheckable(1);
	proxy()->setAllChecked();
	QCOMPARE(proxy()->checkedIndexes().size(), proxy()->rowCount() * proxy()->checkableColumnsCount());
	proxy()->setAllChecked(false);
	QCOMPARE(proxy()->checkedIndexes().size(), 0);

	QCOMPARE(spy.count(), 2);
	QCOMPARE(qvariant_cast<QModelIndex>(spy.at(0).at(0)), proxy()->index(0, 0));
	QCOMPARE(qvariant_cast<QModelIndex>(spy.at(0).at(1)), proxy()->index(2, 1));
	QCOMPARE(qvariant_cast<QModelIndex>(spy.at(1).at(0)), proxy()->index(0, 0));
	QCOMPARE(qvariant_cast<QModelIndex>(spy.at(1).at(1)), proxy()->index(2, 1));
}

void QCheckStateProxyModelTestCase::setChecked()
{
	QSignalSpy spy(proxy(), SIGNAL(dataChanged(QModelIndex, QModelIndex)));
	proxy()->setColumnCheckable(0);
	proxy()->setChecked(proxy()->index(0, 0), true);
	QVERIFY(proxy()->isChecked(proxy()->index(0, 0)));
	QCOMPARE(spy.count(), 1);
	QCOMPARE(qvariant_cast<QModelIndex>(spy.at(0).at(0)), proxy()->index(0, 0));
	QCOMPARE(qvariant_cast<QModelIndex>(spy.at(0).at(1)), proxy()->index(0, 0));
}

void QCheckStateProxyModelTestCase::setCheckedIndexes()
{
	QSignalSpy modelReset(proxy(), SIGNAL(modelReset()));
}

void QCheckStateProxyModelTestCase::setColumnCheckable()
{
	proxy()->setColumnCheckable(0, true);
	QVERIFY(proxy()->isColumnCheckable(0));
}

void QCheckStateProxyModelTestCase::setSourceModel()
{
	QCheckStateProxyModel* p = new QCheckStateProxyModel(this);
	QSignalSpy modelAboutToBeReset(p, SIGNAL(modelAboutToBeReset()));
	QSignalSpy modelReset(p, SIGNAL(modelAboutToBeReset()));
	p->setSourceModel(model());
	QCOMPARE(p->rowCount(), model()->rowCount());
	QCOMPARE(p->checkedIndexes().size(), 0);
	QCOMPARE(p->checkableColumnsCount(), 0);

	QCOMPARE(modelAboutToBeReset.count(), 2);
	QCOMPARE(modelReset.count(), 2);
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QCheckStateProxyModelTestCase tc1;
	return QTest::qExec(&tc1, argc, argv);
}
 
#include "main.moc"
