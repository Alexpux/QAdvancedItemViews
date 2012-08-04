#include "qaivdesignercollection.h"
#include "qadvancedtableviewplugin.h"

QAIVDesignerCollection::QAIVDesignerCollection(QObject *parent) :
    QObject(parent)
{
    m_widgets.append(new QAdvancedTableViewPlugin());
}

QList<QDesignerCustomWidgetInterface *> QAIVDesignerCollection::customWidgets() const
{
    return m_widgets;
}

Q_EXPORT_PLUGIN2(qaivdesignerplugin, QAIVDesignerCollection)

