#ifndef QTABLEMODELEXPORTPUSHBUTTON_H
#define QTABLEMODELEXPORTPUSHBUTTON_H

#include <QPushButton>

#include <qaivlib_global.h>

//! The QTableModelExportPushButton widget provides a command button for exporting table model data.
class QAIVLIBSHARED_EXPORT QTableModelExportPushButton : public QPushButton
{
	Q_OBJECT
public:
	/**
	 * Constructs a push button with the given @p parent.
	 */
	QTableModelExportPushButton(QWidget *parent);
	/**
	 * Destroys the push button.
	 */
	~QTableModelExportPushButton();
signals:
	void exportAllTriggered(const QByteArray & format);
	void exportSelectionTriggered(const QByteArray & format);
private slots:
	void exportAllTriggered();		
	void exportSelectionTriggered();		
};

#endif // QTABLEMODELEXPORTPUSHBUTTON_H
