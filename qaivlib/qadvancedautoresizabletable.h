#ifndef QADVANCEDAUTORESIZABLETABLE_H
#define QADVANCEDAUTORESIZABLETABLE_H

#include <QTableView>
#include <QAbstractItemModel>

class QAdvacedAutoResizableTable : public QTableView
{
    Q_OBJECT

public:
    explicit QAdvacedAutoResizableTable(QWidget *parent = nullptr);
    virtual void setModel(QAbstractItemModel *model) override;
    void setAutoResizeParams(int resColumnIdx, bool forceFitSize);
    void setAutoResizeParams(QMap<int, int> resColumnsIdx, bool forceFitSize);

public slots:
    void modelLayoutChanged();

protected slots:
    virtual void modelLayoutChangedImpl();

protected:
    QMap<int, int> m_resColumnsIdx;
    bool m_forceFitSize;
    virtual void showEvent(QShowEvent *event) override;
    virtual void resizeEvent(QResizeEvent* ev) override;
    void adaptColumns(QMap<int, int> resColumnsIdx, bool forceFitSize);
};

#endif // QADVANCEDAUTORESIZABLETABLE_H
