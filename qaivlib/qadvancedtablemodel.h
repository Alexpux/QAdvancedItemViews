/*
Copyright (c) 2019, Grgo Mariani
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Krasnoshchekov Petr ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Krasnoshchekov Petr BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef QADVANCEDTABLEMODEL_H
#define QADVANCEDTABLEMODEL_H

#include "qadvancedconstants.h"
#include "qadvancedheaderitem.h"

#include <QAbstractTableModel>
#include <QVariant>

class QAdvancedTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit QAdvancedTableModel(QObject *parent = nullptr);
    ~QAdvancedTableModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QAdvancedTableModel &setRow(QAdvancedHeaderItem *rowItem);
    QAdvancedTableModel &setColumn(QAdvancedHeaderItem *columnItem);

    QAdvancedHeaderItem *GetRootItem(Qt::Orientation orientation) const;
    QAdvancedHeaderItem *GetItemAtSection(int section, Qt::Orientation orientation) const;

    // data and headerData control what is shown
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

protected:
signals:

public slots:
    void emit_layoutChanged();

private:
    QStandardItemModel *_rows_model { nullptr };
    QStandardItemModel *_columns_model { nullptr };
};

#endif // QADVANCEDTABLEMODEL_H
