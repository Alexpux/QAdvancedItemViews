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

//! The class QFixedRowsTableView implements a table view with fixed (pinned) rows.
/**
 * @ingroup views
 */
class QAIVLIBSHARED_EXPORT QFixedRowsTableView : public QTableView
{
    Q_OBJECT
	//! @property(fixedRowsMode)
	/**
	 * This property holds whether fixed rows mode is enabled
	 * @see bool fixedRowsMode() const
	 * @see void setFixedRowsMode(bool)
	 */
	Q_PROPERTY(bool fixedRowsMode READ fixedRowsMode WRITE setFixedRowsMode)
public:
	/**
	 * Constructs a fixed rows table view with the given @p parent.
	 */
    QFixedRowsTableView(QWidget* parent = 0);
	/**
	 * Destorys the fixed rows table view.
	 */
    ~QFixedRowsTableView();
	/**
	 * Returns the proxy model used to decorate the table view's vertical header.
	 */
    QFixedRowsDecorationProxyModel* decorationProxy() const;
	/**
	 * Returns true if the fixed rows mode is enabled. Otherwise false.
	 * @sa setFixedRowsMode(), QFixedRowsFilterProxyModel::setEnabled()
	 */
	bool fixedRowsMode() const;
	/**
	 * Returns the view filter proxy.
	 */
    QFixedRowsFilterProxyModel* filterProxy() const;
	/**
	 * @reimp QTableView::setModel()
	 */
    void setModel(QAbstractItemModel* model);
public slots:
	/**
	 * If @p on is true the fixed rows mode is enabled.
	 */
	void setFixedRowsMode(bool on);
signals:
	/**
	 * This signal is emitted when the table view has received the focus.
	 */
    void focusReceived();
protected:
	/**
	 * @reimp QAbstractItemView::closeEditor()
	 */
    void closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint);
	/**
	 * @reimp QAbstractItemView::focusInEvent()
	 */
    void focusInEvent(QFocusEvent* event);
private slots:
    void updateHeight();

    void verticalHeaderSectionClicked(int section);
private:
    QFixedRowsTableViewPrivate* d;
};

//! The QFixedRowsFilterProxyModel class implements a proxy model supporting fixed (pinned) rows.
/**
  * @ingroup proxy
  */
class QAIVLIBSHARED_EXPORT QFixedRowsFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
	/**
	 * Constructs a fixed rows filter proxy model with the given @p parent.
	 */
    QFixedRowsFilterProxyModel(QObject *parent = 0);
	/**
	 * Destroys the fixed rows proxy model.
	 */
    ~QFixedRowsFilterProxyModel();
	/**
	 * Clears the current selection of fixed rows.
	 */
    void clear();
	/**
	 * @reimp QSortFilterProxyModel::filterAcceptsRow()
	 */
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
	/**
	 * @reimp QSortFilterProxyModel::headerData()
	 */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	/**
	 * Returns true if the fixed rows mode is enabled. Otherwise false.
	 */
    bool isEnabled() const;
	/**
	 * Returns true if the specified (source) model @p row is pinned (fixed).
	 */
    bool isRowPinned(int row) const;
public slots:
	/**
	 * If @p on is true fixed rows mode is enabled.
	 */
    void setEnabled(bool on);
	/**
	 * @todo should not be public
	 */
    void sourceModelReset();
	/**
	 * Toggles the state of the row specified by the given @p index.
	 */
    void toggleRow(const QModelIndex & index);
	/**
	 * If @p fixed is true the row specified by the given @p index is added to the list of fixed rows.
	 */
    void setRowFixed(const QModelIndex & index, bool fixed);
private:
    QFixedRowsFilterProxyModelPrivate* d;
};

//! The QFixedRowsDecorationProxyModel class implements a decoration proxy model.
class QAIVLIBSHARED_EXPORT QFixedRowsDecorationProxyModel : public QIdentityProxyModel
{
    Q_OBJECT
public:
	/**
	 * Constructs a fixed rows decoration proxy model attached to the given @p filterProxy and with the given @p parent.
	 */
    QFixedRowsDecorationProxyModel(QFixedRowsFilterProxyModel* filterProxy, QObject* parent = 0);
	/**
	 * Destroys the fixed rows decoration proxy model.
	 */
    ~QFixedRowsDecorationProxyModel();
	/**
	 * @reimp QIdentityProxyModel::headerData()
	 */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	QSize iconSize() const;
	/**
	 * Returns true if the fixed rows mode is enabled. Otherwise false.
	 */
    bool isEnabled() const;
signals:
	/**
	 * This signal is emitted when fixed rows mode has toggled.
	 * If the mode is enabled, @p on is true.
	 */
    void modelToggled(bool on);
public slots:
	/**
	 * If @p on is true fixed rows mode is enabled.
	 */
    void setEnabled(bool on);
    void toggleRow(const QModelIndex & index);
private:
    QFixedRowsDecorationProxyModelPrivate* d;
};

#endif // QFIXEDROWSTABLEVIEW_H
