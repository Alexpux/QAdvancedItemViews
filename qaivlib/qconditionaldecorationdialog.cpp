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
#include "qconditionaldecorationdialog.h"
#include "ui_qconditionaldecorationdialog.h"

#include "qconditionaldecorationproxymodel.h"
#include "qconditionaldecoration.h"
#include "quniquevaluesproxymodel.h"
#include "qsinglecolumnproxymodel.h"

#include "qconditionaldecorationdialog_p.h"

//#include "UngroupProxyModel.h"

QConditionalDecorationDialog::QConditionalDecorationDialog(const QModelIndex &index, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QConditionalDecorationDialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("Conditional Decoration"));

    ui->dataRoleComboBox->addItem(tr("Display Role"), Qt::DisplayRole);

    ui->conditionsTableWidget->setColumnCount(4);
    QStringList mLabels;
    mLabels << tr("Condition") << tr("Value") << tr("Icon Set") << tr("Icon");
    ui->conditionsTableWidget->setHorizontalHeaderLabels(mLabels);

    cIconSets = index.data(QConditionalDecorationProxyModel::IconSetsRole).toMap();
    setProperties(index);
}

QConditionalDecorationDialog::~QConditionalDecorationDialog()
{
    delete ui;
}

QVariantMap QConditionalDecorationDialog::properties() const
{
    QVariantMap properties;
    QVariantList conditions;
    QComboBox* comboBox;
    for (int iRows = 0; iRows < ui->conditionsTableWidget->rowCount(); iRows++){
        QVariantMap mondition;

        comboBox = qobject_cast<QComboBox*>(ui->conditionsTableWidget->cellWidget(iRows, 0));
        mondition["matchFlag"] = comboBox->itemData(comboBox->currentIndex());
        ValueEdit* mValueEdit = qobject_cast<ValueEdit*>(ui->conditionsTableWidget->cellWidget(iRows, 1));
        if (mValueEdit){
            mondition["value"] = mValueEdit->value();
        }
        RangeEdit* mRangeEdit = qobject_cast<RangeEdit*>(ui->conditionsTableWidget->cellWidget(iRows, 1));
        if (mRangeEdit){
            mondition["from"] = mRangeEdit->rangeFrom();
            mondition["to"] = mRangeEdit->rangeTo();
        }

        comboBox = qobject_cast<QComboBox*>(ui->conditionsTableWidget->cellWidget(iRows, 2));
        mondition["set"] = comboBox->currentText();

        comboBox = qobject_cast<QComboBox*>(ui->conditionsTableWidget->cellWidget(iRows, 3));
        if (comboBox){
            mondition["name"] = comboBox->currentText();
        }

        conditions << mondition;
    }
    properties["conditions"] = conditions;
    return properties;
}

void QConditionalDecorationDialog::setProperties(const QModelIndex & index)
{
	if (!index.isValid()){
		return;
	}
    cIndex = index;
    cProperties = index.data(QConditionalDecorationProxyModel::ConditionalDecorationRole).toMap();
    if (cProperties.isEmpty()){
        cProperties["column"] = index.column();
    }
    //
    ui->columnLabel->setText(QString("%1 (%2)").arg(index.model()->headerData(index.column(), Qt::Horizontal).toString()).arg(index.column()));
    //
    ui->dataRoleComboBox->setCurrentIndex(ui->dataRoleComboBox->findData(cProperties.value("dataRole", Qt::DisplayRole).toInt()));
    QVariantList mConditions = cProperties.value("conditions").toList();
    ui->conditionsTableWidget->setRowCount(mConditions.size());
    QComboBox* mComboBox;
    for (int iCondition = 0; iCondition < mConditions.size(); iCondition++){
        mComboBox = conditionsComboBox();
        mComboBox->setCurrentIndex(mComboBox->findData(mConditions.at(iCondition).toMap().value("matchFlag", QConditionalDecoration::Contains)));
        ui->conditionsTableWidget->setCellWidget(iCondition, 0, mComboBox);
        //
        if (mConditions.at(iCondition).toMap().value("matchFlag").toInt() == QConditionalDecoration::IsBetween ||
                mConditions.at(iCondition).toMap().value("matchFlag").toInt() == QConditionalDecoration::IsNotBetween){
            RangeEdit* mEdit = new RangeEdit((QAbstractItemModel*)index.model(), cProperties.value("column", 0).toInt(), this);
            mEdit->setRange(mConditions.at(iCondition).toMap().value("from"), mConditions.at(iCondition).toMap().value("to"));
            ui->conditionsTableWidget->setCellWidget(iCondition, 1, mEdit);
        } else {
            ValueEdit* mEdit = new ValueEdit((QAbstractItemModel*)index.model(), cProperties.value("column", 0).toInt(), this);
            mEdit->setValue(mConditions.at(iCondition).toMap().value("value"));
            ui->conditionsTableWidget->setCellWidget(iCondition, 1, mEdit);
        }
        mComboBox = new QComboBox(this);
        mComboBox->addItems(cIconSets.keys());
        connect(mComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(iconSetComboBoxActivated(QString)));
        ui->conditionsTableWidget->setCellWidget(iCondition, 2, mComboBox);
        mComboBox->setCurrentIndex(mComboBox->findText(mConditions.at(iCondition).toMap().value("set").toString()));
        if (mComboBox->currentIndex() == 0){
             ui->conditionsTableWidget->setCellWidget(iCondition, 3, iconSetComboBox(mConditions.at(iCondition).toMap().value("set").toString()));
        }

        mComboBox = qobject_cast<QComboBox*>(ui->conditionsTableWidget->cellWidget(iCondition, 3));
        if (mComboBox){
            mComboBox->setCurrentIndex(mComboBox->findText(mConditions.at(iCondition).toMap().value("name").toString()));
        }
    }
    ui->conditionsTableWidget->resizeColumnToContents(0);
    ui->conditionsTableWidget->resizeColumnToContents(1);
    ui->conditionsTableWidget->resizeColumnToContents(2);
    ui->conditionsTableWidget->resizeRowsToContents();

    ui->removePushButton->setEnabled(ui->conditionsTableWidget->rowCount() > 0);
}

void QConditionalDecorationDialog::addPushButtonClicked()
{
    ui->conditionsTableWidget->insertRow(ui->conditionsTableWidget->rowCount());
	ui->conditionsTableWidget->setCellWidget(ui->conditionsTableWidget->rowCount() - 1, 0, conditionsComboBox());
    ui->conditionsTableWidget->setCellWidget(ui->conditionsTableWidget->rowCount() - 1, 1, valueEdit());
    QComboBox* cb = new QComboBox(this);
    cb->addItems(cIconSets.keys());
    connect(cb, SIGNAL(currentIndexChanged(QString)), this, SLOT(iconSetComboBoxActivated(QString)));
    ui->conditionsTableWidget->setCellWidget(ui->conditionsTableWidget->rowCount() - 1, 2, cb);
    ui->conditionsTableWidget->setCellWidget(ui->conditionsTableWidget->rowCount() - 1, 3, iconSetComboBox(cb->currentText()));
    ui->conditionsTableWidget->resizeColumnToContents(0);
    ui->conditionsTableWidget->resizeColumnToContents(1);
    ui->conditionsTableWidget->resizeColumnToContents(2);
    ui->conditionsTableWidget->resizeRowToContents(ui->conditionsTableWidget->rowCount() - 1);
	// Ensure that the new row is visible
	ui->conditionsTableWidget->scrollTo(ui->conditionsTableWidget->model()->index(ui->conditionsTableWidget->rowCount() - 1, 0));

    ui->removePushButton->setEnabled(true);
}

void QConditionalDecorationDialog::comparisonComboBoxActivated(int index)
{
    QComboBox* mComboBox = qobject_cast<QComboBox*>(sender());
    if (mComboBox){
        int mRow = -1;
        for (int iRow = 0; iRow < ui->conditionsTableWidget->rowCount(); iRow++){
            if (ui->conditionsTableWidget->cellWidget(iRow, 0) == mComboBox){
                mRow = iRow;
            }
        }
        if (mRow > -1){
            QVariantMap mProperties = value(mRow);
            if (mComboBox->itemData(index).toInt() == QConditionalDecoration::IsBetween || mComboBox->itemData(index).toInt() == QConditionalDecoration::IsNotBetween){
                RangeEdit* mEdit = new RangeEdit((QAbstractItemModel*)cIndex.model(), cProperties.value("column", 0).toInt(), this);
                if (mProperties.contains("value")){
                    mEdit->setRange(mProperties.value("value"), QVariant());
                } else {
                    mEdit->setRange(mProperties.value("from"), mProperties.value("to"));
                }
                ui->conditionsTableWidget->setCellWidget(mRow, 1, mEdit);
            } else {
                ValueEdit* mEdit = new ValueEdit((QAbstractItemModel*)cIndex.model(), cProperties.value("column", 0).toInt(), this);
                mEdit->setValue(mProperties.value("value"));
                ui->conditionsTableWidget->setCellWidget(mRow, 1, mEdit);
            }
        }
//        ui->conditionsTableWidget->resizeColumnToContents(1);
    }
}

QComboBox *QConditionalDecorationDialog::conditionsComboBox()
{
    QComboBox* mComboBox = new QComboBox(this);
    mComboBox->addItem(tr("contains"), QConditionalDecoration::Contains);
    mComboBox->addItem(tr("contains not"), QConditionalDecoration::ContainsNot);
    mComboBox->addItem(tr("less"), QConditionalDecoration::IsLess);
    mComboBox->addItem(tr("between"), QConditionalDecoration::IsBetween);
    mComboBox->addItem(tr("greator"), QConditionalDecoration::IsGreator);
    mComboBox->addItem(tr("less or equal"), QConditionalDecoration::IsLessOrEqual);
    mComboBox->addItem(tr("greator or equal"), QConditionalDecoration::IsGreatorOrEqual);
    mComboBox->addItem(tr("not between"), QConditionalDecoration::IsNotBetween);
    mComboBox->addItem(tr("empty"), QConditionalDecoration::IsEmpty);
    mComboBox->addItem(tr("not empty"), QConditionalDecoration::IsNotEmpty);
    mComboBox->addItem(tr("equal"), QConditionalDecoration::IsEqual);
    mComboBox->addItem(tr("not equal"), QConditionalDecoration::IsNotEqual);
    mComboBox->addItem(tr("starts with"), QConditionalDecoration::StartsWith);
    mComboBox->addItem(tr("ends with"), QConditionalDecoration::EndsWith);
    mComboBox->addItem(tr("starts not with"), QConditionalDecoration::StartsNotWith);
    mComboBox->addItem(tr("ends not with"), QConditionalDecoration::EndsNotWith);
    connect(mComboBox, SIGNAL(activated(int)), this, SLOT(comparisonComboBoxActivated(int)));
    return mComboBox;
}

void QConditionalDecorationDialog::iconSetComboBoxActivated(const QString & text)
{
    int mRow = -1;
    for (int iRow = 0; iRow < ui->conditionsTableWidget->rowCount(); iRow++){
        if (ui->conditionsTableWidget->cellWidget(iRow, 2) == sender()){
            mRow = iRow;
            break;
        }
    }
    ui->conditionsTableWidget->setCellWidget(mRow, 3, iconSetComboBox(text));
}

void QConditionalDecorationDialog::removePushButtonClicked()
{
    ui->conditionsTableWidget->removeRow(ui->conditionsTableWidget->currentRow());
    ui->removePushButton->setEnabled(ui->conditionsTableWidget->rowCount() > 0);
}

QComboBox *QConditionalDecorationDialog::iconSetComboBox(const QString & name)
{
    if (!cIconSets.keys().contains(name)){
        return 0;
    }
    QComboBox* mComboBox = new QComboBox(this);
    QMapIterator<QString,QVariant> mIt(cIconSets.value(name).toMap());
    while(mIt.hasNext()){
        mIt.next();
        mComboBox->addItem(QIcon(mIt.value().toString()), mIt.key(), mIt.value());
    }
    return mComboBox;
}

ValueEdit *QConditionalDecorationDialog::valueEdit()
{
    return new ValueEdit((QAbstractItemModel*)cIndex.model(), cProperties.value("column", 0).toInt(), this);
}

QVariantMap QConditionalDecorationDialog::value(int row) const
{
    QVariantMap mMap;
    RangeEdit* mEdit = qobject_cast<RangeEdit*>(ui->conditionsTableWidget->cellWidget(row, 1));
    if (mEdit){
        mMap["from"] = mEdit->rangeFrom();
        mMap["to"] = mEdit->rangeTo();
    } else {
        ValueEdit* mEdit = qobject_cast<ValueEdit*>(ui->conditionsTableWidget->cellWidget(row, 1));
        if (mEdit){
            mMap["value"] = mEdit->value();
        }
    }
    return mMap;
}

SelectValueDialog::SelectValueDialog(QAbstractItemModel* model, int column, QWidget *parent) :
    QDialog(parent)
{
    setWindowIcon(QIcon(":/qaiv/dialog/table.select"));
    setWindowTitle(tr("Select Value..."));

    QVBoxLayout* layout = new QVBoxLayout(this);

    m_view = new QTableView(this);
    layout->addWidget(m_view);
    m_view->horizontalHeader()->setStretchLastSection(true);
    m_view->setEditTriggers(QTableView::NoEditTriggers);
    m_view->verticalHeader()->setVisible(false);

//    UngroupProxyModel* mUngroupProxy = new UngroupProxyModel(this);
//    mUngroupProxy->setSourceModel(model);

//    qDebug() << mUngroupProxy->index(0, column).data();
//    QSingleColumnProxyModel* mSingleColumnProxy = new QSingleColumnProxyModel(this);
//    mSingleColumnProxy->setModelColumn(column);
//    mSingleColumnProxy->setSourceModel(mUngroupProxy);

//    qDebug() << column << mSingleColumnProxy->rowCount();


    QSingleColumnProxyModel* columnProxy = new QSingleColumnProxyModel(this);
    columnProxy->setSourceModelColumn(column);
    columnProxy->setSourceModel(model);
    QUniqueValuesProxyModel* valueProxy = new QUniqueValuesProxyModel(this);
    valueProxy->setModelColumn(0);
    valueProxy->setSourceModel(columnProxy);
    m_view->setModel(valueProxy);

    m_view->resizeRowsToContents();

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

QVariant SelectValueDialog::selectedValue() const
{
    return m_view->currentIndex().data();
}

QVariantList SelectValueDialog::selectedValues() const
{
    QVariantList l;
    Q_FOREACH(QModelIndex i, m_view->selectionModel()->selectedRows()){
        l << i.data();
    }
    return l;
}

void SelectValueDialog::setSelectionMode(QAbstractItemView::SelectionMode mode)
{
    m_view->setSelectionMode(mode);
}

void SelectValueDialog::doubleClicked(const QModelIndex & index)
{
    Q_UNUSED(index);
    accept();
}

ValueEdit::ValueEdit(QAbstractItemModel* model, int column, QWidget *parent) :
    QWidget(parent)
{
    m_column = column;
    m_model = model;

    QHBoxLayout* mLayout = new QHBoxLayout(this);
    mLayout->setContentsMargins(0, 0, 0, 0);
    mLayout->setSpacing(0);

    m_edit = new QLineEdit(this);

    QPushButton* mButton = new QPushButton(this);
    mButton->setFlat(true);
    mButton->setIcon(QIcon(":/qaiv/dialog/table.select"));
    connect(mButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));

    mLayout->addWidget(m_edit);
    mLayout->addWidget(mButton);
}

void ValueEdit::setValue(const QVariant &value)
{
    m_edit->setText(value.toString());
}

QVariant ValueEdit::value() const
{
    return m_edit->text();
}

void ValueEdit::buttonClicked()
{
    SelectValueDialog* mDlg = new SelectValueDialog(m_model, m_column, this);
    if (mDlg->exec()){
        m_edit->setText(mDlg->selectedValue().toString());
    }
    delete mDlg;
}

RangeEdit::RangeEdit(QAbstractItemModel* model, int column, QWidget* parent) :
    QWidget(parent)
{
    cModel = model;
    cColumn = column;

    QHBoxLayout * mLayout = new QHBoxLayout(this);
    mLayout->setContentsMargins(0, 0, 0, 0);

    cFromEdit = new ValueEdit(model, column, this);
//    cFromEdit->layout()->setContentsMargins(9, 0, 0, 0);
    mLayout->addWidget(cFromEdit);


    mLayout->addWidget(new QLabel(tr("and"), this));

    cToEdit = new ValueEdit(model, column, this);
//    cToEdit->layout()->setContentsMargins(0, 0, 0, 0);
    mLayout->addWidget(cToEdit);

}

void RangeEdit::setRange(const QVariant & from, const QVariant & to)
{
    cFromEdit->setValue(from);
    cToEdit->setValue(to);
}

QVariant RangeEdit::rangeFrom() const
{
    return cFromEdit->value();
}

QVariant RangeEdit::rangeTo() const
{
    return cToEdit->value();
}
