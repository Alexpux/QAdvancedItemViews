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
#ifndef QAUTOFILTER_P_H
#define QAUTOFILTER_P_H

#include "qfiltereditorpopupwidget.h"
#include "qfiltereditorwidget.h"

#include <QStandardItemModel>

class QCheckBox;
class QLineEdit;
class QListView;
class QProgressDialog;
class QToolButton;

class QCheckStateProxyModel;
class QSingleColumnProxyModel;
class QUniqueValuesProxyModel;

class QAutoFilterEditorPopup : public QFilterEditorPopupWidget {
    Q_OBJECT
public:
    explicit QAutoFilterEditorPopup(QWidget *parent);
    ~QAutoFilterEditorPopup() = default;

    int mode() const;

    QVariantList selectedValues(int role = Qt::DisplayRole) const;

    void setSelectedValues(const QVariantList &values);

    void setSourceModel(QAbstractItemModel *model, int column);

signals:
    void accepted();
    void modeChanged();
    void rejected();

private slots:
    void uniqueValueModelProgressChanged(int progress);
    void checkStateProxyDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void emptyToolButtonClicked();
    void notEmptyToolButtonClicked();
    void searchForTextEdited(const QString &text);
    void selectCheckBoxStateChanged(int state);

private:
    int m_mode { 0 };
    QToolButton *m_emptyToolButton { nullptr };
    QLineEdit *m_lineEdit { nullptr };
    QListView *m_listView { nullptr };
    QToolButton *m_notEmptyToolButton { nullptr };
    QProgressDialog *m_progress { nullptr };
    QCheckBox *m_selectCheckBox { nullptr };

    QCheckStateProxyModel *m_checkStateProxy { nullptr };
    QSingleColumnProxyModel *m_singleColumnProxy { nullptr };
    QUniqueValuesProxyModel *m_singleValueProxy { nullptr };
};

class QAutoFilterEditor : public QFilterEditorWidget {
    Q_OBJECT
public:
    explicit QAutoFilterEditor(QWidget *parent = nullptr);

    QVariantList selectedValues(int role = Qt::DisplayRole) const;

    void setSourceModel(QAbstractItemModel *model, int column);

public slots:
    void modeSelected();
};

#endif // QAUTOFILTER_P_H
