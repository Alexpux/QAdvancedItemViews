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
#include "qadvancedtableview.h"
#include "qadvancedtableviewplugin.h"

#include <QtCore/QtPlugin>

QAdvancedTableViewPlugin::QAdvancedTableViewPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QAdvancedTableViewPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool QAdvancedTableViewPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QAdvancedTableViewPlugin::createWidget(QWidget *parent)
{
    return new QAdvancedTableView(parent);
}

QString QAdvancedTableViewPlugin::name() const
{
    return QLatin1String("QAdvancedTableView");
}

QString QAdvancedTableViewPlugin::group() const
{
    return QLatin1String("QAdvancedItemViews");
}

QIcon QAdvancedTableViewPlugin::icon() const
{
    return QIcon(":/qaivdesigner/qadvancedtableview");
}

QString QAdvancedTableViewPlugin::toolTip() const
{
    return QLatin1String("");
}

QString QAdvancedTableViewPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QAdvancedTableViewPlugin::isContainer() const
{
    return false;
}

QString QAdvancedTableViewPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QAdvancedTableView\" name=\"tableView\">\n</widget>\n");
}

QString QAdvancedTableViewPlugin::includeFile() const
{
    return QLatin1String("qadvancedtableview.h");
}

//Q_EXPORT_PLUGIN2(qadvancedtableviewplugin, QAdvancedTableViewPlugin)
