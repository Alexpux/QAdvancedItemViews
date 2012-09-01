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
#include "qvaluefilter.h"

#include <QDate>
#include <QDateTime>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTime>

#include <qclickablelabel.h>
#include <qvaluefilter_p.h>

QValueFilterEditor::QValueFilterEditor(QWidget* parent) :
QWidget(parent)
{
    QHBoxLayout* mLayout = new QHBoxLayout(this);
    mLayout->setSpacing(0);
    mLayout->setContentsMargins(0, 0, 0, 0);

    m_valueLineEdit = new QLineEdit(this);
    mLayout->addWidget(m_valueLineEdit);

    m_sensitivityLabel = new QClickableLabel(this);
    connect(m_sensitivityLabel, SIGNAL(clicked(Qt::MouseButtons)), this, SLOT(sensitivityLabelClicked(Qt::MouseButtons)));
    mLayout->addWidget(m_sensitivityLabel);

    m_matchFlagsLabel = new QClickableLabel(this);
    connect(m_matchFlagsLabel, SIGNAL(clicked(Qt::MouseButtons)), this, SLOT(matchFlagsLabelClicked(Qt::MouseButtons)));
    mLayout->addWidget(m_matchFlagsLabel);

    setFocusProxy(m_valueLineEdit);

    setMatchFlag(Qt::MatchContains);
    setCaseSensitivity(Qt::CaseInsensitive);

    setFocusPolicy(Qt::StrongFocus);
}

QValueFilterEditor::~QValueFilterEditor()
{
}

Qt::CaseSensitivity QValueFilterEditor::caseSensitivity() const
{
    return m_sensitivity;
}

Qt::MatchFlag QValueFilterEditor::matchFlag() const
{
    return m_matchFlag;
}

void QValueFilterEditor::matchFlagsLabelClicked(Qt::MouseButtons buttons)
{
    if (buttons.testFlag(Qt::LeftButton)){
        if (m_matchFlag == Qt::MatchStartsWith){
            setMatchFlag(Qt::MatchEndsWith);
        } else if (m_matchFlag == Qt::MatchEndsWith){
            setMatchFlag(Qt::MatchContains);
        } else if (m_matchFlag == Qt::MatchContains){
            setMatchFlag(Qt::MatchStartsWith);
        }
    }
}

void QValueFilterEditor::sensitivityLabelClicked(Qt::MouseButtons buttons)
{
    if (buttons.testFlag(Qt::LeftButton)){
        if (m_sensitivity == Qt::CaseInsensitive){
            setCaseSensitivity(Qt::CaseSensitive);
        } else {
            setCaseSensitivity(Qt::CaseInsensitive);
        }
    }
}
void QValueFilterEditor::setCaseSensitivity(Qt::CaseSensitivity sensitivity)
{
    if (sensitivity == Qt::CaseSensitive){
        m_sensitivityLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/case_sensitive")));
        m_sensitivityLabel->setToolTip(tr("The filter is case sensitive"));
    } else {
        m_sensitivityLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/case_insensitive")));
        m_sensitivityLabel->setToolTip(tr("The filter is case insensitive"));
    }
    m_sensitivity = sensitivity;
}

void QValueFilterEditor::setMatchFlag(Qt::MatchFlag flag)
{
    if (flag == Qt::MatchEndsWith){
        m_matchFlagsLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/ends_with")));
        m_matchFlagsLabel->setToolTip(tr("The filter value matches the end of the item"));
    } else if (flag == Qt::MatchContains){
        m_matchFlagsLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/contains")));
        m_matchFlagsLabel->setToolTip(tr("The filter value is contained in the item"));
    } else if (flag == Qt::MatchStartsWith){
        m_matchFlagsLabel->setPixmap(QPixmap(QString::fromUtf8(":/qaiv/filter/starts_with")));
        m_matchFlagsLabel->setToolTip(tr("The filter value matches the start of the item"));
    }
    m_matchFlag = flag;
}

void QValueFilterEditor::setText(const QString & text)
{
    m_valueLineEdit->setText(text);
}

QString QValueFilterEditor::text() const
{
    return m_valueLineEdit->text();
}

QValueFilter::QValueFilter(int row, int column) :
    QAbstractFilter(QValueFilter::Type, row, column)
{
    setProperty("caseSensitivity", Qt::CaseInsensitive);
}

QValueFilter::~QValueFilter()
{

}

QWidget* QValueFilter::createEditor(QFilterViewItemDelegate* delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
	Q_UNUSED(delegate);
	Q_UNUSED(index);
    return new QValueFilterEditor(parent);
}

QVariant QValueFilter::data(int role) const
{
    if (role == Qt::DisplayRole){
        return QString("%1").arg(property("value").toString());
    }
    return QVariant();
}

bool QValueFilter::matches(const QVariant & value, int type) const
{
    Qt::CaseSensitivity s;
    Qt::MatchFlag mf;
    int t = type;
    if (t == -1){
        t = value.type();
    }
    switch(t)
    {
    case QVariant::Char:
        return value.toChar() == property("value").toChar();
    case QVariant::Date:
        return value.toDate() == property("value").toDate();
    case QVariant::DateTime:
        return value.toDateTime() == property("value").toDateTime();
    case QVariant::Double:
        return value.toDouble() == property("value").toDouble();
    case QVariant::Int:
        return value.toInt() == property("value").toInt();
    case QVariant::LongLong:
        return value.toLongLong() == property("value").toLongLong();
    case QVariant::String:
        s = static_cast<Qt::CaseSensitivity>(property("caseSensitivity", Qt::CaseInsensitive).toInt());
        mf = static_cast<Qt::MatchFlag>(property("matchFlag", Qt::MatchStartsWith).toInt());
        if (mf == Qt::MatchContains){
            return value.toString().contains(property("value").toString(), s);
        } else if (mf == Qt::MatchEndsWith){
            return QString::compare(value.toString().right(property("value").toString().length()), property("value").toString(), s) == 0;
        }
        return QString::compare(value.toString().left(property("value").toString().length()), property("value").toString(), s) == 0;
    case QVariant::Time:
        return value.toTime() == property("value").toTime();
    case QVariant::UInt:
        return value.toUInt() == property("value").toUInt();
    case QVariant::ULongLong:
        return value.toULongLong() == property("value").toULongLong();
    default:
        return value == property("value");
    }
    return false;
}

void QValueFilter::setEditorData(QWidget * editor, const QModelIndex & index)
{
    QValueFilterEditor* w = qobject_cast<QValueFilterEditor*>(editor);
    if (w){
        QVariantMap p = index.data(Qt::EditRole).toMap();
        w->setText(p.value("value").toString());
        w->setMatchFlag(static_cast<Qt::MatchFlag>(p.value("matchFlag").toInt()));
        w->setCaseSensitivity(static_cast<Qt::CaseSensitivity>(p.value("caseSensitivity").toInt()));
    }
}

void QValueFilter::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex & index)
{
    QValueFilterEditor* w = qobject_cast<QValueFilterEditor*>(editor);
    if (w){
        QVariantMap p(index.data(Qt::EditRole).toMap());
        p["value"] = w->text();
        p["caseSensitivity"] = w->caseSensitivity();
        p["matchFlag"] = w->matchFlag();
		if (property("enableOnCommit").toBool()){
			p["enabled"] = true;
		}
        model->setData(index, p);
    }
}

void QValueFilter::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index)
{
	editor->setGeometry(option.rect);
}

QDebug operator<<(QDebug d, const QValueFilter & f)
{
    d << "(QValueFilter:"
      << "row:" << f.row()
      << "column:" << f.column()
      << "enabled:" << f.isEnabled()
      << "value:" << f.property("value")
	  << "matchFlag" << static_cast<Qt::MatchFlag>(f.property("matchFlag").toInt())
      << ")";
    return d.space();
}
