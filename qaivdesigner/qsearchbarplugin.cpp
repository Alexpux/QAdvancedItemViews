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
#include "stdafx.h"
#include "qsearchbarplugin.h"
#include "qsearchbar.h"

#include <QtCore/QtPlugin>

QSearchBarPlugin::QSearchBarPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QSearchBarPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool QSearchBarPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget* QSearchBarPlugin::createWidget(QWidget *parent)
{
	return new QSearchBar(parent);
}

QString QSearchBarPlugin::name() const
{
    return QLatin1String("QSearchBar");
}

QString QSearchBarPlugin::group() const
{
    return QLatin1String("QAdvancedItemViews");
}

QIcon QSearchBarPlugin::icon() const
{
    return QIcon(":/qaivdesigner/qsearchbar");
}

QString QSearchBarPlugin::toolTip() const
{
    return QLatin1String("");
}

QString QSearchBarPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QSearchBarPlugin::isContainer() const
{
    return false;
}

QString QSearchBarPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QSearchBar\" name=\"searchBar\">\n</widget>\n");
}

QString QSearchBarPlugin::includeFile() const
{
    return QLatin1String("qsearchbar.h");
}
