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

#include <QCheckBox>
#include <QDialog>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QListWidget>
#include <QMetaType>
#include <QSignalMapper>
#include <QStyledItemDelegate>
#include <QSortFilterProxyModel>
#include <QStringList>
#include <QTableView>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

#include <qadvancedtableview.h>
#include <qclickablelabel.h>

class QComboBox;
class QAbstractItemDelegate;
class QLineEdit;
class QStandardItemModel;

class QAdvancedTableViewProxy : public QTableView
{
    Q_OBJECT
public:
    QAdvancedTableViewProxy(QWidget* parent = 0);
    ~QAdvancedTableViewProxy();
signals:
    void focusReceived();
protected:
    void closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint);
    void focusInEvent(QFocusEvent *event);
};

//-----------------------------------------------
// class QFilterTableViewSettingsDialog
//-----------------------------------------------

class QFilterTableViewSettingsDialog : public QDialog
{
	Q_OBJECT
public:
    QFilterTableViewSettingsDialog(QAbstractFilterModel* model, QWidget* parent = 0);
	~QFilterTableViewSettingsDialog();
private:
	void retranslate();
	void setup();

    QAbstractFilterModel* cModel;

	QVBoxLayout *verticalLayout_2;
	QHBoxLayout *horizontalLayout_2;
	QHBoxLayout *horizontalLayout;
	QLabel *label_2;
	QLineEdit *nameLineEdit;
	QPushButton *renamePushButton;
	QGroupBox *groupBox_2;
	QHBoxLayout *horizontalLayout_3;
	QTableWidget *tableWidget;
	QVBoxLayout *verticalLayout;
	QPushButton *addPushButton;
	QPushButton *removePushButton;
	QPushButton *selectPushButton;
	QSpacerItem *verticalSpacer;
	QPushButton *closePushButton;
};

#endif // QFILTERTABLEVIEW_P_H
