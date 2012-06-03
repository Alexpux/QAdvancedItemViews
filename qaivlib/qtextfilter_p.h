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
#ifndef QTEXTFILTER_P_H
#define QTEXTFILTER_P_H

#include <QLineEdit>
#include <QWidget>

#include <qclickablelabel.h>

class QTextFilterEditor : public QWidget
{
    Q_OBJECT
public:
    QTextFilterEditor(QWidget* parent = 0);
    ~QTextFilterEditor();

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
    Qt::CaseSensitivity cSensitivity;
    Qt::MatchFlag cMatchFlag;

    QLineEdit* cValueLineEdit;
    QClickableLabel* cSensitivityLabel;
    QClickableLabel* cMatchFlagsLabel;
};

#endif // QTEXTFILTER_P_H
