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
#include "qregexpfilter.h"

#include <qregexpfilter_p.h>

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

QRegExpFilterEditor::QRegExpFilterEditor(QWidget* parent)
    : QWidget(parent)
{
    QGridLayout* l = new QGridLayout();

    m_pattern = new QLineEdit(this);
    m_pattern->setMinimumWidth(100);

    m_sensitivity = new QComboBox(this);
    m_sensitivity->addItem(tr("Case Insensitive"), Qt::CaseInsensitive);
    m_sensitivity->addItem(tr("Case Sensitive"), Qt::CaseSensitive);

    m_wildcard = new QCheckBox(tr("Wildcard"), this);

    l->addWidget(new QLabel(tr("Pattern"), this), 0, 0);
    l->addWidget(m_pattern, 0, 1, 1, 2);
    l->addWidget(new QLabel(tr("Sensitivity"), this), 1, 0);
    l->addWidget(m_sensitivity, 1, 1);
    l->addWidget(m_wildcard, 1, 2);

    setFocusProxy(m_pattern);
    //
//    setFocusPolicy(Qt::StrongFocus);
    setTabOrder(m_pattern, m_sensitivity);
    setTabOrder(m_sensitivity, m_wildcard);

    setLayout(l);
}

QRegExpFilterEditor::~QRegExpFilterEditor()
{
}

Qt::CaseSensitivity QRegExpFilterEditor::caseSenstivity() const
{
    return static_cast<Qt::CaseSensitivity>(m_sensitivity->itemData(m_sensitivity->currentIndex()).toInt());
}

QString QRegExpFilterEditor::pattern() const
{
    return m_pattern->text();
}

QRegExp::PatternSyntax QRegExpFilterEditor::patternSyntax() const
{
    if (m_wildcard->isChecked()){
        return QRegExp::Wildcard;
    }
    return QRegExp::RegExp;
}

void QRegExpFilterEditor::setCaseSensitivity(Qt::CaseSensitivity caseSenstivity)
{
    m_sensitivity->setCurrentIndex(m_sensitivity->findData(caseSenstivity));
}

void QRegExpFilterEditor::setPattern(const QString &pattern)
{
    m_pattern->setText(pattern);
}

void QRegExpFilterEditor::setPatternSyntax(QRegExp::PatternSyntax patternSyntax)
{
    m_wildcard->setChecked(patternSyntax == QRegExp::Wildcard);
}

QRegExpFilter::QRegExpFilter(int row, int column) :
    QAbstractFilter(QRegExpFilter::Type, row, column)
{
    setProperty("caseSensitivity", Qt::CaseInsensitive);
    // @todo change default for Qt5
    setProperty("patternSyntax", QRegExp::RegExp);
}

QWidget* QRegExpFilter::createEditor( QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return new QRegExpFilterEditor(parent);
}

QVariant QRegExpFilter::data(int role) const
{
    if (role == Qt::DisplayRole){
        return QString("%1").arg(property("pattern").toString());
    }
    return QVariant();
}

bool QRegExpFilter::matches(const QVariant & value, int type) const
{
    Q_UNUSED(type);
    QRegExp regExp(property("pattern").toString(), static_cast<Qt::CaseSensitivity>(property("caseSensitivity").toInt()), static_cast<QRegExp::PatternSyntax>(property("patternSyntax").toInt()));
    if (regExp.indexIn(value.toString()) == -1){
        return false;
    }
    return true;
}

void QRegExpFilter::setEditorData(QWidget * editor, const QModelIndex & index)
{
    QRegExpFilterEditor* w = qobject_cast<QRegExpFilterEditor*>(editor);
    if (w){
        w->setCaseSensitivity(static_cast<Qt::CaseSensitivity>(property("caseSensitivity").toInt()));
        w->setPattern(property("pattern").toString());
        w->setPatternSyntax(static_cast<QRegExp::PatternSyntax>(property("patternSyntax").toInt()));
    }
}

void QRegExpFilter::setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index)
{
    QRegExpFilterEditor* w = qobject_cast<QRegExpFilterEditor*>(editor);
    if (w){
        QVariantMap mProperties(index.data(Qt::EditRole).toMap());
        mProperties["pattern"] = w->pattern();
        mProperties["caseSensitivity"] = w->caseSenstivity();
        mProperties["patternSyntax"] = w->patternSyntax();
        model->setData(index, mProperties);
    }
}

QDebug operator<<(QDebug d, const QRegExpFilter & f)
{
    d << "(QRegExpFilter:"
      << "row:" << f.row()
      << "column:" << f.column()
      << "enabled:" << f.isEnabled()
      << "caseSensitivity:" << static_cast<Qt::CaseSensitivity>(f.property("caseSensitivity").toInt())
      << "patternSyntax" << static_cast<QRegExp::PatternSyntax>(f.property("patternSyntax").toInt())
      << "pattern:" << f.property("pattern").toString()
      << ")";
    return d.space();
}
