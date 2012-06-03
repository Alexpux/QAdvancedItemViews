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

#include <QDialog>

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

    void setRange(const QVariant & from, const QVariant & to);

    QVariant rangeFrom() const;

    QVariant rangeTo() const;
private:
    int cColumn;
    ValueEdit* cFromEdit;
    ValueEdit* cToEdit;
    QAbstractItemModel* cModel;
};


class SelectValueDialog : public QDialog
{
    Q_OBJECT
public:
    SelectValueDialog(QAbstractItemModel* model, int column, QWidget* parent);

    QVariant selectedValue() const;
private slots:
    void doubleClicked(const QModelIndex & index);
private:
    QTableView* cView;
};

class ValueEdit : public QWidget
{
    Q_OBJECT
public:
    ValueEdit(QAbstractItemModel* model, int column, QWidget* parent = 0);

    void setValue(const QVariant & value);

    QVariant value() const;
private slots:
    void buttonClicked();
private:
    int cColumn;
    QLineEdit* cEdit;
    QAbstractItemModel* cModel;
};


#endif // QCONDITIONALDECORATIONDIALOG_P_H
