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
#ifndef QHEADERVIEWCONNECTOR_H
#define QHEADERVIEWCONNECTOR_H

#include <QHeaderView>
#include <QObject>
#include <QPointer>

#include <qaivlib_global.h>

//! The QHeaderViewConnector class provides a connector to align QHeaderViews.
class QAIVLIBSHARED_EXPORT QHeaderViewConnector : public QObject
{
    Q_OBJECT
public:
    /**
      * Constructs a QHeaderViewConnector for the given @p orientation.
      */
    explicit QHeaderViewConnector(Qt::Orientation orientation, QObject *parent = 0);
    /**
      * Adds the given header @p view to the lists of aligned header views.
      */
    void append(QHeaderView* view);
signals:
    
public slots:
    void adjustSectionSize();
private slots:
    void scrollBarRangeChanged(int min, int max);
    void scrollBarSilderMoved(int value);
    void sectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
    void sectionResized(int logicalIndex, int oldSize, int newSize);
    void sortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void viewDestroyed(QObject* object);
private:
    bool cAdjustSectionSizePending;
    QList<QPointer< QHeaderView> > cHeaderViewList;
    Qt::Orientation cOrientation;
};

#endif // QHEADERVIEWCONNECTOR_H
