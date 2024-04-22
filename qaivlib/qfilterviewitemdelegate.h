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
#ifndef QFILTERVIEWITEMDELEGATE_H
#define QFILTERVIEWITEMDELEGATE_H

#include "qaivlib_global.h"

#include <QListWidgetItem>
#include <QStyledItemDelegate>

//! The QFilterViewItemDelegate class provides display and editing facilities for filter from a filter model.
class QAIVLIBSHARED_EXPORT QFilterViewItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    /**
     * Constructs an item delegate with the given @p parent.
     */
    explicit QFilterViewItemDelegate(QObject *parent);
    /**
     * Destroys the item delegate.
     */
    ~QFilterViewItemDelegate() = default;
    /**
     * @reimp
     * @see QAbstractFilter:createEditor()
     */
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    /**
     * Updates the editor for the item specified by index according to the style option given.
     */
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

public slots:
    void commitAndClose(QAbstractItemDelegate::EndEditHint hint = QAbstractItemDelegate::NoHint);
    void cancelAndClose(QAbstractItemDelegate::EndEditHint hint = QAbstractItemDelegate::NoHint);

private slots:
    void comboxBoxItemActivated(int index);

    void listWidgetCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void lineEditReturnPressed();

private:
    bool m_enableFilter { false };
};

#endif // QFILTERVIEWITEMDELEGATE_H
