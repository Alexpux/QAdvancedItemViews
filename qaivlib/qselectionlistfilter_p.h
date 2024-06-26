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
#ifndef QSELECTIONLISTFILTER_P_H
#define QSELECTIONLISTFILTER_P_H

#include "qcheckstateproxymodel.h"
#include "qfiltereditorpopupwidget.h"
#include "qfiltereditorwidget.h"

#include <QStandardItemModel>

class QCheckBox;
class QLineEdit;
class QListView;
class QToolButton;

class QSelectionListFilterEditorPopup : public QFilterEditorPopupWidget {
    Q_OBJECT
public:
    explicit QSelectionListFilterEditorPopup(QWidget *parent);
    ~QSelectionListFilterEditorPopup() = default;

    bool cancelAndClose(QObject *obj, int key) const override;

    bool commitAndClose(QObject *obj, int key) const override;

    int mode() const;

    QVariantList selectedValues() const;

    void setSourceModel(QAbstractItemModel *model, int column);

    void setSelectedValues(const QVariantList &selectedValues);

    void setValues(const QVariantList &values);

    QVariantList values(int role = Qt::DisplayRole) const;

signals:
    void accepted();
    void modeChanged();
    void rejected();

private slots:
    void checkStateProxyDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void emptyToolButtonClicked();
    void notEmptyToolButtonClicked();
    void searchForTextEdited(const QString &text);
    void selectCheckBoxStateChanged(int state);

private:
    // 0 = Selected values
    // 1 = empty
    // 2 = not empty
    int m_mode { 0 };

    QCheckStateProxyModel *m_checkStateProxy { nullptr };
    QCheckBox *m_selectCheckBox { nullptr };
    QLineEdit *m_lineEdit { nullptr };
    QListView *m_listView { nullptr };
    QToolButton *m_emptyToolButton { nullptr };
    QToolButton *m_notEmptyToolButton { nullptr };
    QStandardItemModel *m_model { nullptr };
};

class QSelectionListFilterEditor : public QFilterEditorWidget {
    Q_OBJECT
public:
    explicit QSelectionListFilterEditor(QWidget *parent = nullptr);
    ~QSelectionListFilterEditor() = default;

public slots:
    void modeSelected();
};

#endif // QSELECTIONLISTFILTER_P_H
