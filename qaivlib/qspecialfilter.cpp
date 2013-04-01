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
#include "qspecialfilter.h"
#include "qspecialfilter_p.h"

#include <QHBoxLayout>

QSpecialFilterEditor::QSpecialFilterEditor(QWidget* parent) :
	QFilterEditorWidget(parent)
{
    QHBoxLayout* mLayout = new QHBoxLayout(this);
    mLayout->setSpacing(0);
    mLayout->setContentsMargins(0, 0, 0, 0);

    cValueLineEdit = new QLineEdit(this);
    mLayout->addWidget(cValueLineEdit);

    cSensitivityLabel = new QClickableLabel(this);
    connect(cSensitivityLabel, SIGNAL(clicked(Qt::MouseButtons)), this, SLOT(sensitivityLabelClicked(Qt::MouseButtons)));
    mLayout->addWidget(cSensitivityLabel);

    cMatchFlagsLabel = new QClickableLabel(this);
    connect(cMatchFlagsLabel, SIGNAL(clicked(Qt::MouseButtons)), this, SLOT(matchFlagsLabelClicked(Qt::MouseButtons)));
    mLayout->addWidget(cMatchFlagsLabel);

    setFocusProxy(cValueLineEdit);

    setMatchFlag(Qt::MatchStartsWith);
    setCaseSensitivity(Qt::CaseInsensitive);

    setFocusPolicy(Qt::StrongFocus);
}

QSpecialFilterEditor::~QSpecialFilterEditor()
{
}

Qt::CaseSensitivity QSpecialFilterEditor::caseSensitivity() const
{
    return cSensitivity;
}

Qt::MatchFlag QSpecialFilterEditor::matchFlag() const
{
    return cMatchFlag;
}

void QSpecialFilterEditor::matchFlagsLabelClicked(Qt::MouseButtons buttons)
{
    if (buttons.testFlag(Qt::LeftButton)){
        if (cMatchFlag == Qt::MatchStartsWith){
            setMatchFlag(Qt::MatchEndsWith);
        } else if (cMatchFlag == Qt::MatchEndsWith){
            setMatchFlag(Qt::MatchContains);
        } else if (cMatchFlag == Qt::MatchContains){
            setMatchFlag(Qt::MatchStartsWith);
        }
    }
}

void QSpecialFilterEditor::sensitivityLabelClicked(Qt::MouseButtons buttons)
{
    if (buttons.testFlag(Qt::LeftButton)){
        if (cSensitivity == Qt::CaseInsensitive){
            setCaseSensitivity(Qt::CaseSensitive);
        } else {
            setCaseSensitivity(Qt::CaseInsensitive);
        }
    }
}
void QSpecialFilterEditor::setCaseSensitivity(Qt::CaseSensitivity sensitivity)
{
    if (sensitivity == Qt::CaseSensitive){
        cSensitivityLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/case_sensitive")));
        cSensitivityLabel->setToolTip(tr("The filter is case sensitive"));
    } else {
        cSensitivityLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/case_insensitive")));
        cSensitivityLabel->setToolTip(tr("The filter is case insensitive"));
    }
    cSensitivity = sensitivity;
}

void QSpecialFilterEditor::setMatchFlag(Qt::MatchFlag flag)
{
    if (flag == Qt::MatchEndsWith){
        cMatchFlagsLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/ends_with")));
        cMatchFlagsLabel->setToolTip(tr("The filter value matches the end of the item"));
    } else if (flag == Qt::MatchContains){
        cMatchFlagsLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/contains")));
        cMatchFlagsLabel->setToolTip(tr("The filter value is contained in the item"));
    } else if (flag == Qt::MatchStartsWith){
        cMatchFlagsLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/starts_with")));
        cMatchFlagsLabel->setToolTip(tr("The filter value matches the start of the item"));
    }
    cMatchFlag = flag;
}

void QSpecialFilterEditor::setText(const QString & text)
{
    cValueLineEdit->setText(text);
}

QString QSpecialFilterEditor::text() const
{
    return cValueLineEdit->text();
}

QSpecialFilter::QSpecialFilter(int row, int column) :
    QAbstractFilter(QSpecialFilter::Type, row, column)
{
    setProperty("caseSensitivity", Qt::CaseInsensitive);
    setProperty("matchFlag", Qt::MatchContains);
}

QWidget* QSpecialFilter::createEditor(QFilterViewItemDelegate* delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return new QSpecialFilterEditor(parent);
}

void QSpecialFilter::addContextMenuActions(QMenu* menu, QWidget* receiver)
{
    QVariantMap mDefaultProperties;
    QVariantMap mPropertiesToChange;
    mDefaultProperties["row"] = property("row").toInt();
    mDefaultProperties["column"] = property("column").toInt();

    QAction* mAction = 0;
    mAction = menu->addAction(QIcon(":/qaiv/filter/case_insensitive"), QObject::tr("Case insensitive"), receiver, SLOT(changeProperties()));
    mAction->setCheckable(true);
    mAction->setChecked(property("caseSensitivity").toInt() == Qt::CaseInsensitive);
    mPropertiesToChange = QVariantMap(mDefaultProperties);
    mPropertiesToChange["caseSensitivity"] = Qt::CaseInsensitive;
    mAction->setData(mPropertiesToChange);

    mAction = menu->addAction(QIcon(":/qaiv/filter/case_sensitive"), QObject::tr("Case sensitive"), receiver, SLOT(changeProperties()));
    mAction->setCheckable(true);
    mAction->setChecked(property("caseSensitivity").toInt() == Qt::CaseSensitive);
    mPropertiesToChange = QVariantMap(mDefaultProperties);
    mPropertiesToChange["caseSensitivity"] = Qt::CaseSensitive;
    mAction->setData(mPropertiesToChange);
    menu->addSeparator();

    mAction = menu->addAction(QIcon(":/qaiv/filter/starts_with"), QObject::tr("Match starts with"), receiver, SLOT(changeProperties()));
    mAction->setCheckable(true);
    mAction->setChecked(property("matchFlag").toInt() == Qt::MatchStartsWith);
    mPropertiesToChange = QVariantMap(mDefaultProperties);
    mPropertiesToChange["matchFlag"] = Qt::MatchStartsWith;
    mAction->setData(mPropertiesToChange);

    mAction = menu->addAction(QIcon(":/qaiv/filter/ends_with"), QObject::tr("Match ends with"), receiver, SLOT(changeProperties()));
    mAction->setCheckable(true);
    mAction->setChecked(property("matchFlag").toInt() == Qt::MatchEndsWith);
    mPropertiesToChange = QVariantMap(mDefaultProperties);
    mPropertiesToChange["matchFlag"] = Qt::MatchEndsWith;
    mAction->setData(mPropertiesToChange);

    mAction = menu->addAction(QIcon(":/qaiv/filter/contains"), QObject::tr("Match contains"), receiver, SLOT(changeProperties()));
    mAction->setCheckable(true);
    mAction->setChecked(property("matchFlag").toInt() == Qt::MatchContains);
    mPropertiesToChange = QVariantMap(mDefaultProperties);
    mPropertiesToChange["matchFlag"] = Qt::MatchContains;
    mAction->setData(mPropertiesToChange);
}

QVariant QSpecialFilter::data(int role) const
{
    if (role == Qt::DisplayRole){
        return QString("%1").arg(property("value").toString());
    }
    return QVariant();
}

bool QSpecialFilter::matches(const QVariant & value, int type) const
{
    Q_UNUSED(type);

    QSpecialFilter::MatchFlag matchFlag;
    matchFlag = static_cast<QSpecialFilter::MatchFlag>(property("matchFlag", Qt::MatchStartsWith).toInt());
	if (matchFlag == QSpecialFilter::IsEmpty){
        return value.isNull();
	} else if (matchFlag == QSpecialFilter::IsNotEmpty){
        return !value.isNull();
    }
	return false;
    //return QString::compare(value.toString().left(property("value").toString().length()), property("value").toString(), mSensitivity) == 0;
}

QSpecialFilter::MatchFlag QSpecialFilter::matchFlag() const
{
	return static_cast<QSpecialFilter::MatchFlag>(property("matchFlag", Qt::MatchStartsWith).toInt());
}

void QSpecialFilter::setEditorData(QWidget * editor, const QModelIndex & index)
{
    QSpecialFilterEditor* mWidget = qobject_cast<QSpecialFilterEditor*>(editor);
    if (mWidget){
        QVariantMap mProperties = index.data(Qt::EditRole).toMap();
        mWidget->setText(mProperties.value("value").toString());
        mWidget->setMatchFlag(static_cast<Qt::MatchFlag>(mProperties.value("matchFlag").toInt()));
        mWidget->setCaseSensitivity(static_cast<Qt::CaseSensitivity>(mProperties.value("caseSensitivity").toInt()));
    }
}

void QSpecialFilter::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex & index)
{
    QSpecialFilterEditor* mWidget = qobject_cast<QSpecialFilterEditor*>(editor);
    if (mWidget){
        QVariantMap mProperties(index.data(Qt::EditRole).toMap());
        mProperties["value"] = mWidget->text();
        mProperties["caseSensitivity"] = mWidget->caseSensitivity();
        mProperties["matchFlag"] = mWidget->matchFlag();
        model->setData(index, mProperties);
    }
}

void QSpecialFilter::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index)
{
}
