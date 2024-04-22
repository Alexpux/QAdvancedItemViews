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
#ifndef QADVANCEDTABLEVIEWPLUGIN_H
#define QADVANCEDTABLEVIEWPLUGIN_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class QAdvancedTableViewPlugin : public QObject, public QDesignerCustomWidgetInterface {
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    explicit QAdvancedTableViewPlugin(QObject *parent = nullptr);

    bool isContainer() const override;
    bool isInitialized() const override;
    QIcon icon() const override;
    QString domXml() const override;
    QString group() const override;
    QString includeFile() const override;
    QString name() const override;
    QString toolTip() const override;
    QString whatsThis() const override;
    QWidget *createWidget(QWidget *parent) override;
    void initialize(QDesignerFormEditorInterface *core) override;

private:
    bool m_initialized { false };
};

#endif
