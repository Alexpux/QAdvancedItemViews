#ifndef QSHAREDITEMSELECTIONMODEL_H
#define QSHAREDITEMSELECTIONMODEL_H

#include <QItemSelectionModel>

#include <qaivlib_global.h>

class QSharedItemSelectionModelPrivate;

class QAIVLIBSHARED_EXPORT QSharedItemSelectionModel : public QItemSelectionModel
{
    Q_OBJECT
public:
    QSharedItemSelectionModel(QAbstractItemModel* model, QItemSelectionModel* shared, QObject *parent = 0);
    ~QSharedItemSelectionModel();
signals:
    
public slots:
    void select(const QModelIndex & index, QItemSelectionModel::SelectionFlags command);
    void select(const QItemSelection & selection, QItemSelectionModel::SelectionFlags command);
private slots:
    void sharedSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
private:
    QSharedItemSelectionModelPrivate* d;
};

#endif // QSHAREDITEMSELECTIONMODEL_H
