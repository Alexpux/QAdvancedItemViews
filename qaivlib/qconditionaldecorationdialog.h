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
#ifndef QCONDITIONALDECORATIONDIALOG_H
#define QCONDITIONALDECORATIONDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QVariant>

namespace Ui {
class QConditionalDecorationDialog;
}

class QComboBox;

class ValueEdit;

#include <qaivlib_global.h>

//! The QConditionalDecorationDialog class provides a dialog that allow users to modify conditional decorations.
/**
  * @code
  * QConditionalDecorationDialog* dlg = new QConditionalDecorationDialog(index, this);
  * if (dlg->exec()){
  *  QAbstractItemModel* model = (QAbstractItemModel*)index.model();
  *     model->setData(index, dlg->properties(), QConditionalDecorationProxyModel::ConditionalDecorationRole);
  * }
  * delete dlg;
  * @endcode
  * @image html qconditionaldecorationdialog01.png
  */
class QAIVLIBSHARED_EXPORT QConditionalDecorationDialog : public QDialog
{
    Q_OBJECT
    
public:
    /**
      * Constructs a conditional decoration dialog for the given @p index and with the given @p parent.
      */
    explicit QConditionalDecorationDialog(const QModelIndex & index, QWidget *parent = 0);
    /**
      * Destroys the QConditionalDecorationDialog.
      */
    ~QConditionalDecorationDialog();
    /**
      * Returns the properties.
      */
    QVariantMap properties() const;

    void setProperties(const QModelIndex & index);
private slots:
    void addPushButtonClicked();
    void comparisonComboBoxActivated(int index);
    void iconSetComboBoxActivated(const QString & text);
    void removePushButtonClicked();
private:
    QComboBox* conditionsComboBox();
    QComboBox* iconSetComboBox(const QString & name);
    ValueEdit* valueEdit();

    QVariantMap value(int row) const;

    QModelIndex cIndex;
    QVariantMap cIconSets;
    QVariantMap cProperties;

    Ui::QConditionalDecorationDialog *ui;
};

#endif // QCONDITIONALDECORATIONDIALOG_H
