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
#ifndef QSPECIALFILTER_P_H
#define QSPECIALFILTER_P_H

#include <QLineEdit>
#include <qfiltereditorwidget.h>

#include <qclickablelabel.h>

class QSpecialFilterEditor : public QFilterEditorWidget
{
    Q_OBJECT
public:
    QSpecialFilterEditor(QWidget* parent = 0);
    ~QSpecialFilterEditor();

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

#endif // QSPECIALFILTER_P_H
