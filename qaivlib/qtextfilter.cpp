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
#include "qtextfilter.h"
#include "qtextfilter_p.h"

#include <QHBoxLayout>

QTextFilterEditor::QTextFilterEditor(QWidget* parent) :
QWidget(parent)
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
	setAutoFillBackground(true);
}

QTextFilterEditor::~QTextFilterEditor()
{
}

Qt::CaseSensitivity QTextFilterEditor::caseSensitivity() const
{
    return cSensitivity;
}

Qt::MatchFlag QTextFilterEditor::matchFlag() const
{
    return cMatchFlag;
}

void QTextFilterEditor::matchFlagsLabelClicked(Qt::MouseButtons buttons)
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

void QTextFilterEditor::sensitivityLabelClicked(Qt::MouseButtons buttons)
{
    if (buttons.testFlag(Qt::LeftButton)){
        if (cSensitivity == Qt::CaseInsensitive){
            setCaseSensitivity(Qt::CaseSensitive);
        } else {
            setCaseSensitivity(Qt::CaseInsensitive);
        }
    }
}

void QTextFilterEditor::setCaseSensitivity(Qt::CaseSensitivity sensitivity)
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

void QTextFilterEditor::setMatchFlag(Qt::MatchFlag flag)
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

void QTextFilterEditor::setText(const QString & text)
{
    cValueLineEdit->setText(text);
}

QString QTextFilterEditor::text() const
{
    return cValueLineEdit->text();
}

QTextFilter::QTextFilter(int row, int column) :
    QAbstractFilter(QTextFilter::Type, row, column)
{
    setProperty("caseSensitivity", Qt::CaseInsensitive);
    setProperty("matchFlag", Qt::MatchContains);
}

QWidget* QTextFilter::createEditor(QFilterViewItemDelegate* delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return new QTextFilterEditor(parent);
}

void QTextFilter::addContextMenuActions(QMenu* menu, QWidget* receiver)
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

QVariant QTextFilter::data(int role) const
{
    if (role == Qt::DisplayRole){
        return QString("%1").arg(property("value").toString());
    }
    return QVariant();
}

bool QTextFilter::matches(const QVariant & value, int type) const
{
    Q_UNUSED(type);

    Qt::CaseSensitivity mSensitivity;
    Qt::MatchFlag mFlag;
    mSensitivity = static_cast<Qt::CaseSensitivity>(property("caseSensitivity", Qt::CaseInsensitive).toInt());
    mFlag = static_cast<Qt::MatchFlag>(property("matchFlag", Qt::MatchStartsWith).toInt());
    if (mFlag == Qt::MatchContains){
        return value.toString().contains(property("value").toString(), mSensitivity);
    } else if (mFlag == Qt::MatchEndsWith){
        return QString::compare(value.toString().right(property("value").toString().length()), property("value").toString(), mSensitivity) == 0;
    }
    return QString::compare(value.toString().left(property("value").toString().length()), property("value").toString(), mSensitivity) == 0;
}

void QTextFilter::setEditorData(QWidget * editor, const QModelIndex & index)
{
    QTextFilterEditor* e = qobject_cast<QTextFilterEditor*>(editor);
    if (e){
        QVariantMap p = index.data(Qt::EditRole).toMap();
        e->setText(p.value("value").toString());
        e->setMatchFlag(static_cast<Qt::MatchFlag>(p.value("matchFlag").toInt()));
        e->setCaseSensitivity(static_cast<Qt::CaseSensitivity>(p.value("caseSensitivity").toInt()));
    }
}

void QTextFilter::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex & index)
{
    QTextFilterEditor* e = qobject_cast<QTextFilterEditor*>(editor);
    if (e){
        QVariantMap p(index.data(Qt::EditRole).toMap());
        p["value"] = e->text();
        p["caseSensitivity"] = e->caseSensitivity();
        p["matchFlag"] = e->matchFlag();
		if (property("enableOnCommit").toBool()){
			p["enabled"] = true;
		}
        model->setData(index, p);
    }
}

void QTextFilter::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index)
{
	editor->setGeometry(option.rect);
}

QDebug operator<<(QDebug d, const QTextFilter & f)
{
    d << "(QValueFilter:"
      << "row:" << f.row()
      << "column:" << f.column()
      << "enabled:" << f.isEnabled()
      << "text:" << f.property("value").toString()
	  << "matchFlag" << static_cast<Qt::MatchFlag>(f.property("matchFlag").toInt())
      << ")";
    return d.space();
}
