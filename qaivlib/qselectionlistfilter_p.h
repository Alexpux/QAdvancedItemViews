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

#include <QWidget>
#include <QLineEdit>
#include <QListView>
#include <QStandardItemModel>

#include <qcheckstateproxymodel.h>

class QSelectionListFilterEditor : public QWidget
{
    Q_OBJECT
public:
    QSelectionListFilterEditor(QWidget* parent = 0);

    QVariantList values(int role = Qt::DisplayRole) const;

    QVariantList selectedValues() const;

    void setSourceModel(QAbstractItemModel* model, int column);

    void setSelectedValues(const QVariantList & selectedValues);

    void setValues(const QVariantList & values);
private slots:
    void searchForTextEdited(const QString & text);
private:
    QCheckStateProxyModel* cCheckStateProxy;
    QLineEdit* cLineEdit;
    QListView* cListView;
    QStandardItemModel* cModel;
};

#endif // QSELECTIONLISTFILTER_P_H
