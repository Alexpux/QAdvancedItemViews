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

#define COLUMN_COLUMN		0
#define COLUMN_CONDITION	1
#define COLUMN_EDIT			2
#define COLUMN_SET			3
#define COLUMN_ICON			4

QConditionalDecorationDialog::QConditionalDecorationDialog(const QModelIndex & index, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::QConditionalDecorationDialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("Conditional Decoration"));

    ui->dataRoleComboBox->addItem(tr("Display Role"), Qt::DisplayRole);

    ui->conditionsTableWidget->setColumnCount(5);
    QStringList sl;
    sl << tr("Column") << tr("Condition") << tr("Value") << tr("Icon Set") << tr("Icon");
    ui->conditionsTableWidget->setHorizontalHeaderLabels(sl);

    cIconSets = index.data(QConditionalDecorationProxyModel::IconSetsRole).toMap();
    setProperties(index);

	m_defaultColumn = index.column();
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
        QVariantMap condition;

		comboBox = qobject_cast<QComboBox*>(ui->conditionsTableWidget->cellWidget(iRows, COLUMN_CONDITION));
        condition["matchFlag"] = comboBox->itemData(comboBox->currentIndex());
		condition["column"] = column(iRows);
		ValueEdit* ve = qobject_cast<ValueEdit*>(ui->conditionsTableWidget->cellWidget(iRows, COLUMN_EDIT));
        if (ve){
            condition["value"] = ve->value();
        }
		RangeEdit* re = qobject_cast<RangeEdit*>(ui->conditionsTableWidget->cellWidget(iRows, COLUMN_EDIT));
        if (re){
            condition["from"] = re->rangeFrom();
            condition["to"] = re->rangeTo();
        }

		comboBox = qobject_cast<QComboBox*>(ui->conditionsTableWidget->cellWidget(iRows, COLUMN_SET));
        condition["set"] = comboBox->currentText();

		comboBox = qobject_cast<QComboBox*>(ui->conditionsTableWidget->cellWidget(iRows, COLUMN_ICON));
        if (comboBox){
            condition["name"] = comboBox->currentText();
        }
        conditions << condition;
    }
    properties["conditions"] = conditions;
    return properties;
}

void QConditionalDecorationDialog::setProperties(const QModelIndex & index)
{
	if (!index.isValid()){
		return;
	}
    m_index = index;
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
    QComboBox* cb = 0;
    for (int iCondition = 0; iCondition < mConditions.size(); iCondition++){
		QVariantMap properties = mConditions.at(iCondition).toMap();
		// add combo box to column 0
		cb = columnComboBox();
		cb->setCurrentIndex(properties.value("column", index.column()).toInt());
		connect(cb, SIGNAL(activated(int)), this, SLOT(columnComboBoxActivated(int)));
		ui->conditionsTableWidget->setCellWidget(iCondition, COLUMN_COLUMN, cb);
		// 
        cb = conditionsComboBox();
        cb->setCurrentIndex(cb->findData(mConditions.at(iCondition).toMap().value("matchFlag", QConditionalDecoration::Contains)));
		ui->conditionsTableWidget->setCellWidget(iCondition, COLUMN_CONDITION, cb);
        //
        if (mConditions.at(iCondition).toMap().value("matchFlag").toInt() == QConditionalDecoration::IsBetween ||
                mConditions.at(iCondition).toMap().value("matchFlag").toInt() == QConditionalDecoration::IsNotBetween){
            RangeEdit* re = new RangeEdit((QAbstractItemModel*)index.model(), cProperties.value("column", 0).toInt(), this);
            re->setRange(mConditions.at(iCondition).toMap().value("from"), mConditions.at(iCondition).toMap().value("to"));
            ui->conditionsTableWidget->setCellWidget(iCondition, COLUMN_EDIT, re);
        } else {
			ValueEdit* ve = new ValueEdit((QAbstractItemModel*)index.model(), properties.value("column", index.column()).toInt(), this);
            ve->setValue(mConditions.at(iCondition).toMap().value("value"));
            ui->conditionsTableWidget->setCellWidget(iCondition, COLUMN_EDIT, ve);
        }
        cb = new QComboBox(this);
        cb->addItems(cIconSets.keys());
        connect(cb, SIGNAL(currentIndexChanged(QString)), this, SLOT(iconSetComboBoxActivated(QString)));
		ui->conditionsTableWidget->setCellWidget(iCondition, COLUMN_SET, cb);

		cb->setCurrentIndex(cb->findText(mConditions.at(iCondition).toMap().value("set").toString()));
        if (cb->currentIndex() == 0){
			ui->conditionsTableWidget->setCellWidget(iCondition, COLUMN_ICON, iconSetComboBox(mConditions.at(iCondition).toMap().value("set").toString()));
        }

        cb = qobject_cast<QComboBox*>(ui->conditionsTableWidget->cellWidget(iCondition, 4));
        if (cb){
            cb->setCurrentIndex(cb->findText(mConditions.at(iCondition).toMap().value("name").toString()));
        }
    }
    ui->conditionsTableWidget->resizeColumnToContents(0);
    ui->conditionsTableWidget->resizeColumnToContents(1);
    ui->conditionsTableWidget->resizeColumnToContents(2);
    ui->conditionsTableWidget->resizeColumnToContents(3);
    ui->conditionsTableWidget->resizeRowsToContents();

    ui->removePushButton->setEnabled(ui->conditionsTableWidget->rowCount() > 0);
}

void QConditionalDecorationDialog::addPushButtonClicked()
{
    ui->conditionsTableWidget->insertRow(ui->conditionsTableWidget->rowCount());
	//
	QComboBox* cb = columnComboBox();
	ui->conditionsTableWidget->setCellWidget(ui->conditionsTableWidget->rowCount() - 1, COLUMN_COLUMN, cb);
	connect(cb, SIGNAL(activated(int)), this, SLOT(columnComboBoxActivated(int)));
	ui->conditionsTableWidget->setCellWidget(ui->conditionsTableWidget->rowCount() - 1, COLUMN_CONDITION, conditionsComboBox());
	ValueEdit* ve = valueEdit();
	ve->setColumn(0);
    ui->conditionsTableWidget->setCellWidget(ui->conditionsTableWidget->rowCount() - 1, COLUMN_EDIT, ve);
    cb = new QComboBox(this);
    cb->addItems(cIconSets.keys());
    connect(cb, SIGNAL(currentIndexChanged(QString)), this, SLOT(iconSetComboBoxActivated(QString)));
	ui->conditionsTableWidget->setCellWidget(ui->conditionsTableWidget->rowCount() - 1, COLUMN_SET, cb);
	ui->conditionsTableWidget->setCellWidget(ui->conditionsTableWidget->rowCount() - 1, COLUMN_ICON, iconSetComboBox(cb->currentText()));
    ui->conditionsTableWidget->resizeColumnToContents(0);
    ui->conditionsTableWidget->resizeColumnToContents(1);
    ui->conditionsTableWidget->resizeColumnToContents(2);
    ui->conditionsTableWidget->resizeColumnToContents(3);
    ui->conditionsTableWidget->resizeRowToContents(ui->conditionsTableWidget->rowCount() - 1);
	// Ensure that the new row is visible
	ui->conditionsTableWidget->scrollTo(ui->conditionsTableWidget->model()->index(ui->conditionsTableWidget->rowCount() - 1, 0));

    ui->removePushButton->setEnabled(true);
}

int QConditionalDecorationDialog::column(int row) const
{
	QComboBox* cb = qobject_cast<QComboBox*>(ui->conditionsTableWidget->cellWidget(row, 0));
	if (cb){
		return cb->currentIndex();
	}
	return -1;
}

QComboBox* QConditionalDecorationDialog::columnComboBox()
{
	// add combo box to column 0
	QComboBox* cb = new QComboBox(this);
	for (int i = 0; i < m_index.model()->columnCount(); i++){
		cb->addItem(m_index.model()->headerData(i, Qt::Horizontal).toString(), i);
	}
	return cb;
}

void QConditionalDecorationDialog::columnComboBoxActivated(int index)
{
    QComboBox* cb = qobject_cast<QComboBox*>(sender());
    if (cb == 0){
		qWarning() << Q_FUNC_INFO << "combox box not found";
		return;
	}
	int r = -1;
	for (int iRow = 0; iRow < ui->conditionsTableWidget->rowCount(); iRow++){
		if (ui->conditionsTableWidget->cellWidget(iRow, COLUMN_COLUMN) == cb){
			r = iRow;
		}
	}
	if (r == -1){
		qWarning() << Q_FUNC_INFO << "invalid row";
		return;
	}
	RangeEdit* re = qobject_cast<RangeEdit*>(ui->conditionsTableWidget->cellWidget(r, COLUMN_EDIT));
	if (re){
		re->setColumn(index);
	}
	ValueEdit* ve = qobject_cast<ValueEdit*>(ui->conditionsTableWidget->cellWidget(r, COLUMN_EDIT));
	if (ve){
		ve->setColumn(index);
	}
}

void QConditionalDecorationDialog::comparisonComboBoxActivated(int index)
{
    QComboBox* cb = qobject_cast<QComboBox*>(sender());
    if (cb == 0){
		qWarning() << Q_FUNC_INFO << "combox box not found";
		return;
	}
	int r = -1;
	for (int iRow = 0; iRow < ui->conditionsTableWidget->rowCount(); iRow++){
		if (ui->conditionsTableWidget->cellWidget(iRow, COLUMN_CONDITION) == cb){
			r = iRow;
		}
	}
	if (r == -1){
		qWarning() << Q_FUNC_INFO << "invalid row";
		return;
	}
	QVariantMap p = value(r);
	if (cb->itemData(index).toInt() == QConditionalDecoration::IsBetween || cb->itemData(index).toInt() == QConditionalDecoration::IsNotBetween){
		RangeEdit* re = new RangeEdit((QAbstractItemModel*)m_index.model(), column(r), this);
		if (p.contains("value")){
			re->setRange(p.value("value"), QVariant());
		} else {
			re->setRange(p.value("from"), p.value("to"));
		}
		ui->conditionsTableWidget->setCellWidget(r, COLUMN_EDIT, re);
	} else {
		ValueEdit* ve = new ValueEdit((QAbstractItemModel*)m_index.model(), column(r), this);
		ve->setValue(p.value("value"));
		ui->conditionsTableWidget->setCellWidget(r, COLUMN_EDIT, ve);
	}
}

QComboBox* QConditionalDecorationDialog::conditionsComboBox()
{
    QComboBox* cb = new QComboBox(this);
    cb->addItem(tr("contains"), QConditionalDecoration::Contains);
    cb->addItem(tr("contains not"), QConditionalDecoration::ContainsNot);
    cb->addItem(tr("less"), QConditionalDecoration::IsLess);
    cb->addItem(tr("between"), QConditionalDecoration::IsBetween);
    cb->addItem(tr("greater"), QConditionalDecoration::IsGreater);
    cb->addItem(tr("less or equal"), QConditionalDecoration::IsLessOrEqual);
    cb->addItem(tr("greater or equal"), QConditionalDecoration::IsGreaterOrEqual);
    cb->addItem(tr("not between"), QConditionalDecoration::IsNotBetween);
    cb->addItem(tr("empty"), QConditionalDecoration::IsEmpty);
    cb->addItem(tr("not empty"), QConditionalDecoration::IsNotEmpty);
    cb->addItem(tr("equal"), QConditionalDecoration::IsEqual);
    cb->addItem(tr("not equal"), QConditionalDecoration::IsNotEqual);
    cb->addItem(tr("starts with"), QConditionalDecoration::StartsWith);
    cb->addItem(tr("ends with"), QConditionalDecoration::EndsWith);
    cb->addItem(tr("starts not with"), QConditionalDecoration::StartsNotWith);
    cb->addItem(tr("ends not with"), QConditionalDecoration::EndsNotWith);
    connect(cb, SIGNAL(activated(int)), this, SLOT(comparisonComboBoxActivated(int)));
    return cb;
}

void QConditionalDecorationDialog::iconSetComboBoxActivated(const QString & text)
{
    int row = -1;
    for (int iRow = 0; iRow < ui->conditionsTableWidget->rowCount(); iRow++){
		if (ui->conditionsTableWidget->cellWidget(iRow, COLUMN_SET) == sender()){
            row = iRow;
            break;
        }
    }
	ui->conditionsTableWidget->setCellWidget(row, COLUMN_ICON, iconSetComboBox(text));
}

void QConditionalDecorationDialog::removePushButtonClicked()
{
    ui->conditionsTableWidget->removeRow(ui->conditionsTableWidget->currentRow());
    ui->removePushButton->setEnabled(ui->conditionsTableWidget->rowCount() > 0);
}

QComboBox* QConditionalDecorationDialog::iconSetComboBox(const QString & name)
{
    if (!cIconSets.keys().contains(name)){
        return 0;
    }
    QComboBox* cb = new QComboBox(this);
    QMapIterator<QString,QVariant> it(cIconSets.value(name).toMap());
    while(it.hasNext()){
        it.next();
        cb->addItem(QIcon(it.value().toString()), it.key(), it.value());
    }
    return cb;
}

ValueEdit* QConditionalDecorationDialog::valueEdit()
{
    return new ValueEdit((QAbstractItemModel*)m_index.model(), m_defaultColumn, this);
}

QVariantMap QConditionalDecorationDialog::value(int row) const
{
    QVariantMap m;
	RangeEdit* re = qobject_cast<RangeEdit*>(ui->conditionsTableWidget->cellWidget(row, COLUMN_EDIT));
    if (re){
        m["from"] = re->rangeFrom();
        m["to"] = re->rangeTo();
    } else {
		ValueEdit* ve = qobject_cast<ValueEdit*>(ui->conditionsTableWidget->cellWidget(row, COLUMN_EDIT));
        if (ve){
            m["value"] = ve->value();
        }
    }
    return m;
}

SelectValueDialog::SelectValueDialog(QAbstractItemModel* model, int column, QWidget *parent) :
    QDialog(parent)
{
    setWindowIcon(QIcon(":/qaiv/dialog/table.select"));
    setWindowTitle(tr("Select Value..."));

    QVBoxLayout* layout = new QVBoxLayout(this);
	QHBoxLayout* hl = new QHBoxLayout();
	QLabel* l = new QLabel(this);
	l->setText(tr("Column:"));
	l->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	hl->addWidget(l);

	m_columnsComboBox = new QComboBox(this);
	for (int i = 0; i < model->columnCount(); i++){
		m_columnsComboBox->addItem(model->headerData(i, Qt::Horizontal).toString(), i);
	}
	m_columnsComboBox->setCurrentIndex(column);
	hl->addWidget(m_columnsComboBox);

	layout->addLayout(hl);
    m_view = new QTableView(this);
    layout->addWidget(m_view);
    m_view->horizontalHeader()->setStretchLastSection(true);
    m_view->setEditTriggers(QTableView::NoEditTriggers);
    m_view->verticalHeader()->setVisible(false);

    QSingleColumnProxyModel* columnProxy = new QSingleColumnProxyModel(this);
    columnProxy->setSourceModelColumn(column);
    columnProxy->setSourceModel(model);
	connect(m_columnsComboBox, SIGNAL(activated(int)), columnProxy, SLOT(setSourceModelColumn(int)));

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

int SelectValueDialog::selectedColumn() const
{
	return m_columnsComboBox->currentIndex();
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

int ValueEdit::column() const
{
	return m_column;
}

void ValueEdit::setColumn(int col)
{
	m_column = col;
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
    SelectValueDialog* d = new SelectValueDialog(m_model, m_column, this);
    if (d->exec()){
		m_column = d->selectedColumn();
        m_edit->setText(d->selectedValue().toString());
    }
    delete d;
}

RangeEdit::RangeEdit(QAbstractItemModel* model, int column, QWidget* parent) :
    QWidget(parent)
{
    m_model = model;
    m_column = column;

    QHBoxLayout * l = new QHBoxLayout(this);
    l->setContentsMargins(0, 0, 0, 0);

    m_fromEdit = new ValueEdit(model, column, this);
//    cFromEdit->layout()->setContentsMargins(9, 0, 0, 0);
    l->addWidget(m_fromEdit);


    l->addWidget(new QLabel(tr("and"), this));

    m_toEdit = new ValueEdit(model, column, this);
//    cToEdit->layout()->setContentsMargins(0, 0, 0, 0);
    l->addWidget(m_toEdit);

}

void RangeEdit::setColumn(int col)
{
	m_column = col;
}

void RangeEdit::setRange(const QVariant & from, const QVariant & to)
{
    m_fromEdit->setValue(from);
    m_toEdit->setValue(to);
}

QVariant RangeEdit::rangeFrom() const
{
    return m_fromEdit->value();
}

QVariant RangeEdit::rangeTo() const
{
    return m_toEdit->value();
}
