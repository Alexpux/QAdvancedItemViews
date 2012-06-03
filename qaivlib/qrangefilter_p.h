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
#ifndef QRANGEFILTER_P_H
#define QRANGEFILTER_P_H

#include <QWidget>

class QLineEdit;

class QRangeFilterEditor : public QWidget
{
    Q_OBJECT
public:
    QRangeFilterEditor( QWidget* parent = 0 );
    ~QRangeFilterEditor();

    QString rangeFrom() const;

    QString rangeTo() const;

    void setRangeFrom( const QString & text );

    void setRangeTo( const QString & text );
private:
    QLineEdit* m_rangeFrom;
    QLineEdit* m_rangeTo;
};

#endif // QRANGEFILTER_P_H
