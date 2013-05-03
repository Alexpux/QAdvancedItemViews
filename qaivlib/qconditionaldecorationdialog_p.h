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
#ifndef QCONDITIONALDECORATIONDIALOG_P_H
#define QCONDITIONALDECORATIONDIALOG_P_H

#include <QAbstractItemView>
#include <QDialog>
#include <QVariant>

class QAbstractItemModel;
class QLineEdit;
class QModelIndex;
class QTableView;

class ValueEdit;

class RangeEdit : public QWidget
{
    Q_OBJECT
public:
    RangeEdit(QAbstractItemModel* model, int column, QWidget* parent = 0);

	void setColumn(int col);

    void setRange(const QVariant & from, const QVariant & to);

    QVariant rangeFrom() const;

    QVariant rangeTo() const;
private:
    int m_column;
    ValueEdit* m_fromEdit;
    ValueEdit* m_toEdit;
    QAbstractItemModel* m_model;
};


class SelectValueDialog : public QDialog
{
    Q_OBJECT
public:
    SelectValueDialog(QAbstractItemModel* model, int column, QWidget* parent);

	int selectedColumn() const;

    QVariant selectedValue() const;

    QVariantList selectedValues() const;

    void setSelectionMode(QAbstractItemView::SelectionMode mode);
private slots:
	//void columnsComboBoxActivate(int index);
    void doubleClicked(const QModelIndex & index);
private:
	QComboBox* m_columnsComboBox;
    QTableView* m_view;
};

class ValueEdit : public QWidget
{
    Q_OBJECT
public:
    ValueEdit(QAbstractItemModel* model, int column, QWidget* parent = 0);

	int column() const;

	void setColumn(int col);

    void setValue(const QVariant & value);

    QVariant value() const;
private slots:
    void buttonClicked();
private:
    int m_column;
    QLineEdit* m_edit;
    QAbstractItemModel* m_model;
};


#endif // QCONDITIONALDECORATIONDIALOG_P_H
