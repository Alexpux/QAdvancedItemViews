#include <QSignalSpy>
#include <QStandardItemModel>
#include <QTest>
#include <qabstractfilter.h>
#include <qadvancedtableview.h>
#include <qtextfilter.h>

class QAdvancedTableViewTest : public QObject {
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
    QAdvancedTableView *m_view { nullptr };
    QStandardItemModel *m_model { nullptr };
};

void QAdvancedTableViewTest::clicked()
{
    QSignalSpy spy(m_view, SIGNAL(clicked(QModelIndex)));
    QTest::mouseClick(m_view->viewport(), Qt::LeftButton, Qt::KeyboardModifiers(),
                      QPoint(20, 20));
    QCOMPARE(spy.count(), 1);
}

void QAdvancedTableViewTest::currentIndex()
{
    // QModelIndex i = m_view->model()->index(0, 1);
    // m_view->setCurrentIndex(i);
    // QModelIndex c = m_view->currentIndex();
    // QCOMPARE(m_view->currentIndex(), i);
}

void QAdvancedTableViewTest::doubleClicked()
{
    QSignalSpy spy(m_view, SIGNAL(doubleClicked(QModelIndex)));
    QTest::mouseDClick(m_view->viewport(), Qt::LeftButton,
                       Qt::KeyboardModifiers(), QPoint(20, 20));
    QCOMPARE(spy.count(), 1);
}

void QAdvancedTableViewTest::indexAt()
{
    QModelIndex i = m_view->indexAt(QPoint(20, 20));
    QCOMPARE(i, m_view->model()->index(0, 0));
}

void QAdvancedTableViewTest::model() { QCOMPARE(m_view->model(), m_model); }

void QAdvancedTableViewTest::pressed()
{
    QSignalSpy spy(m_view, SIGNAL(pressed(QModelIndex)));
    QTest::mousePress(m_view->viewport(), Qt::LeftButton, Qt::KeyboardModifiers(),
                      QPoint(20, 20));
    QCOMPARE(spy.count(), 1);
}

void QAdvancedTableViewTest::selectionModel()
{
    m_view->selectAll();
    QCOMPARE(m_view->selectionModel()->selectedRows().size(),
             m_view->model()->rowCount());
    for (int i = 0; i < m_view->model()->rowCount(); i++) {
        m_view->selectionModel()->select(
            m_view->selectionModel()->model()->index(0, 0),
            QItemSelectionModel::Deselect);
        QCOMPARE(m_view->selectionModel()->selectedRows().size(),
                 m_view->model()->rowCount() - 1);
    }
}

void QAdvancedTableViewTest::setFilterType()
{
    m_view->setFilterType(QTextFilter::Type, 1);
    const QTextFilter *f = qfilter_cast<QTextFilter *>(m_view->filterAt(0, 1));
    QVERIFY(f != nullptr);
}

void QAdvancedTableViewTest::viewport()
{
    // Qt6: Ensure widget is shown and active
    m_view->show();
    m_view->activateWindow();
    QVERIFY(QTest::qWaitForWindowActive(m_view));
    
    // Qt6: Explicitly enable mouse tracking
    m_view->viewport()->setMouseTracking(true);
    m_view->viewport()->setAttribute(Qt::WA_Hover, true);
    
    // Qt6: Set up signal spy
    QSignalSpy spy(m_view, SIGNAL(viewportEntered()));
    
    // Qt6: Use QTest::mouseMove with specific position
    QPoint centerPoint = m_view->viewport()->rect().center();
    
    // Move mouse away first, then back to trigger enter event
    QTest::mouseMove(m_view->viewport(), QPoint(-10, -10));
    QTest::qWait(100);
    QTest::mouseMove(m_view->viewport(), centerPoint);
    
    // Qt6: Wait for signal with timeout
    QVERIFY(QTest::qWaitFor([&spy]() { 
        return spy.count() >= 1; 
    }, 1000));
    
    QCOMPARE(spy.count(), 1);
    
    // Cleanup
    m_view->viewport()->setMouseTracking(false);
}

void QAdvancedTableViewTest::hideColumn()
{
    m_view->hideColumn(0);
    QCOMPARE(m_view->isColumnHidden(0), true);
}

void QAdvancedTableViewTest::initTestCase()
{
    m_view = new QAdvancedTableView();
    m_view->show();

    m_model = new QStandardItemModel(this);
    m_model->setColumnCount(2);

    QStandardItem *item;
    QList<QStandardItem *> items;

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
