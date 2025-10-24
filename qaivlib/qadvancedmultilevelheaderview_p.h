#ifndef QADVANCEDMULTILEVELHEADERVIEWPRIVATE_H
#define QADVANCEDMULTILEVELHEADERVIEWPRIVATE_H

#include "qadvancedconstants.h"

#include <QAbstractItemModel>
#include <QPainter>
#include <QStandardItemModel>

class QAdvancedMultiLevelHeaderViewPrivate {
public:
    // contains the model
    QAbstractItemModel *headerModel { nullptr };

    // Do we want to hide/show items when we click on their parents?
    bool collapseHeaderSection { true };

    bool rootLevelVisible { false };

    int minimumPaintepth { 1 };

    /* -----------------------------METHODS------------------------ */
    QAdvancedMultiLevelHeaderViewPrivate() = default;

    ~QAdvancedMultiLevelHeaderViewPrivate()
    {
        ClearModel();
    }

    QAdvancedMultiLevelHeaderViewPrivate *ClearModel()
    {
        delete headerModel;
        headerModel = nullptr;
        return this;
    }

    QAdvancedMultiLevelHeaderViewPrivate *initFromNewModel(Qt::Orientation orientation, QAbstractItemModel *model)
    {
        ClearModel();
        CustomRoles hf = (orientation == Qt::Orientation::Vertical)
            ? CustomRoles::MultiLevelHeaderView_Rows
            : CustomRoles::MultiLevelHeaderView_Columns;

        headerModel = new QStandardItemModel();
        if (model) {
            QVariant v(model->data(QModelIndex(), hf));
            if (v.isValid()) {
                headerModel = qobject_cast<QAbstractItemModel *>(v.value<QObject *>());
            }
        }
        return this;
    }
};

#endif // QADVANCEDMULTILEVELHEADERVIEWPRIVATE_H
