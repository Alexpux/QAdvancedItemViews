/******************************************************************************
** This file is part of qadvanceditemviews.
**
** Copyright (c) 2011-2012 Martin Hoppe martin@2x2hoppe.de
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
#ifndef QCOLORFILTER_P_H
#define QCOLORFILTER_P_H

#include "qfiltereditorpopupwidget.h"
#include "qfiltereditorwidget.h"

#include <QCheckBox>
#include <QListView>
#include <QStandardItemModel>

class QCheckStateProxyModel;
class QToolButton;

/**
 * @internal
 * Popup widget for color filter editor.
 * Displays list of available colors from the model with checkboxes.
 */
class QColorFilterEditorPopup : public QFilterEditorPopupWidget {
    Q_OBJECT

public:
    explicit QColorFilterEditorPopup(QWidget *parent = nullptr);
    ~QColorFilterEditorPopup() override = default;

    QColorFilterEditorPopup(const QColorFilterEditorPopup &) = delete;
    QColorFilterEditorPopup &operator=(const QColorFilterEditorPopup &) = delete;

    /**
     * Gets the current mode (0=selected colors, 1=empty, 2=not empty).
     */
    [[nodiscard]] int mode() const { return m_mode; }

    /**
     * Gets the list of selected color values.
     */
    [[nodiscard]] QVariantList selectedColors() const;

    /**
     * Sets the list of selected colors.
     */
    void setSelectedColors(const QVariantList &colors);

    /**
     * Sets the list of available colors to choose from.
     */
    void setAvailableColors(const QVariantList &colors);

signals:
    void modeChanged();
    void accepted();
    void rejected();

private slots:
    void emptyToolButtonClicked();
    void notEmptyToolButtonClicked();
    void checkStateProxyDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void selectCheckBoxStateChanged(Qt::CheckState state);

private:
    bool cancelAndClose(QObject *obj, int key) const override;
    bool commitAndClose(QObject *obj, int key) const override;

    QListView *m_listView { nullptr };
    QStandardItemModel *m_model { nullptr };
    QCheckStateProxyModel *m_checkStateProxy { nullptr };
    QToolButton *m_emptyToolButton { nullptr };
    QToolButton *m_notEmptyToolButton { nullptr };
    QCheckBox *m_selectCheckBox { nullptr };
    int m_mode { 0 }; // 0=selected, 1=empty, 2=not empty
};

/**
 * @internal
 * Editor widget for QColorFilter.
 */
class QColorFilterEditor : public QFilterEditorWidget {
    Q_OBJECT

public:
    explicit QColorFilterEditor(QWidget *parent = nullptr);
    ~QColorFilterEditor() override = default;

    QColorFilterEditor(const QColorFilterEditor &) = delete;
    QColorFilterEditor &operator=(const QColorFilterEditor &) = delete;

private slots:
    void modeSelected();
};

#endif // QCOLORFILTER_P_H
