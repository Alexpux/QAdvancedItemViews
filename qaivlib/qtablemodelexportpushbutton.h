#ifndef QTABLEMODELEXPORTPUSHBUTTON_H
#define QTABLEMODELEXPORTPUSHBUTTON_H

#include "qaivlib_global.h"

#include <QPushButton>

//! The QTableModelExportPushButton widget provides a command button for exporting table model data.
class QAIVLIBSHARED_EXPORT QTableModelExportPushButton : public QPushButton {
    Q_OBJECT
public:
    /**
     * Constructs a push button with the given @p parent.
     */
    explicit QTableModelExportPushButton(QWidget *parent);
    /**
     * Destroys the push button.
     */
    ~QTableModelExportPushButton() = default;

signals:
    void exportAllTriggered(const QByteArray &format);
    void exportSelectionTriggered(const QByteArray &format);

private slots:
    void exportAllTriggeredImpl();
    void exportSelectionTriggeredImpl();
};

#endif // QTABLEMODELEXPORTPUSHBUTTON_H
