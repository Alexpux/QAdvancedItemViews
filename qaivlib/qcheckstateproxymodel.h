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
#ifndef QCHECKSTATEPROXYMODEL_H
#define QCHECKSTATEPROXYMODEL_H

#include <QIdentityProxyModel>

#include <qaivlib_global.h>

class QCheckStateProxyModelPrivate;

//! The QCheckStateProxyModel class adds check boxes to a model.
/**
 * @ingroup proxy
 */
class QAIVLIBSHARED_EXPORT QCheckStateProxyModel : public QIdentityProxyModel
{
    Q_OBJECT
public:
	/**
	 * Constructs a QCheckStateProxyModel with the given @p parent.
	 */
    explicit QCheckStateProxyModel(QObject* parent = 0);
    /**
      * Destroys the QCheckStateProxyModel object.
      */
    ~QCheckStateProxyModel();
	/**
	 * Returns the number of columns checkable.
	 */
	int checkableColumnsCount() const;
    /**
      * Returns a list with the checked indexes.
      */
    QModelIndexList checkedIndexes() const;
	/**
	 * @reimp
	 */
    virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    /**
      * @reimp
      */
    virtual Qt::ItemFlags flags( const QModelIndex & index ) const;
	/**
	 * Returns true if the given model item @p index is checked.
	 */
	bool isChecked(const QModelIndex & index) const;
    /**
      * Returns true if the column @p column is checkable. Otherwise false.
      * @see setColumnCheckable()
      */
    bool isColumnCheckable(int column) const;
	/**
	 * If @p checkable is true the items in the specified @p column are checkable.
	 */
    void setColumnCheckable(int column, bool checkable = true);
	/**
	 * Checks the item specified by the list of @p indexes.
	 */
    void setCheckedIndexes(const QModelIndexList & indexes);
	/**
	 * Checks all items in the specifed @p column matching the list of @p values.
	 */
    void setCheckedValues(int column, const QVariantList & values);
    /**
      * @reimp
      */
    virtual bool setData( const QModelIndex & index, const QVariant & value, int role );
    /**
      * @reimp
      */
    virtual void setSourceModel(QAbstractItemModel *sourceModel);
public slots:
	/**
	 * If @p checked is true, all checkable items are checked.
	 */
	void setAllChecked(bool checked = true);
	/**
	 * If @p checked is true, the item specified by the given @p index is checked.
	 */
	void setChecked(const QModelIndex & index, bool checked);
private slots:
	void sourceModelRowsAboutToBeRemoved(const QModelIndex & parent, int start, int end);
    void sourceModelAboutToBeReset();
private:
    QCheckStateProxyModelPrivate* d;
};

#endif // QCHECKSTATEPROXYMODEL_H
