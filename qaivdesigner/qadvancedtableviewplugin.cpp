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
