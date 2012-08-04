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
