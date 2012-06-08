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
#ifndef QADVANCEDTABLEVIEW_H
#define QADVANCEDTABLEVIEW_H

#include <QAbstractItemView>
#include <QHeaderView>

#include <qaivlib_global.h>
#include <qabstractfilter.h>

class QFilterView;
class QAdvancedHeaderView;
class QFilterModelProxy;
class QAbstractFilterModel;
class QFilterTableViewSettingsDialog;

class QAdvancedTableViewPrivate;

namespace Ui {
    class QAdvancedTableView;
}

//! The QAdvancedTableView class provides an advanced model/view implementation of a table view.
class QAIVLIBSHARED_EXPORT QAdvancedTableView : public QWidget
{
    Q_OBJECT
	//! @property(alternatingRowColors)
	/**
	 * This property holds whether to draw the background using alternating colors.
     *
     * By default this property is false.
     *
     * @see alternatingRowColors()
     * @see setAlternatingRowColors()
	 */
    Q_PROPERTY(bool alternatingRowColors READ alternatingRowColors WRITE setAlternatingRowColors)
	//! @property(autoScroll)
    Q_PROPERTY(bool autoScroll READ hasAutoScroll WRITE setAutoScroll)
	//! @property(dragEnabled)
	Q_PROPERTY(bool dragEnabled READ dragEnabled WRITE setDragEnabled)
	//! @property(editTriggers)
	Q_PROPERTY(QAbstractItemView::EditTriggers editTriggers READ editTriggers WRITE setEditTriggers)
    //! @property(bool filterVisible READ filterVisible WRITE setFilterVisible)
    Q_PROPERTY(QAbstractItemView::EditTriggers editTriggers READ editTriggers WRITE setEditTriggers)
    //! @property(gridStyle)
	/**
	 * This property holds the pen style used to draw the grid.
	 */
	Q_PROPERTY(Qt::PenStyle gridStyle READ gridStyle WRITE setGridStyle)
	//! @property(selectionMode)
	/**
	 * This property holds which selection mode the view operates in.
	 */
	Q_PROPERTY(QAbstractItemView::SelectionMode selectionMode READ selectionMode WRITE setSelectionMode)
    //! @property(showGrid)
	/**
	 * This property holds whether the grid is shown.
	 * If this property is true a grid is drawn for the table; if the property is false, no grid is drawn. The default value is true.
	 */
	Q_PROPERTY(bool showGrid READ showGrid WRITE setShowGrid)
	//! @property(editTriggers)
    Q_PROPERTY(bool showSortIndicator READ isSortIndicatorShown WRITE setSortIndicatorShown)
	//! @property(sortingEnabled)
    Q_PROPERTY(bool sortingEnabled READ isSortingEnabled WRITE setSortingEnabled)

	//! @property(textElideMode)
	Q_PROPERTY(Qt::TextElideMode textElideMode READ textElideMode WRITE setTextElideMode)
	//! @property(
	/**
	 * This property holds the item text word-wrapping policy.
	 */
	 Q_PROPERTY(bool wordWrap READ wordWrap WRITE setWordWrap)
public:
	/**
     * The QAdvancedTableView defined additional item data roles for data needed by the view.
	 */
	enum ItemDataRole {
		AutoFilterRole = Qt::UserRole + 42,
		SelectionFilterRole,
		ValueRole
	};
	/**
	 * Constructs a filter table view with a @p parent to represent the data.
	 */
    explicit QAdvancedTableView(QWidget *parent = 0);
	/**
	 * Destroys the filter table view.
	 */
    ~QAdvancedTableView();
	/**
	 * Returns true if alternating row colors are displayed. Otherwise false.
	 * @see setAlternatingRowColors()
	 */
    bool alternatingRowColors() const;
    /**
      * Returns a list filter types allowed for the specfied @p column.
      * The list is empty if the specified @p column does not exists or all types are allowed.
      * @see setColumnFilterTypes()
      */
    QVariantList columnsFilterTypes(int column) const;
	/**
	 *
	 */
	bool hasAutoScroll() const;
	/**
	 * Returns the model index of the current item.
	 * @see setCurrentIndex()
	 */
	QModelIndex currentIndex() const;
    /**
      * Returns the default filter type set for the given @p column.
      * Returns -1 if no default filter type is set or the given @p column does not exists.
      * @see setDefaultFilterType()
      */
    int defaultFilterType(int column) const;
	/**
	 *
	 */
	bool dragEnabled() const;

	QAbstractItemView::EditTriggers editTriggers() const;
    /**
      *
      */
    QAbstractFilter* filterAt(int row, int col) const;
    /**
      *
      */
    QAbstractFilterModel* filterModel() const;
    /**
      * Returns true if the filters are show in the filter view. Otherwise false.
      * @see setFilterVisible()
      */
    bool filterVisible() const;
	/**
	 * Returns the table view's horizontal header.
	 */
	QHeaderView* horizontalHeader() const;
	/**
	 *
	 */
    bool horizontalStretchLastSection() const;
	/**
	 * Returns the style used when drawing the grid.
	 * @see setGridStyle()
	 */
	Qt::PenStyle gridStyle() const;
	/**
	 * Returns the model index of the item at the viewport coordinates @p point.
	 */
	virtual QModelIndex indexAt(const QPoint & point) const;
	/**
	 * Returns true if the given @p column is hidden; otherwise returns false.
	 * @see isRowHidden()
	 */
	bool isColumnHidden(int column) const;
	/**
	 * Returns true if the given @p row is hidden; otherwise returns false.
	 * @see isColumnHidden()
	 */
	bool isRowHidden(int row) const;
	/**
	 * Returns true if the sort indicator is shown. Otherwise false.
	 */
    bool isSortIndicatorShown() const;
	/**
	 * Returns true if sorting is enabled. Otherwise false.
	 * @see setSortingEnabled()
	 */
    bool isSortingEnabled() const;
    /**
      * Returns the item delegate used by this view and model. This is either one set with setItemDelegate(), or the default one.
      * @see setItemDelegate()
      */
    QAbstractItemDelegate* itemDelegate() const;
	/**
	 *
	 */
	QSize minimumSizeHint() const;
    /**
      * Returns the model used by this view.
      */
    QAbstractItemModel* model() const;

    void removeFilter( int column );
    /**
      * Restores the filter definition of this table view. This function returns true if the filter definition was restored; otherwise returns false.
      * @see saveState()
      */
    bool restoreFilter(const QByteArray & data);
    /**
      * Returns the model index of the model's root item. The root item is the parent item to the view's toplevel items. The root can be invalid.
      */
    QModelIndex rootIndex() const;
    /**
      * Saves the current filter definition of this table view.
      * @see restoreFilter()
      */
    QByteArray saveFilter() const;
	/**
	 * Returns the current selection mode
	 * @see setSelectionMode()
	 */
	QAbstractItemView::SelectionMode selectionMode() const;
    /**
     * Returns the current selection model.
     */
    QItemSelectionModel* selectionModel() const;
    /**
	 * If @p enabled is true, the item background will be drawn using QPalette::Base and QPalette::AlternateBase; otherwise the background will be drawn using the QPalette::Base color.
	 * @see alternatingRowColors()
	 */
    void setAlternatingRowColors(bool enable);
	/**
	 *
	 */
	void setAutoScroll(bool enable);
    /**
      *
      */
	void setContextMenuPolicy(Qt::ContextMenuPolicy policy);
    /**
      * Sets the default filter @p type for the specified @p column.
      */
    void setDefaultFilterType(int column, int type);
	/**
	 *
	 */
	void setDragEnabled(bool enable);
	/**
	 * Sets the current item to be the item at @p index.
	 */
	void setEditTriggers(QAbstractItemView::EditTriggers triggers);
    /**
      * Sets the list of available filter @p types for the given @p column.
      * @see columnsFilterTypes()
      */
    void setColumnFilterTypes(int column, const QVariantList & types);
	/**
	 * This property holds the pen @p style used to draw the grid.
	 * @see gridStyle()
	 */
	void setGridStyle(Qt::PenStyle style);
	/**
	 * Sets the item delegate for this view and its model to @p delegate. This is useful if you want complete control over the editing and display of items.
     * @see ItemDelegate()
	 */
	void setItemDelegate(QAbstractItemDelegate* delegate);
	/**
	 * Sets the @p model for the view to present.
	 */
    void setModel( QAbstractItemModel* model );

	void setSelectionBehavior(QAbstractItemView::SelectionBehavior behavior);
	/**
	 * Sets the selection @p mode.
	 */
	void setSelectionMode(QAbstractItemView::SelectionMode mode);
    /**
      * Sets the filter @p type for the specified @p column.
      * @see filterType()
      */
    bool setFilterType(int type, int column, int row = 0);
	/**
	 * If @p show is true a grid is drawn for the table. Otherwise no grid is drawn.
	 * @see showGrid()
	 */
	void setShowGrid( bool show );
	/**
	 * If @p show is true a sort indicator is shown.
	 */
    void setSortIndicatorShown(bool show);
	/**
	 * If @p enable is true sorting is enabled. Otherwise sorting is disabled.
	 * @see isSortingEnabled()
	 */
    void setSortingEnabled(bool enable);
	/**
	 * Returns the settings dialog for the view.
	 * This function creates and returns a settings dialog.
	 */
	QFilterTableViewSettingsDialog* settingsDialog();
	/**
	 *
	 */
	void setTextElideMode(Qt::TextElideMode mode);
	/**
	 * If @p wrap is true word wrapping is enabled.
	 */
	void setWordWrap(bool wrap);
	/**
	 * Returns true if the show is shwon. Otherwise false.
	 */
	bool showGrid() const;

	QSize sizeHint() const;
	/**
	 *
	 */
	Qt::TextElideMode textElideMode() const;
	/**
	 * Returns the table view's vertical header.
	 */
	QHeaderView* verticalHeader() const;
	/**
	 * Returns the viewport widget.
	 */
	QWidget* viewport() const;
	/**
	 *
	 */
	bool wordWrap() const;
signals:
	/**
	 * This signal is emitted when the item specified by index is activated by the user. How to activate items depends on the platform; e.g., by single- or double-clicking the item, or by pressing the Return or Enter key when the item is current.
	 * @see clicked() doubleClicked() entered() pressed()
	 */
	void activated( const QModelIndex & index );
	/**
	 * This signal is emitted when a mouse button is clicked. The item the mouse was clicked on is specified by index. The signal is only emitted when the index is valid.
	 * @see activated() doubleClicked() entered() pressed()
	 */
	void clicked( const QModelIndex & index );
    /**
      * This signal is emitted when a mouse button is double-clicked. The item the mouse was double-clicked on is specified by index. The signal is only emitted when the index is valid.
      */
	void doubleClicked( const QModelIndex & index );
    /**
      * This signal is emitted when the mouse cursor enters the item specified by index. Mouse tracking needs to be enabled for this feature to work.
      * @see clicked() doubleClicked()
      */
	void entered( const QModelIndex & index );
    /**
      * This signal is emitted when a mouse button is pressed. The item the mouse was pressed on is specified by @p index. The signal is only emitted when the @p index is valid.
      * Use the QApplication::mouseButtons() function to get the state of the mouse buttons.
      * @see activated() clicked() doubleClicked() entered()
      */
	void pressed( const QModelIndex & index );
    /**
      * This function is used to handle tool tips, and What's This? mode, if the given event is a QEvent::ToolTip,or a QEvent::WhatsThis. It passes all other events on to its base class viewportEvent() handler.
      */
	void viewportEntered();
public slots:
    /**
      * Adds the an empty filter group.
      */
    void addFilterGroup();
	/**
	 * Deselects all selected items. The current index will not be changed.
	 * @see setSelection()selectAll()
	 */
	void clearSelection();
    /**
      * Starts editing the item corresponding to the given qp index if it is editable.
      */
	void edit(const QModelIndex & index);
	/**
	 * Hide the given column.
	 * @see showColumn() hideRow()
	 */
	void hideColumn(int column);
	/**
     * Hides the filter view.
	 */
    void hideFilterView();
	/**
	 * Resizes the given @p column based on the size hints of the delegate used to render each item in the column.
	 * @remark Only visible columns will be resized. Reimplement sizeHintForColumn() to resize hidden columns as well.
	 */
	void resizeColumnToContents(int column);
	/**
	 * Resizes all rows based on the size hints of the delegate used to render each item in the rows.
	 */
	void resizeRowsToContents();
    /**
      * Reset the internal state of the view.
      */
	virtual void reset();
    /**
      * Scrolls the view to the bottom.
      * @see scrollToTop()
      */
	void scrollToBottom();
    /**
      * Scrolls the view to the top.
      * @see scrollToTop()
      */
	void scrollToTop();
    /**
      * Selects all items in the view. This function will use the selection behavior set on the view when selecting.
      */
	virtual void selectAll();
    /**
      * Sets the current item to be the item at @p index.
      */
	void setCurrentIndex(const QModelIndex & index);

    void setFilterEnabled(int row, int column, bool enable);
	/**
	 * If @p visible is true the filter is shown. Otherwise the filter is hidden.
	 * @see isFilterVisible()
	 */
	void setFilterVisible(bool visible);
    /**
      * Sets the model's root @p index.
      */
	void setRootIndex(const QModelIndex & index);
	/**
     * Updates the area occupied by the given @p index.
	 */
	void update(const QModelIndex & index);

private slots:
    void dataModelLayoutChanged();
	void dataViewCustomContextMenuRequested(const QPoint & pos);
    void dataViewHorizontalScrollBarSilderMoved( int value );
    void dataViewHorizontalScrollBarValueChanged( int value );
	void filterAdded(const QModelIndex & parent, int start, int end);
    void headerViewHorizontalScrollBarSilderMoved( int value );
    void headerViewHorizontalScrollBarValueChanged( int value );
    void headerViewSectionResized( int logicalIndex, int oldSize, int newSize );
    void horizontalHeaderViewSectionMoved( int logicalIndex, int oldVisualIndex, int newVisualIndex );
    void horizontalHeaderSortIndicatorChanged( int logicalIndex, Qt::SortOrder order );
    void modelReset();
	void showSettingsDialog();
    void updateHeaderViewHorizontalScrollBar( int min, int max );
    void updateHeaderViewGeometries();
    void updateHeaderViewVerticalScrollBar( int min, int max );
private:
    QAdvancedTableViewPrivate* d;

    Ui::QAdvancedTableView *ui;
};

#endif // QADVANCEDTABLEVIEW_H
