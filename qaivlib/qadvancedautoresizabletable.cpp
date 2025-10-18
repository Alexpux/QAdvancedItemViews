#include "qadvancedautoresizabletable.h"
#include <QDebug>
#include <QTimer>
#include <QHeaderView>
#include <QtMath>
#include <QProgressDialog>
#include <QString>
#include <QApplication>

void QAdvacedAutoResizableTable::adaptColumns(QMap<int, int> resColumnsIdx, bool forceFitSize)
{
    if (!model())
        return;

    int columnsCnt = model()->columnCount();

    QMap<int, int>::const_iterator it = resColumnsIdx.constBegin();
    auto end = resColumnsIdx.constEnd();
    int partCount = 0;
    while (it != end) {
        partCount += it.value();
        if (it.key() >= columnsCnt) {
            return;
        }
        ++it;
    }

    if (forceFitSize) {
        int resizedWidth = 0;
        for (int i = 0; i < columnsCnt; i++) {
            if (!resColumnsIdx.contains(i)) {
                resizeColumnToContents(i);
                resizedWidth += columnWidth(i);
            }
        }

        int resColumnWidth = horizontalHeader()->width() - resizedWidth;
        if (resColumnWidth < 0)
            return;

        it = resColumnsIdx.constBegin();
        while (it != end) {
            setColumnWidth(it.key(), (resColumnWidth / partCount) * it.value());
            ++it;
        }
    } else {
        int resizedWidth = 0;
        for (int i=0; i < columnsCnt; i++) {
            resizeColumnToContents(i);
            resizedWidth += columnWidth(i);
        }

        int deltaWidth = horizontalHeader()->width() - resizedWidth;
        if (deltaWidth < 0)
            return;

        it = resColumnsIdx.constBegin();
        while (it != end) {
            setColumnWidth(it.key(), (deltaWidth / partCount) * it.value());
            ++it;
        }
    }
}

QAdvacedAutoResizableTable::QAdvacedAutoResizableTable(QWidget *parent) : QTableView(parent)
{
    m_forceFitSize = true;
    //setWordWrap(true);
    //setFocusPolicy(Qt::NoFocus);
    //setContextMenuPolicy(Qt::CustomContextMenu);
    //setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //setSelectionBehavior(QAbstractItemView::SelectRows);
    //setSelectionMode(QAbstractItemView::SingleSelection);
    //setAlternatingRowColors(true);

    //horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    //horizontalHeader()->setStretchLastSection(false);
    //horizontalHeader()->setHighlightSections(false);
    //horizontalHeader()->setSortIndicatorShown(false);
    //horizontalHeader()->setHidden(true);

    //verticalHeader()->setHidden(true);

    // It's greatly decrease perfomance!!!
    /*verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);*/
}

void QAdvacedAutoResizableTable::setAutoResizeParams(int resColumnIdx, bool forceFitSize)
{
    m_resColumnsIdx[resColumnIdx] = 1;
    m_forceFitSize = forceFitSize;
}

void QAdvacedAutoResizableTable::setAutoResizeParams(QMap<int, int> resColumnsIdx, bool forceFitSize)
{
    m_resColumnsIdx = resColumnsIdx;
    m_forceFitSize = forceFitSize;
}

void QAdvacedAutoResizableTable::setModel(QAbstractItemModel *model)
{
    connect(model, &QAbstractItemModel::layoutChanged, this, &QAdvacedAutoResizableTable::modelLayoutChanged);
    QTableView::setModel(model);
}

void QAdvacedAutoResizableTable::modelLayoutChanged()
{
    QTimer::singleShot(0, this, &QAdvacedAutoResizableTable::modelLayoutChangedImpl);
}

void QAdvacedAutoResizableTable::modelLayoutChangedImpl()
{
    adaptColumns(m_resColumnsIdx, m_forceFitSize);
}

void QAdvacedAutoResizableTable::showEvent(QShowEvent *event)
{
    QTableView::showEvent(event);
    //modelLayoutChanged();
}

void QAdvacedAutoResizableTable::resizeEvent(QResizeEvent* ev) {
    QTableView::resizeEvent(ev);
    modelLayoutChanged();
}
