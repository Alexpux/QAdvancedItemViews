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
#ifndef QREGEXPFILTER_P_H
#define QREGEXPFILTER_P_H

#include <QWidget>

class QCheckBox;
class QComboBox;
class QLineEdit;
class QToolButton;
class QRegExp;
class QFrame;

class QClickableLabel;

class QRegExpFilterEditor : public QWidget
{
    Q_OBJECT
public:
    QRegExpFilterEditor(QWidget* parent = nullptr);
    ~QRegExpFilterEditor();

    Qt::CaseSensitivity caseSenstivity() const;

    QString pattern() const;

    QRegExp::PatternSyntax patternSyntax() const;

    void setCaseSensitivity(Qt::CaseSensitivity caseSenstivity);

    void setPattern(const QString & pattern);

    void setPatternSyntax(QRegExp::PatternSyntax patternSyntax);
private slots:
    void sensitivityLabelClicked(Qt::MouseButtons buttons);
private:
    Qt::CaseSensitivity m_sensitivity;
    QLineEdit* m_patternEdit;
    QCheckBox* m_wildcardCheckBox;
    QClickableLabel* m_sensitivityLabel;
};

#endif // QREGEXPFILTER_P_H
