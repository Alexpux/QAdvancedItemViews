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
#ifndef QFIXEDROWSTABLEVIEW_H
#define QFIXEDROWSTABLEVIEW_H

#include <QHeaderView>
#include <QIdentityProxyModel>
#include <QPointer>
#include <QSortFilterProxyModel>
#include <QTableView>

#include <qaivlib_global.h>

class QFixedRowsFilterProxyModel;
class QFixedRowsDecorationProxyModel;
class QFixedRowsDecorationProxyModelPrivate;
class QFixedRowsFilterProxyModelPrivate;
class QFixedRowsTableViewPrivate;

class QAIVLIBSHARED_EXPORT QFixedRowsTableView : public QTableView
{
    Q_OBJECT
public:
    QFixedRowsTableView(QWidget* parent = 0);
    ~QFixedRowsTableView();

    QFixedRowsDecorationProxyModel* decorationProxy() const;

    QFixedRowsFilterProxyModel* filterProxy() const;

    void setModel(QAbstractItemModel* model);
signals:
    void focusReceived();
protected:
    void closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint);
    void focusInEvent(QFocusEvent* event);
private slots:
    void updateHeight();

    void verticalHeaderSectionClicked(int section);
private:
    QFixedRowsTableViewPrivate* d;
};

class QAIVLIBSHARED_EXPORT QFixedRowsFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    QFixedRowsFilterProxyModel(QObject *parent = 0);
    ~QFixedRowsFilterProxyModel();

    void clear();

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool isEnabled() const;

    bool isRowPinned(int row) const;
public slots:
    void setEnabled(bool on);

    void sourceModelReset();

    void toggleRow(const QModelIndex & index);

    void setRowFixed(const QModelIndex & index, bool fixed);
private:
    QFixedRowsFilterProxyModelPrivate* d;
};

class QAIVLIBSHARED_EXPORT QFixedRowsDecorationProxyModel : public QIdentityProxyModel
{
    Q_OBJECT
public:
    QFixedRowsDecorationProxyModel(QFixedRowsFilterProxyModel* filterProxy, QObject* parent = 0);
    ~QFixedRowsDecorationProxyModel();

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	QSize iconSize() const;

    bool isEnabled() const;

    void setEnabled(bool on);
signals:
    void pinRowsEnabled(bool on);
public slots:
    void toggleRow(const QModelIndex & index);
private:
    QFixedRowsDecorationProxyModelPrivate* d;
};

#endif // QFIXEDROWSTABLEVIEW_H
