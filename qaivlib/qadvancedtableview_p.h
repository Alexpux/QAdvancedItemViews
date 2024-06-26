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
#ifndef QFILTERTABLEVIEW_P_H
#define QFILTERTABLEVIEW_P_H

#include "qadvancedtableview.h"
#include "qclickablelabel.h"

#include <QCheckBox>
#include <QDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QListWidget>
#include <QMetaType>
#include <QSignalMapper>
#include <QSortFilterProxyModel>
#include <QStringList>
#include <QStyledItemDelegate>
#include <QTableView>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

class QAbstractItemDelegate;
class QComboBox;
class QLineEdit;
class QStandardItemModel;

class QAdvancedTableViewProxy : public QTableView {
    Q_OBJECT
public:
    explicit QAdvancedTableViewProxy(QWidget *parent = nullptr);
    ~QAdvancedTableViewProxy() = default;

signals:
    void focusReceived();

protected:
    void closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint) override;
    void focusInEvent(QFocusEvent *event) override;
    // void mousePressEvent(QMouseEvent *event) override;
};

#endif // QFILTERTABLEVIEW_P_H
