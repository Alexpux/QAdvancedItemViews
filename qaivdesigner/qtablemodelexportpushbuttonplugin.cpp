#include "qtablemodelexportpushbuttonplugin.h"

#include "qtablemodelexportpushbutton.h"

QTableModelExportPushButtonPlugin::QTableModelExportPushButtonPlugin(QObject *parent)
	: QObject(parent)
{
    m_initialized = false;
}

void QTableModelExportPushButtonPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool QTableModelExportPushButtonPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QTableModelExportPushButtonPlugin::createWidget(QWidget *parent)
{
    return new QTableModelExportPushButton(parent);
}

QString QTableModelExportPushButtonPlugin::name() const
{
    return QLatin1String("QTableModelExportPushButton");
}

QString QTableModelExportPushButtonPlugin::group() const
{
    return QLatin1String("QAdvancedItemViews");
}

QIcon QTableModelExportPushButtonPlugin::icon() const
{
    return QIcon(":/qaiv/table.export");
}

QString QTableModelExportPushButtonPlugin::toolTip() const
{
    return QLatin1String("");
}

QString QTableModelExportPushButtonPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QTableModelExportPushButtonPlugin::isContainer() const
{
    return false;
}

QString QTableModelExportPushButtonPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QTableModelExportPushButton\" name=\"pushButton\">\n</widget>\n");
}

QString QTableModelExportPushButtonPlugin::includeFile() const
{
    return QLatin1String("qtablemodelexportpushbutton.h");
}
