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
#include "qselectionlistfilter.h"
#include "qselectionlistfilter_p.h"

QSelectionListFilter::QSelectionListFilter(int row, int column) :
    QAbstractFilter(QSelectionListFilter::Type, row, column)
{
}

QWidget* QSelectionListFilter::createEditor(QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return new QSelectionListFilterEditor(parent);
}

QVariant QSelectionListFilter::data(int role) const
{
    if (role == Qt::DisplayRole){
        QStringList mList;
        Q_FOREACH(QVariant mValue, property("selectedValues").toList()){
            mList.append(mValue.toString());
        }
        if (mList.isEmpty()){
            return QObject::tr("<none>");
        }
        return mList.join("; ");
    }
    return QVariant();
}

bool QSelectionListFilter::matches(const QVariant & value, int type) const
{
    return property("selectedValues").toList().contains(value);
}

void QSelectionListFilter::setEditorData(QWidget* editor, const QModelIndex & index)
{
    QSelectionListFilterEditor* mWidget = qobject_cast<QSelectionListFilterEditor*>(editor);
    if (mWidget){
        mWidget->setValues(property("values").toList());
        mWidget->setSelectedValues(property("selectedValues").toList());
    }
}

void QSelectionListFilter::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex & index)
{
    QSelectionListFilterEditor* mWidget = qobject_cast<QSelectionListFilterEditor*>(editor);
    if (mWidget){
        QVariantMap mProperties(index.data(Qt::EditRole).toMap());
        mProperties["selectedValues"] = mWidget->selectedValues();
        model->setData(index, mProperties);
    }
}

void QSelectionListFilter::setValues(const QVariantList &values)
{
    setProperty("values", values);
}

QSelectionListFilterEditor::QSelectionListFilterEditor(QWidget* parent) :
    QWidget(parent)
{
    cModel = new QStandardItemModel(this);
    cCheckStateProxy = new QCheckStateProxyModel(this);
    cCheckStateProxy->setSourceModel(cModel);

    QVBoxLayout* mLayout = new QVBoxLayout();
    mLayout->setContentsMargins(0, 0, 0, 0);

    cLineEdit = new QLineEdit(this);
    cLineEdit->setMinimumWidth(200);
    cLineEdit->setPlaceholderText(tr("Search pattern"));
    connect(cLineEdit, SIGNAL(textEdited(QString)), this, SLOT(searchForTextEdited(QString)));

    mLayout->addWidget(cLineEdit);
    cListView = new QListView(this);
    mLayout->addWidget(cListView);
    setLayout(mLayout);

    cListView->setModel(cCheckStateProxy);
}

void QSelectionListFilterEditor::searchForTextEdited(const QString & text)
{
    QModelIndexList mIndexes = cCheckStateProxy->match(cCheckStateProxy->index(0,0), Qt::DisplayRole, text);
    if (!mIndexes.isEmpty()){
        cListView->setCurrentIndex(mIndexes.first());
    }
}

void QSelectionListFilterEditor::setValues(const QVariantList & values)
{
    cModel->clear();
    cModel->setRowCount(values.size());
    cModel->setColumnCount(1);
    for (int iValue = 0; iValue < values.size(); iValue++){
        cModel->setItem(iValue, 0, new QStandardItem(values.at(iValue).toString()));
    }
}

QVariantList QSelectionListFilterEditor::selectedValues() const
{
    QVariantList mValues;
    Q_FOREACH(QModelIndex mIndex, cCheckStateProxy->checkedIndexes()){
        mValues.append(mIndex.data());
    }
    return mValues;
}

void QSelectionListFilterEditor::setSelectedValues(const QVariantList & selectedValues)
{
    cCheckStateProxy->setCheckedValues(0, selectedValues);
}
