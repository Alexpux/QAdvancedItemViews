/******************************************************************************
** This file is part of qadvanceditemviews.
**
** Copyright (c) 2011-2012 Martin Hoppe martin@2x2hoppe.de
**
** qadvanceditemviews is free software: you can redistribute it
** and/or modify it under the terms of the GNU Lesser General
** Public License as published by the Free Software Foundation,
** either version 3 of the License, or (at your option) any
** later version.
**
** qadvanceditemviews is distributed in the hope that it will be
** useful, but WITHOUT ANY WARRANTY; without even the implied
** warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with qadvanceditemviews.
** If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#include "stdafx.h"
#include "qfilterviewitemdelegate.h"

#include "qabstractfilter.h"
#include "qabstractfiltermodel.h"
#include "qfilterview.h"

QFilterViewItemDelegate::QFilterViewItemDelegate( QObject* parent )
    : QStyledItemDelegate(parent)
{

}

QFilterViewItemDelegate::~QFilterViewItemDelegate()
{
}

void QFilterViewItemDelegate::comboxBoxItemActivated( int index )
{
    Q_UNUSED(index);
    emit commitData((QWidget*)sender());
    emit closeEditor((QWidget*)sender());

}

QWidget* QFilterViewItemDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
   const QAbstractFilterModel* mModel = qobject_cast<const QAbstractFilterModel*>(index.model());
    if (mModel){
        QAbstractFilter* mFilter = mModel->filter(index);
        if (mFilter){
            return mFilter->createEditor(parent, option, index);
        } else {
            QFilterView* mFilterView = qobject_cast<QFilterView*>(parent->parentWidget());
            if (mFilterView){
                QVariantMap mProperties;
                mProperties["column"] = index.column();
                mProperties["row"] = index.row();
                mProperties["type"] = index.data(QAbstractFilterModel::DefaultFilterTypeRole).toInt();
                mFilter = mModel->createFilter(index, mProperties);
                if (mFilter){
                    mFilter->setEnabled(true);
                    return mFilter->createEditor(parent, option, index);
                }
            }
        }
    }
    return 0;
}

void QFilterViewItemDelegate::lineEditReturnPressed()
{
    emit commitData((QWidget*)sender());
    emit closeEditor((QWidget*)sender());
}

void QFilterViewItemDelegate::listWidgetCurrentItemChanged( QListWidgetItem* current, QListWidgetItem* previous )
{
    emit commitData((QWidget*)sender());
    emit closeEditor((QWidget*)sender());
}

void QFilterViewItemDelegate::setEditorData(QWidget* editor, const QModelIndex & index ) const
{
    const QAbstractFilterModel* mModel = qobject_cast<const QAbstractFilterModel*>(index.model());
     if (mModel){
         QAbstractFilter* mFilter = mModel->filter(index);
         if (mFilter){
             return mFilter->setEditorData(editor, index);
         }
     }
}

void QFilterViewItemDelegate::setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    const QAbstractFilterModel* mModel = qobject_cast<const QAbstractFilterModel*>(index.model());
     if (mModel){
         QAbstractFilter* mFilter = mModel->filter(index);
         if (mFilter){
             mFilter->setModelData(editor, model, index);
         }
     }
}

void QFilterViewItemDelegate::updateEditorGeometry( QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QRect mRect = option.rect;
    mRect.setHeight(editor->sizeHint().height());
    editor->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    editor->setGeometry(mRect);

    QTableView* mView = qobject_cast<QTableView*>(editor->parentWidget()->parentWidget());
    if (mView){
        mRect = QRect(editor->parentWidget()->parentWidget()->mapToGlobal(editor->parentWidget()->parentWidget()->geometry().topLeft()), QSize(100,100));
        editor->move(mView->viewport()->mapToGlobal(option.rect.topLeft()));
    }
}
