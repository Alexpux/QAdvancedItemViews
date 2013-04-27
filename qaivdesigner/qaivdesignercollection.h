/******************************************************************************
** This file is part of QAIV (QAdvanced Item Views).
**
** Copyright (c) 2011-2013 Martin Hoppe martin@2x2hoppe.de
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
#ifndef QAIVDESIGNERCOLLECTION_H
#define QAIVDESIGNERCOLLECTION_H

#include <QtDesigner/QtDesigner>
#include <QtCore/qplugin.h>

class QAIVDesignerCollection : public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
public:
    explicit QAIVDesignerCollection(QObject *parent = 0);
    virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;
signals:
    
public slots:
private:
     QList<QDesignerCustomWidgetInterface*> m_widgets;
};

#endif // QAIVDESIGNERCOLLECTION_H
