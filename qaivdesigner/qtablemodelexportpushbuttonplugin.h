#ifndef QTABLEMODELEXPORTPUSHBUTTONPLUGIN_H
#define QTABLEMODELEXPORTPUSHBUTTONPLUGIN_H

#include <QDesignerCustomWidgetInterface>

class QTableModelExportPushButtonPlugin : public QObject, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	QTableModelExportPushButtonPlugin(QObject* parent);
    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);
    
private:
    bool m_initialized;
};

#endif // QTABLEMODELEXPORTPUSHBUTTONPLUGIN_H
