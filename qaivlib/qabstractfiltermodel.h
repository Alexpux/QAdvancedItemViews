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
#ifndef QABSTRACTFILTERMODEL_H
#define QABSTRACTFILTERMODEL_H

#include <QAbstractItemModel>
#include <QAction>
#include <QMenu>

#include <qaiv.h>
#include <qaivlib_global.h>

class QAbstractFilterProxyModel;
class QAbstractFilter;
class QFilterGroup;

class QAbstractFilterModelPrivate;

//! The QAbstractFilterModel class provides the abstract interface for filter model classes.
/**
  * The QAbstractFilterModel class defines the standard interface that filter models must use to be able to interoperate with an advanced table view.
  */
class QAIVLIBSHARED_EXPORT QAbstractFilterModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    struct FilterTypeEntry {
        int type;
        QIcon icon;
        QString text;
        QString tooltip;
    };
    /**
     * The QAbstractFilterModel defined additional item data roles for data needed by the view.
     */
    enum ItemDataRole {
        ValueFilterTypeRole = Qt::UserRole + 42        
        ,DefaultFilterTypeRole
        ,ColumnFilterTypesRole
    };
    /**
      * Constructs an abstract filter model with the given @p parent.
      */
    QAbstractFilterModel(QObject* parent = 0);
    /**
      * Destroys the abstract filter model.
      */
    ~QAbstractFilterModel();
    /**
      * @reimp
      * Returns the number of columns of the underlaying data (source) model.
      */
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    /**
      *
      */
    virtual QAbstractFilter* createFilter(const QModelIndex & index, const QVariantMap & properties) const = 0;
    /**
      * Returns the data stored under the given @p role for the filter referred to by the @p index.
      * @note
      * Returns an invalid QVariant if the given @p index is invalid or no folter exists for the the given @p index.
      * @see setData()
      */
    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    /**
      * Returns the filter group for the given @p index or 0 if no filter group exists for the specified @p index.
      */
    QFilterGroup* filterGroup(const QModelIndex & index) const;
    /**
      * Return the filter group at @p row or 0 if there is no filter group at the specified @p row.
      */
    QFilterGroup* groupAt(int row) const;
    /**
      * Returns the filter at the specified @p index.
      */
    QAbstractFilter* filter(const QModelIndex & index) const;

    QList<QAbstractFilter*> filtersAtRow(int row) const;
    /**
      * Returns the item flags for the given @p index.
      */
    Qt::ItemFlags flags(const QModelIndex & index) const;

    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;

    bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());

    QAdvancedItemViews::FilterMatchMode matchMode() const;

    QAdvancedItemViews::FilterProxyMode mode() const;
    /**
      * Returns an invalid index.
      */
    QModelIndex parent(const QModelIndex & index) const;
    /**
      * Returns a list of registered filter types.
      */
    QList<FilterTypeEntry> registeredFilterTypes() const;

    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

    int rowCount( const QModelIndex & parent = QModelIndex() ) const;

    bool setData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

    bool setHeaderData (int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole);

    void setMatchMode(QAdvancedItemViews::FilterMatchMode flag);

    void setMode(QAdvancedItemViews::FilterProxyMode mode);
    /**
      * Sets the given source @p model to be processed by the filter model.
      * @see sourceModel()
      */
    void setSourceModel(QAbstractItemModel* model);
    /**
      * Returns the model that contains the data this filter model is used for filtering.
      */
    QAbstractItemModel* sourceModel() const;
signals:
    void matchModeChanged(QAdvancedItemViews::FilterMatchMode mode);
    void modeChanged(QAdvancedItemViews::FilterProxyMode mode);
protected:
    /**
      * Registers the given filter @p type with its @p text and @p toolTip.
      */
    bool registerFilter(int type, const QString & text, const QString & toolTip = QString());
    /**
      * @overload registerFilter(int type, const QIcon & icon, const QString & text, const QString & toolTip = QString())
      */
    bool registerFilter(int type, const QIcon & icon, const QString & text, const QString & toolTip = QString());
private:
    QAbstractFilterModelPrivate* d;
};

#endif // QABSTRACTFILTERMODEL_H
