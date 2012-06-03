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
#ifndef QGROUPINGPROXYMODEL_H
#define QGROUPINGPROXYMODEL_H

#include <QAbstractProxyModel>
#include <qaivlib_global.h>

class QGroupingProxyModelGroup;
class QGroupingProxyModelPrivate;

//! The QGroupingProxyModel class implements a grouping proxy model.
/**
  * The QGroupingProxyModel can be used for grouping items. A tree structure is created to arrange the source items as defined by the group definitions.
  *
  * Let's assume that we want to group the items provided by a custom model. The code to set up the model and the view, without grouping, would look like this:
  * @code
  * QTreeView* mGroupView = new QTreeView;
  * QStandardItemModel* mItemModel = new QStandardItemModel(this);
  * mGroupView->setModel(mItemModel);
  * @endcode
  * To add grouping support to mDataModel, we need to create a QGroupingProxyModel, call setSourceModel() with the mDataModel as argument, and install the QGroupingProxyModel on the view:
  * @code
  * QTreeView* mGroupView = new QTreeView;
  * QStandardItemModel* mDataModel = new QStandardItemModel(this);
  * QGroupingProxyModel* mGroupingProxy = new QGroupingProxyModel(this);
  * mGroupingProxy->setSourceModel(mItemModel);
  * mGroupView->setModel(mGroupingProxy);
  * @endcode
  *
  * At this point no grouping is applied and the source model items are displayed in the tree view as children of the group "Ungrouped".
  * @image html qgroupingmodel01.png
  * <h2>Grouping</h2>
  * To enable grouping it is required to call setModelColumn() to determine the source model colum the proxy shall operate on.
  * @code
  * mGroupingProxy->setModelColumn(2);
  * @endcode
  * Once the model column is set, groups can be added by calling addGroup().
  *
  * @code
  * QTreeView* mGroupView = new QTreeView;
  * QStandardItemModel* mDataModel = new QStandardItemModel(this);
  * QGroupingProxyModel* mGroupingProxy = new QGroupingProxyModel(this);
  *
  * mGroupingProxy->setModelColumn(2);
  * mGroupingProxy->addGroup(QIcon(":/icons/folder"), "Qt's Tools", "Qt's tools");
  *
  * mGroupingProxy->setSourceModel(mItemModel);
  * mGroupView->setModel(mGroupingProxy);
  * @endcode
  *
  * A second group 'Qt's Tools' is added to the tree listing all source items matching the value 'Qt's tools' in the selected model column.
  * @note The addGroup() call use different values for displaying the group title and matching.
  *
  * @image html qgroupingmodel02.png
  *
  * <h2>Sorting</h2>
  * Sorting can be achieved by cascading proxy models. The first proxy will group the model items and the second sorts them.
  * @code
  * QTreeView* mGroupView = new QTreeView;
  * QStandardItemModel* mDataModel = new QStandardItemModel(this);
  * QGroupingProxyModel* mGroupingProxy = new QGroupingProxyModel(this);
  *
  * mGroupingProxy->setModelColumn(2);
  * mGroupingProxy->addGroup(QIcon(":/icons/folder"), "Qt's Tools", "Qt's tools");
  *
  * mGroupingProxy->setSourceModel(mItemModel);
  *
  * QSortFilterProxyModel* mSortFilterProxy = new QSortFilterProxyModel(this);
  * mSortFilterProxy->setSourceModel(mGroupingProxy);
  *
  * mGroupView->setModel(mSortFilterProxy);
  * @endcode
  * <h2>Save and Restore Groups</h2>
  * Group definitions can be saved and restored calling saveGroups() or restoreGroups().
  */
class QAIVLIBSHARED_EXPORT QGroupingProxyModel : public QAbstractItemModel
{
    Q_OBJECT
    //! @property(modelColumn)
    /**
      * This property holds the column in the source model that is used for grouping items.
      *
      * By default, this property contains 0, indicating that the first column in the source model will be used.
      * @see modelColumn() setModelColumn()
      */
    Q_PROPERTY(int modelColumn READ modelColumn WRITE setModelColumn)
public:
    /**
      * Constructs a QGroupingProxyModel with the given @p parent.
      */
    explicit QGroupingProxyModel(QObject *parent = 0);
    /**
      * Destroys the QGroupingProxyModel object.
      */
    ~QGroupingProxyModel();
    /**
      * Creates a new group named @p text matching the specified @p value.
      * @overload
      */
    void addGroup(const QString & text, const QVariant & value = QVariant());
    /**
      * Creates a new group named @p text, with an @p icon and matching the specified @p value.
      * @overload
      */
    void addGroup(const QIcon & icon, const QString & text, const QVariant & value = QVariant());
    /**
      * @reimp
      */
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    /**
      * @reimp
      */
    QVariant data(const QModelIndex & proxyIndex, int role) const;

    Qt::ItemFlags flags(const QModelIndex & index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;

    int findText(const QString & text) const;

    QModelIndex mapFromSource(const QModelIndex & sourceIndex) const;

    QModelIndex mapToSource(const QModelIndex & proxyIndex) const;
    /**
      * Returns the source model column used for grouping items.
      * @see setModelColumn()
      */
    int modelColumn() const;
    /**
      * dsghds
      * @reimp
      */
    QModelIndex parent(const QModelIndex & child) const;
    /**
      * Removes the group specified by the given @p index.
      */
    bool removeGroup(int index);
    /**
      * Restores the group definitions. Returns true if the groups are restored; otherwise returns false.
      * Typically this is used in conjunction with QSettings to restore the group definitions from a past session. Here is an example:
      * @code
      * QSettings settings;
      * groupingProxyModel->restoreGroups(settings.value("groups").toByteArray());
      * @endcode
      *
      * A failure to restore the group definitions may result from either invalid or out-of-date data in the supplied byte array.
      */
    bool restoreGroups(const QByteArray & data);
    /**
      * @reimp
      */
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    /**
      * Saves the proxy's group definitions.
      * Typically this is used in conjunction with QSettings to remember the groups for a future session. A version number is stored as part of the data. Here is an example:
      * @code
      * QSettings settings;
      * settings.setValue("groups", groupingProxyModel->saveGroups());
      * @endcode
      * @see restoreGroups()
      */
    QByteArray saveGroups() const;
    /**
      * Sets the @p role data for the item at @p index to @p value.
      */
    bool setData(const QModelIndex & index, const QVariant &value, int role);
    /**
      * Sets the source model @p column that is used for grouping items.
      * @see modelColumn()
      */
    void setModelColumn(int column);
    /**
      * @reimp
      */
    virtual void setSourceModel( QAbstractItemModel* sourceModel );
signals:
    
public slots:
private slots:
    void dataChangedHandler(const QModelIndex & topLeft, const QModelIndex & bottomRight);
    void rowsAboutToBeInsertedHandler( const QModelIndex & parent, int start, int end );
    void rowsInsertedHandler(const QModelIndex & parent, int first, int last);
private:
    void buildGroups();

    int groupAt(int sourceModelRow);
    void moveRows(int row, int count);
    void removeSourceModelRow(int sourceModelRow);

    QGroupingProxyModelPrivate* d;
};

#endif // QGROUPINGPROXYMODEL_H
