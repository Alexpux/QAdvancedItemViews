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

#include "qselectionlistfilter.h"

#include "qaiv.h"
#include "qfiltermodel.h"
#include "qfilterviewitemdelegate.h"
#include "qselectionlistfilter_p.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QToolButton>
#include <QVBoxLayout>

QSelectionListFilterEditorPopup::QSelectionListFilterEditorPopup(QWidget *parent) :
    QFilterEditorPopupWidget(parent)
{
    m_model = new QStandardItemModel(this);
    m_checkStateProxy = new QCheckStateProxyModel(this);
    connect(m_checkStateProxy, &QCheckStateProxyModel::dataChanged, this, &QSelectionListFilterEditorPopup::checkStateProxyDataChanged);
    m_checkStateProxy->setSourceModel(m_model);
    m_checkStateProxy->setColumnCheckable(0);

    auto *l = new QVBoxLayout();
    l->setContentsMargins(6, 6, 6, 6);

    auto *lb = new QVBoxLayout();
    lb->setSpacing(3);
    m_emptyToolButton = new QToolButton(this);
    m_emptyToolButton->setText(tr("Empty"));
    m_emptyToolButton->setAutoRaise(true);
    m_emptyToolButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    lb->addWidget(m_emptyToolButton);
    connect(m_emptyToolButton, &QToolButton::clicked, this, &QSelectionListFilterEditorPopup::emptyToolButtonClicked);

    m_notEmptyToolButton = new QToolButton(this);
    m_notEmptyToolButton->setText(tr("Not Empty"));
    m_notEmptyToolButton->setAutoRaise(true);
    m_notEmptyToolButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    lb->addWidget(m_notEmptyToolButton);
    l->addLayout(lb);
    connect(m_notEmptyToolButton, &QToolButton::clicked, this, &QSelectionListFilterEditorPopup::notEmptyToolButtonClicked);

    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setMinimumWidth(200);
    m_lineEdit->setPlaceholderText(tr("Search pattern"));
    m_lineEdit->installEventFilter(parent);
    connect(m_lineEdit, &QLineEdit::textEdited, this, &QSelectionListFilterEditorPopup::searchForTextEdited);

    l->addWidget(m_lineEdit);
    m_listView = new QListView(this);
    l->addWidget(m_listView);

    m_selectCheckBox = new QCheckBox(this);
    m_selectCheckBox->setText(tr("Select/Deselect all"));
    m_selectCheckBox->setTristate(true);
    m_selectCheckBox->installEventFilter(parent);
    connect(m_selectCheckBox, &QCheckBox::stateChanged, this, &QSelectionListFilterEditorPopup::selectCheckBoxStateChanged);

    l->addWidget(m_selectCheckBox);

    setLayout(l);

    m_listView->setModel(m_checkStateProxy);
    setFocusProxy(m_listView);
    m_listView->setFocus();
    m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_listView->installEventFilter(parent);

    auto *b = new QDialogButtonBox(this);
    b->addButton(QDialogButtonBox::Ok);
    b->addButton(QDialogButtonBox::Cancel);
    connect(b, &QDialogButtonBox::accepted, this, &QSelectionListFilterEditorPopup::accepted);
    connect(b, &QDialogButtonBox::rejected, this, &QSelectionListFilterEditorPopup::rejected);
    l->addWidget(b);
}

void QSelectionListFilterEditorPopup::checkStateProxyDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);
    m_selectCheckBox->blockSignals(true);
    if (m_checkStateProxy->checkedIndexes().size() == 0) {
        m_selectCheckBox->setCheckState(Qt::Unchecked);
    } else if (m_checkStateProxy->checkedIndexes().size() == (m_checkStateProxy->checkableColumnsCount() * m_checkStateProxy->rowCount())) {
        m_selectCheckBox->setCheckState(Qt::Checked);
    } else {
        m_selectCheckBox->setCheckState(Qt::PartiallyChecked);
    }
    m_selectCheckBox->blockSignals(false);
}

bool QSelectionListFilterEditorPopup::cancelAndClose(QObject *obj, int key) const
{
    Q_UNUSED(obj);
    if (key == Qt::Key_Escape) {
        return true;
    }
    return false;
}

bool QSelectionListFilterEditorPopup::commitAndClose(QObject *obj, int key) const
{
    if (key == Qt::Key_Return) {
        if (obj == m_listView) {
            return true;
        }
    }
    return false;
}

void QSelectionListFilterEditorPopup::emptyToolButtonClicked()
{
    m_mode = 1;
    emit modeChanged();
}

int QSelectionListFilterEditorPopup::mode() const
{
    return m_mode;
}

void QSelectionListFilterEditorPopup::notEmptyToolButtonClicked()
{
    m_mode = 2;
    emit modeChanged();
}

void QSelectionListFilterEditorPopup::searchForTextEdited(const QString &text)
{
    QModelIndexList i = m_checkStateProxy->match(m_checkStateProxy->index(0, 0), Qt::DisplayRole, text);
    if (!i.isEmpty()) {
        m_listView->setCurrentIndex(i.first());
    }
}

void QSelectionListFilterEditorPopup::selectCheckBoxStateChanged(int state)
{
    if (state == Qt::Checked) {
        m_checkStateProxy->setAllChecked(true);
    } else if (state == Qt::Unchecked) {
        m_checkStateProxy->setAllChecked(false);
    } else if (state == Qt::PartiallyChecked) {
        m_selectCheckBox->setChecked(true);
    }
}

QVariantList QSelectionListFilterEditorPopup::selectedValues() const
{
    QVariantList v;
    for (const QModelIndex &i : m_checkStateProxy->checkedIndexes()) {
        v.append(i.data());
    }
    return v;
}

void QSelectionListFilterEditorPopup::setValues(const QVariantList &values)
{
    m_model->clear();
    m_model->setRowCount(values.size());
    m_model->setColumnCount(1);
    for (int iValue = 0; iValue < values.size(); iValue++) {
        m_model->setItem(iValue, 0, new QStandardItem(values.at(iValue).toString()));
    }
}

void QSelectionListFilterEditorPopup::setSelectedValues(const QVariantList &selectedValues)
{
    m_checkStateProxy->setCheckedValues(0, selectedValues);
}

QSelectionListFilterEditor::QSelectionListFilterEditor(QWidget *parent) :
    QFilterEditorWidget(parent)
{
    auto *popUp = new QSelectionListFilterEditorPopup(this);
    setPopup(popUp);
    setFocusProxy(popUp);
    connect(popUp, &QSelectionListFilterEditorPopup::modeChanged, this, &QSelectionListFilterEditor::modeSelected);
    connect(popUp, &QSelectionListFilterEditorPopup::accepted, this, [this]() { emit commitAndClose(); });
    connect(popUp, &QSelectionListFilterEditorPopup::rejected, this, [this]() { emit cancelAndClose(); });
}

void QSelectionListFilterEditor::modeSelected()
{
    emit commitAndClose();
}

QSelectionListFilter::QSelectionListFilter(int row, int column) :
    QAbstractFilter(QSelectionListFilter::Type, row, column)
{
    setProperty("dataSource", QSelectionListFilter::Model);
}

QWidget *QSelectionListFilter::createEditor(QFilterViewItemDelegate *delegate, QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    auto *e = new QSelectionListFilterEditor(parent);
    QObject::connect(e, &QSelectionListFilterEditor::cancelAndClose, delegate, &QFilterViewItemDelegate::cancelAndClose);
    QObject::connect(e, &QSelectionListFilterEditor::commitAndClose, delegate, &QFilterViewItemDelegate::commitAndClose);
    return e;
}

QVariant QSelectionListFilter::data(int role) const
{
    if (role == Qt::DisplayRole) {
        if (property("mode").toInt() == 0) {
            if (property("selectedValues").toList().isEmpty()) {
                return QObject::tr("<none>");
            }
            if (property("selectedValues").toList().size() == 1) {
                return QString(QObject::tr("%1 entry")).arg(property("selectedValues").toList().size());
            }
            return QString(QObject::tr("%1 entries")).arg(property("selectedValues").toList().size());
        }
        if (property("mode").toInt() == 1) {
            return QObject::tr("Empty");
        }
        if (property("mode").toInt() == 2) {
            return QObject::tr("Not Empty");
        }
    }
    return QVariant();
}

QSelectionListFilter::DataSource QSelectionListFilter::dataSource() const
{
    return static_cast<QSelectionListFilter::DataSource>(property("dataSource").toInt());
}

bool QSelectionListFilter::matches(const QVariant &value, int type) const
{
    Q_UNUSED(type);
    if (property("mode").toInt() == 1) {
        return value.toString().isEmpty();
    }
    if (property("mode").toInt() == 2) {
        return !value.toString().isEmpty();
    }
    return property("selectedValues").toList().contains(value);
}

void QSelectionListFilter::setDataSource(QSelectionListFilter::DataSource source)
{
    setProperty("dataSource", source);
}

void QSelectionListFilter::setEditorData(QWidget *editor, const QModelIndex &index)
{
    Q_UNUSED(index);
    auto *w = qobject_cast<QSelectionListFilterEditor *>(editor);
    if (w) {
        auto *p = qobject_cast<QSelectionListFilterEditorPopup *>(w->popup());
        if (dataSource() == QSelectionListFilter::Filter) {
            p->setValues(property("values").toList());
        } else if (dataSource() == QSelectionListFilter::Model) {
            const auto *m = qobject_cast<const QFilterModel *>(index.model());
            if (m) {
                p->setValues(m->sourceModel()->index(0, index.column()).data(QAdvancedItemViews::SelectionListFilterDataRole).toList());
            }
        }
        p->setSelectedValues(property("selectedValues").toList());
    }
}

void QSelectionListFilter::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)
{
    Q_UNUSED(index);
    const QSelectionListFilterEditor *w = qobject_cast<QSelectionListFilterEditor *>(editor);
    if (w) {
        const QSelectionListFilterEditorPopup *p = qobject_cast<QSelectionListFilterEditorPopup *>(w->popup());
        QVariantMap properties(index.data(Qt::EditRole).toMap());
        properties["mode"] = p->mode();
        if (p->mode() > 0) {
            properties["selectedValues"] = QVariantList();
        } else {
            properties["selectedValues"] = p->selectedValues();
        }
        if (property("enableOnCommit").toBool()) {
            properties["enabled"] = true;
        }
        model->setData(index, properties);
    }
}

void QSelectionListFilter::setValues(const QVariantList &values)
{
    setProperty("values", values);
}

void QSelectionListFilter::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(index);
    auto *e = qobject_cast<QSelectionListFilterEditor *>(editor);
    if (e) {
        e->setGeometry(option.rect);
        e->showPopup();
    }
}
