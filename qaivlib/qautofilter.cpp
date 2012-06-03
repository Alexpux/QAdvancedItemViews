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
#include "qautofilter.h"

#include <qabstractfiltermodel.h>
#include <qautofilter_p.h>
#include <qcheckstateproxymodel.h>
#include <quniquevaluesproxymodel.h>
#include "qsinglecolumnproxymodel.h"

QAutoFilterEditor::QAutoFilterEditor(QWidget *parent) :
    QWidget(parent)
{
    m_checkStateProxy = new QCheckStateProxyModel(this);
    m_singleColumnProxy = new QSingleColumnProxyModel(this);
    m_singleValueProxy = new QUniqueValuesProxyModel(this);

    m_singleColumnProxy->setSourceModel(m_singleValueProxy);
    m_checkStateProxy->setSourceModel(m_singleColumnProxy);

    QVBoxLayout* mLayout = new QVBoxLayout();
    mLayout->setContentsMargins(0, 0, 0, 0);

    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setMinimumWidth(200);
    m_lineEdit->setPlaceholderText(tr("Search for"));
    connect(m_lineEdit, SIGNAL(textEdited(QString)), this, SLOT(searchForTextEdited(QString)));

    mLayout->addWidget(m_lineEdit);
    m_listView = new QListView(this);
    mLayout->addWidget(m_listView);
    setLayout(mLayout);

    m_listView->setModel(m_checkStateProxy);

    setFocusPolicy(Qt::StrongFocus);
}

void QAutoFilterEditor::setSourceModel(QAbstractItemModel *model, int column)
{
    m_singleValueProxy->setModelColumn(column);
    m_singleValueProxy->setSourceModel(model);
    m_singleColumnProxy->setSourceModelColumn(column);
    m_singleColumnProxy->sort(0);
}

void QAutoFilterEditor::setSelectedValues(const QVariantList & values)
{
    m_checkStateProxy->setCheckedValues(0, values);
}

void QAutoFilterEditor::searchForTextEdited(const QString & text)
{
    QModelIndexList mIndexes = m_checkStateProxy->match(m_checkStateProxy->index(0,0), Qt::DisplayRole, text);
    if (!mIndexes.isEmpty()){
        m_listView->setCurrentIndex(mIndexes.first());
    }
}

QVariantList QAutoFilterEditor::selectedValues(int role) const
{
    QVariantList v;
    Q_FOREACH(QModelIndex mIndex, m_checkStateProxy->checkedIndexes()){
        v.append(mIndex.data(role));
    }
    return v;
}

QAutoFilter::QAutoFilter(int row, int column) :
    QAbstractFilter(QAutoFilter::Type, row, column)
{
}

QWidget* QAutoFilter::createEditor( QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return new QAutoFilterEditor(parent);
}

QVariant QAutoFilter::data(int role) const
{
    if (role == Qt::DisplayRole){
        if (property("selectedValues").toList().isEmpty()){
            return QObject::tr("<none>");
        } else {
            return QString(QObject::tr("%1 entries")).arg(property("selectedValues").toList().size());
        }
    }
    return QVariant();
}

bool QAutoFilter::matches(const QVariant & value, int type) const
{
    Q_UNUSED(type);
    return property("selectedValues").toList().contains(value);
}

void QAutoFilter::setEditorData(QWidget* editor, const QModelIndex & index)
{
    QAutoFilterEditor* w = qobject_cast<QAutoFilterEditor*>(editor);
    if (w){
        QAbstractFilterModel* m = qobject_cast<QAbstractFilterModel*>((QAbstractItemModel*)index.model());
        if (m){
            w->setSourceModel(m->sourceModel(), column());
            w->setSelectedValues(property("selectedValues").toList());
        }
    }
}

void QAutoFilter::setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index)
{
    QAutoFilterEditor* w = qobject_cast<QAutoFilterEditor*>(editor);
    if (w){
        QVariantMap mProperties(index.data(Qt::EditRole).toMap());
        mProperties["selectedValues"] = w->selectedValues();
        model->setData(index, mProperties);
    }
}

