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

#include <QSortFilterProxyModel>

#include <qaivlib_global.h>

class QCheckStateProxyModelPrivate;

//! The QCheckStateProxyModel class adds check boxes to a model.
class QAIVLIBSHARED_EXPORT QCheckStateProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit QCheckStateProxyModel(QObject *parent = 0);
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

    virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    /**
      * @reimp
      */
    virtual Qt::ItemFlags flags( const QModelIndex & index ) const;
    /**
      * Returns true if the column @p column is checkable. Otherwise false.
      * @see setColumnCheckable()
      */
    bool isColumnCheckable(int column) const;

    void setColumnCheckable(int column, bool checkable = true);

    void setCheckedIndexes(const QModelIndexList & indexes);

    void setCheckedValues(int column, const QVariantList & values);

    virtual bool setData( const QModelIndex & index, const QVariant & value, int role );
    /**
      * @reimp
      */
    virtual void setSourceModel(QAbstractItemModel *sourceModel);
public slots:
	void setAllChecked(bool checked = true);

	void setChecked(const QModelIndex & index, bool checked);
private slots:
    void sourceModelAboutToBeReset();
private:
    QCheckStateProxyModelPrivate* d;
};

#endif // QCHECKSTATEPROXYMODEL_H
