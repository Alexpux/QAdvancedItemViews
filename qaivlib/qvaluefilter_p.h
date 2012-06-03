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
#ifndef QVALUEFILTER_P_H
#define QVALUEFILTER_P_H

#include <QWidget>

class QClickableLabel;
class QLineEdit;

class QValueFilterEditor : public QWidget
{
    Q_OBJECT
public:
    QValueFilterEditor(QWidget* parent = 0);
    ~QValueFilterEditor();

    Qt::CaseSensitivity caseSensitivity() const;

    Qt::MatchFlag matchFlag() const;

    void setCaseSensitivity(Qt::CaseSensitivity sensitivity);

    void setMatchFlag(Qt::MatchFlag flag);

    void setText(const QString & text);

    QString text() const;
private slots:
    void sensitivityLabelClicked(Qt::MouseButtons buttons);
    void matchFlagsLabelClicked(Qt::MouseButtons buttons);
private:
    Qt::CaseSensitivity m_sensitivity;
    Qt::MatchFlag m_matchFlag;

    QLineEdit* m_valueLineEdit;
    QClickableLabel* m_sensitivityLabel;
    QClickableLabel* m_matchFlagsLabel;
};

#endif // QVALUEFILTER_P_H
