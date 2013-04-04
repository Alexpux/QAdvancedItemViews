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
#ifndef QUNIQUEVALUESPROXYMODEL_H
#define QUNIQUEVALUESPROXYMODEL_H

#include <QSortFilterProxyModel>

#include <qaivlib_global.h>

class QUniqueValuesProxyModelPrivate;

//! The QUniqueValuesProxyModel provides a filter model remove duplicate values from a source model.
/**
  * @ingroup proxy
  */
class QAIVLIBSHARED_EXPORT QUniqueValuesProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
	Q_PROPERTY(bool emptyItemsAllowed READ emptyItemsAllowed WRITE setEmptyItemsAllowed)
	//! @property(modelColumn)
	/**
	 * This property holds the column in the model that is use for filtering.
	 * @sa int modelColumn() const
	 * @sa void setModelColumn(int)
	 */
	Q_PROPERTY(int modelColumn READ modelColumn WRITE setModelColumn)
public:
	/**
	 * Contructs a unqiue values model with the given @p parent.
	 */
    QUniqueValuesProxyModel(QObject *parent = 0);
    /**
	 * Destroys the unique values model.
	 */
    ~QUniqueValuesProxyModel();
	/**
	 * @reimp QSortFilterProxyModel::data()
	 */
    QVariant data(const QModelIndex &index, int role) const;

	bool emptyItemsAllowed() const;
	/**
	 * Returns true if the item in the row indicated by the given source_row and source_parent contains a unique value in the model; otherwise returns false.
	 * @note By default, the Qt::DisplayRole is used to determine if the row should be accepted or not.
	 */
    bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const;

    virtual bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
	/**
	 * Returns model column used to determine unique value.
	 * @see setModelColumn()
	 */
    int modelColumn() const;
	/**
	 *
	 */
	void setEmptyItemsAllowed(bool on);
	/**
	 * Sets model column used to determine unique value to @p column.
	 * @see modelColumn()
	 */
    void setModelColumn(int colum);
	/**
	 * @reimp QSortFilterProxyModel::setSourceModel()
	 */
    void setSourceModel(QAbstractItemModel* sourceModel);
private slots:
    bool isDuplicate(int row) const;
    void buildMap();
    void sourceModelDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
private:
    QUniqueValuesProxyModelPrivate* d;
};

#endif // QUNQIUEVALUESPROXYMODEL_H
