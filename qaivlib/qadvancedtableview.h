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
class QAbstractFilterProxyModel;

class QAdvancedTableViewPrivate;

namespace Ui {
    class QAdvancedTableView;
}

//! The QAdvancedTableView class provides an advanced model/view implementation of a table view.
/** @ingroup views
 * QAdvancedTableView is not deribed from QAbstractItemView, but the current implementation provides maximum compatibility with QAbstractItemView.
 * <h2>Features</h2>
 * QAdvancedTableView provides the following features.
 * <ul><li>Filter</li>
 * <li>Fixed Rows</li>
 * <li>Splitted View</li></ul>
 * <h2>Visual Appearance</h2>
 * <h3>QTable View Compatibility</h3>
 * With all features disabled, the QAbvancedTableView provides with same look and feel as Qt's QTableView class.
 * @image html qadvancedtableview01.png "Compatibility Mode" width=5cm
 * <h3>Filter</h3>
 * The visibility of the filter is controlled with setShowFilter().
 * @image html qadvancedtableview02.png "Filter View Visible"
 * <h3>Fixed Rows</h3>
 * @image html qadvancedtableview03.png Fixed Rows Enabled
 * A list
 * <h3>Splitted View</h3>
 * @image html qadvancedtableview04.png Splitted View Enabled
 * <h2>Filter</h2>
 *
 */
class QAIVLIBSHARED_EXPORT QAdvancedTableView : public QWidget
{
    Q_OBJECT
	//! @property(alternatingRowColors)
	/**
	 * This property holds whether to draw the background using alternating colors.
     *
     * By default this property is false.
     *
     * @see alternatingRowColors(), setAlternatingRowColors()
	 */
    Q_PROPERTY(bool alternatingRowColors READ alternatingRowColors WRITE setAlternatingRowColors)
	//! @property(autoScroll)
    /**
      * This property holds whether autoscrolling in drag move events is enabled.
      *
      * If this property is set to true (the default), the QAbstractItemView automatically scrolls the contents of the view if the user drags within 16 pixels of the viewport edge. If the current item changes, then the view will scroll automatically to ensure that the current item is fully visible.
      *
      * This property only works if the viewport accepts drops. Autoscroll is switched off by setting this property to false.
      * @see bool hasAutoScroll() const
      * @see void setAutoScroll(bool enable)
      */
    Q_PROPERTY(bool autoScroll READ hasAutoScroll WRITE setAutoScroll)
	//! @property(dragEnabled)
    /**
      * This property holds whether the view supports dragging of its own items.
      */
	Q_PROPERTY(bool dragEnabled READ dragEnabled WRITE setDragEnabled)
	//! @property(editTriggers)
    /**
      * This property holds which actions will initiate item editing.
      *
      * This property is a selection of flags defined by EditTrigger, combined using the OR operator. The view will only initiate the editing of an item if the action performed is set in this property.
      */
    Q_PROPERTY(QAbstractItemView::EditTriggers editTriggers READ editTriggers WRITE setEditTriggers)
	//! @property(filterDisabledIcon)
	/**
	 * This property holds the icon shown if a filter is disabled.
	 */
	Q_PROPERTY(QIcon filterDisabledIcon READ filterDisabledIcon WRITE setFilterDisabledIcon)
	//! @property(filterEnabledIcon)
	/**
	 * This property holds the icon shown if a filter is enabled.
	 */
	Q_PROPERTY(QIcon filterEnabledIcon READ filterEnabledIcon WRITE setFilterEnabledIcon)
    //! @property(gridStyle)
	/**
	 * This property holds the pen style used to draw the grid.
	 */
	Q_PROPERTY(Qt::PenStyle gridStyle READ gridStyle WRITE setGridStyle)
    //! @property(iconSize)
    /**
      * This property holds the size of items' icons.
      *
      * Setting this property when the view is visible will cause the items to be laid out again.
      */
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
	//! @property(selectionBehavior)
	/**
	 * This property holds which selection behavior the view uses.
	 * This property holds whether selections are done in terms of single items, rows or columns.
	 * @see QTableView::selectionBehavior()
	 */
	Q_PROPERTY(QAbstractItemView::SelectionBehavior selectionBehavior READ selectionBehavior WRITE setSelectionBehavior)
	//! @property(selectionMode)
	/**
	 * This property holds which selection mode the view operates in.
	 */
	Q_PROPERTY(QAbstractItemView::SelectionMode selectionMode READ selectionMode WRITE setSelectionMode)
    //! @property(showFilter)
    /**
     * This property holds whether the filter are shown.
     * If this property is true the filter are visible; if the property is false, no filter are shown. The default value is true.
     * @see bool showFilter() const
     * @see void setShowFilter(bool show)
     */
    Q_PROPERTY(bool showFilter READ showFilter WRITE setShowFilter)
    //! @property(showFixedRows)
    /**
     * This property holds whether fixed rows are shown.
     * If this property is true fixed rows are show as a seperate table view above the main view; if the property is false, no fixed rows are shown. The default value is false.
     * @see bool showFixedRows() const
     * @see void setShowFixedRows(bool show)
     */
    Q_PROPERTY(bool showFixedRows READ showFixedRows WRITE setShowFixedRows)
    //! @property(showGrid)
	/**
	 * This property holds whether the grid is shown.
	 * If this property is true a grid is drawn for the table; if the property is false, no grid is drawn. The default value is true.
     * @see bool showGrid() const
     * @see void setShowGrid(bool show)
	 */
	Q_PROPERTY(bool showGrid READ showGrid WRITE setShowGrid)
    //! @property(showSortIndicator)
    Q_PROPERTY(bool showSortIndicator READ isSortIndicatorShown WRITE setSortIndicatorShown)
    //! @property(splitView)
    /**
     * This property holds whether the view is splitted
     * If this property is true is splitted into two table view; if the property is false, a single view is shown. The default value is false.
     * @see bool viewSplitted() const
     * @see void splitView(bool split)
     */
    Q_PROPERTY(bool splitView READ viewSplitted WRITE splitView)
	//! @property(sortingEnabled)
    /**
      * This property holds whether sorting is enabled.
      *
      * If this property is true, sorting is enabled for the table. If this property is false, sorting is not enabled. The default value is false.
      *
      * @note  Setting the property to true with setSortingEnabled() immediately triggers a call to sortByColumn() with the current sort section and order.
      * @see isSortingEnabled()
      * @see setSortingEnabled()
      */
    Q_PROPERTY(bool sortingEnabled READ isSortingEnabled WRITE setSortingEnabled)
	//! @property(textElideMode)
    /**
      * This property holds the position of the "..." in elided text.
      *
      * The default value for all item views is Qt::ElideRight.
      */
    Q_PROPERTY(Qt::TextElideMode textElideMode READ textElideMode WRITE setTextElideMode)
    //! @property(wordWrap)
	/**
	 * This property holds the item text word-wrapping policy.
     *
     * If this property is true then the item text is wrapped where necessary at word-breaks; otherwise it is not wrapped at all. This property is true by default.
     *
     * Note that even of wrapping is enabled, the cell will not be expanded to fit all text. Ellipsis will be inserted according to the current textElideMode.
	 */
	 Q_PROPERTY(bool wordWrap READ wordWrap WRITE setWordWrap)
public:
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
      * Returns the column in which the given x-coordinate, @p x, in contents coordinates is located.
      * @note This function returns -1 if the given coordinate is not valid (has no column).
      * @see rowAt()
      */
    int	columnAt(int x) const;
    /**
      * Returns a list filter types allowed for the specfied @p column.
      * The list is empty if the specified @p column does not exists or all types are allowed.
      * @see setColumnFilterTypes()
      */
    QVariantList columnsFilterTypes(int column) const;
    /**
      * Returns the width of the given @p column.
      * @see setColumnWidth()
      */
    int columnWidth(int column) const;
    /**
      *
      */
    QMenu* createStandardContextMenu(const QModelIndex &index);
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
	 * Returns true if drag is enabled. Otherwise false.
	 */
	bool dragEnabled() const;
	/**
	 * Returns the current edit trigger settings.
	 * @see QAbstractItemView::editTriggers()
	 */
	QAbstractItemView::EditTriggers editTriggers() const;
    /**
      * Returns the filter at the specified @p row and @p column, or 0 if no filter exists at @p row and @p column.
      */
    QAbstractFilter* filterAt(int row, int column) const;

	QIcon filterDisabledIcon() const;
	QIcon filterEnabledIcon() const;
    /**
      * Returns the filter model that this view is using for filtering.
      */
    QAbstractFilterModel* filterModel() const;
    /**
      * Retunr the filter proxy model that this view is using.
      */
    QAbstractFilterProxyModel* filterProxyModel() const;
    /**
      * Returns the indexes in the given @p column for the rows where all columns are fixed.
      */
    QModelIndexList fixedRows(int column = 0) const;
    /**
     * Returns true if autoscrolling in drag move events is enabled.
	 * @see setAutoScroll()
     */
    bool hasAutoScroll() const;
    /**
	 * Returns the table view's horizontal header.
	 */
	QHeaderView* horizontalHeader() const;
	/**
	 * Returns the style used when drawing the grid.
	 * @see setGridStyle()
	 */
	Qt::PenStyle gridStyle() const;

    QSize iconSize() const;
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
      * Returns true if the fixed rows are show. Otherwise false.
      * @see setShowFixedRows()
      */
    bool showFixedRows() const;
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
      * Returns the item delegate used by this view and model for the given @p column. You can call itemDelegate() to get a pointer to the current delegate for a given index.
      * @see setItemDelegateForColumn() setItemDelegateForRow() itemDelegate()
      */
    QAbstractItemDelegate* itemDelegateForColumn(int column) const;
    /**
      * Returns the item delegate used by this view and model for the given @p row, or 0 if no delegate has been assigned. You can call itemDelegate() to get a pointer to the current delegate for a given index.
      * @see setItemDelegateForRow() setItemDelegateForColumn() itemDelegate()
      */
    QAbstractItemDelegate* itemDelegateForRow(int row) const;
	/**
	 * @reimp QWidget::minimumSizeHint()
	 */
	QSize minimumSizeHint() const;
    /**
      * Returns the model used by this view.
      */
    QAbstractItemModel* model() const;
    /**
      * Restores the filter definition of this table view. This function returns true if the filter definition was restored; otherwise returns false.
      * @see saveState()
      */
    bool restoreFilter(const QByteArray & data);
	/**
	 * Restores the state of this advanced table view. This function returns true if the state was restored; otherwise returns false.
	 */
	bool restoreState(const QByteArray & data);
    /**
      * Returns the model index of the model's root item. The root item is the parent item to the view's toplevel items. The root can be invalid.
      */
    QModelIndex rootIndex() const;
    /**
      * Returns the row in which the given y-coordinate, @p y, in contents coordinates is located.
      * @note This function returns -1 if the given coordinate is not valid (has no row).
      */
    int rowAt(int y) const;
    /**
      * Returns the height of the given @p row.
      */
    int rowHeight(int row) const;
    /**
      * Saves the current filter definition of this table view.
      * @see restoreFilter()
      */
    QByteArray saveFilter() const;
	/**
	 * Saves the current state of this advanced table view.
	 * @see restoreState()
	 */
	QByteArray saveState();

	QAbstractItemView::SelectionBehavior selectionBehavior() const;
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
	 * If @p enable is true, the item background will be drawn using QPalette::Base and QPalette::AlternateBase; otherwise the background will be drawn using the QPalette::Base color.
	 * @see alternatingRowColors()
	 */
    void setAlternatingRowColors(bool enable);
	/**
	 * If @p enable is true auto scroll is enabled.
	 */
	void setAutoScroll(bool enable);
    /**
      * Sets the width of the given @p column to be @p width.
      * @see columnWidth()
      */
    void setColumnWidth(int column, int width);
    /**
      * Sets the custom context menu policy to @p policy.
      */
	void setContextMenuPolicy(Qt::ContextMenuPolicy policy);
    /**
      * Sets the default filter @p type for the specified @p column.
      */
    void setDefaultFilterType(int column, int type);
    /**
      * Sets the default filter @p type for the specified @p column.
      */
    void setDefaultFilterType(const QString & name, int type);
	/**
	 * If @p is true drag is enabled.
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
	void setFilterDisabledIcon(const QIcon & icon);
	void setFilterEnabledIcon(const QIcon & icon);
	void setFilterModel(QAbstractFilterModel* model);
	void setFilterProxyModel(QAbstractFilterProxyModel* proxy);
    /**
      * Sets the filter @p type for the specified @p column.
      * @see filterType()
      */
    bool setFilterType(int type, int column, int row = 0);
    /**
	 * This property holds the pen @p style used to draw the grid.
	 * @see gridStyle()
	 */
	void setGridStyle(Qt::PenStyle style);

    void setIconSize(const QSize & size);
	/**
	 * Sets the item delegate for this view and its model to @p delegate. This is useful if you want complete control over the editing and display of items.
     * @see itemDelegate()
	 */
	void setItemDelegate(QAbstractItemDelegate* delegate);
	/**
	 * Sets the given item delegate used by this view and model for the given column. All items on column will be drawn and managed by delegate instead of using the default delegate (i.e., itemDelegate()).
     * @see itemDelegate()
	 */
	void setItemDelegateForColumn(int column, QAbstractItemDelegate * delegate);
	/**
	 * Sets the given item delegate used by this view and model for the given row. All items on row will be drawn and managed by delegate instead of using the default delegate (i.e., itemDelegate()).
     * @see itemDelegate()
	 */
	void setItemDelegateForRow(int row, QAbstractItemDelegate * delegate);
	/**
	 * Sets the @p model for the view to present.
	 */
    void setModel( QAbstractItemModel* model );
    /**
      * Sets the model's root @p index.
      */
    virtual void setRootIndex(const QModelIndex & index);
    /**
      * Sets the height of the given @p row to be @p height.
      * @see rowHeight()
      */
    void setRowHeight(int row, int height);
	/*! @cond property */
	void setSelectionBehavior(QAbstractItemView::SelectionBehavior behavior);
	//! @endcond
	/**
	 * Sets the selection @p mode.
	 */
	void setSelectionMode(QAbstractItemView::SelectionMode mode);
	/**
	 * If @p show is true a sort indicator is shown.
	 */
    void setSortIndicatorShown(bool show);
	/**
	 * If @p enable is true sorting i#s enabled. Otherwise sorting is disabled.
	 * @see isSortingEnabled()
	 */
    void setSortingEnabled(bool enable);
	/**
     * Set the text elide mode to @p mode.
     * @see textElideMode()
	 */
	void setTextElideMode(Qt::TextElideMode mode);
	/**
	 * If @p wrap is true word wrapping is enabled.
	 */
	void setWordWrap(bool wrap);
    /**
      * Returns true if the filter are shown. Otherwise false.
      * @see setShowFilter()
      */
    bool showFilter() const;
    /**
	 * Returns true if the show is shwon. Otherwise false.
     * @see setShowFilter()
	 */
	bool showGrid() const;
    /**
      * Returns true if the filter are shown. Otherwise false.
      * @see setShowFilter()
      */
	QSize sizeHint() const;
    /**
      * Sorts the model by the values in the given @p column in the given @p order.
      * @see sortingEnabled
      */
    void sortByColumn(int column, Qt::SortOrder order);
	/**
	 * Return the vies's text elide mode.
	 * @see setTextElideMode()
	 */
	Qt::TextElideMode textElideMode() const;
	/**
	 * Returns the table view's vertical header.
	 */
	QHeaderView* verticalHeader() const;
    /**
      * Returns true if the view is splitted. Otherwise false.
      * @see splitView()
      */
    bool viewSplitted() const;
    /**
	 * Returns the viewport widget.
	 */
	QWidget* viewport() const;
	/**
     * Returns true if word wrap is enabled. Otherwise false.
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
	 * @see setSelection(), selectAll()
	 */
	void clearSelection();
    /**
      * Starts editing the item corresponding to the given qp index if it is editable.
      */
	void edit(const QModelIndex & index);
	/**
     * Hide the given @p column.
	 * @see showColumn(), hideRow()
	 */
	void hideColumn(int column);
	/**
	 * Hide the columns specified by the list of column @p names.
	 */
	void hideColumns(const QStringList & names);
	/**
     * Hides the filter view.
	 */
    void hideFilterView();
    /**
      * Hide the given @p row.
      * @see showRow(), hideColumn()
      */
    void hideRow(int row);
	/**
	 * Resizes the given @p column based on the size hints of the delegate used to render each item in the column.
	 * @remark Only visible columns will be resized. Reimplement sizeHintForColumn() to resize hidden columns as well.
	 */
	void resizeColumnToContents(int column);
    /**
      * Resizes all columns based on the size hints of the delegate used to render each item in the columns.
      */
    void resizeColumnsToContents();
    /**
      * Resizes the given @p row based on the size hints of the delegate used to render each item in the row.
      */
    void resizeRowToContents(int row);
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
      * Selects the given @p column in the table view if the current SelectionMode and SelectionBehavior allows columns to be selected.
      * @see selectRow()
      */
    void selectColumn(int colum);
    /**
      * Selects the given @p row in the table view if the current SelectionMode and SelectionBehavior allows rows to be selected.
      * @see selectColumn()
      */
    void selectRow(int row);
    /**
      * Sets the current item to be the item at @p index.
      */
	void setCurrentIndex(const QModelIndex & index);
	/**
	 * If @p is true the filter specified by the given @p row and @p column is enabled.
	 * @note @p row and @p column specify a cell in the view's filter view.
	 */
    void setFilterEnabled(int row, int column, bool enable);
	/**
	 * If @p visible is true the filter is shown. Otherwise the filter is hidden.
     * @see showFilter()
	 */
    void setShowFilter(bool show);
    /**
      * Enables the fixed rows mode if @p on is true.
      * @see showFixedRows()
      */
    void setShowFixedRows(bool show);
    /**
     * If @p show is true a grid is drawn for the table. Otherwise no grid is drawn.
     * @see showGrid()
     */
    void setShowGrid( bool show );
    /**
      * Show the given @p column.
      * @see hideColumn() showRow()
      */
    void showColumn(int column);
    /**
      * Show the given @p row.
      * @see hideRow() showColumn()
      */
    void showRow(int row);
    /**
      * Splits view if @p split is true.
      * @see viewSplitted()
      */
    void splitView(bool split);
    /**
     * Updates the area occupied by the given @p index.
	 */
	void update(const QModelIndex & index);
protected:
	/**
	 * @reimp QWidget::contextMenuEvent().
	 */
	virtual void contextMenuEvent(QContextMenuEvent* event);
	/**
	 * @reimp QWidget::contextMenuEvent().
	 */
	bool eventFilter(QObject *obj, QEvent *event);
	/**
	 * @reimp QWidget::contextMenuEvent().
	 */
	void showEvent(QShowEvent* event);
private slots:
	void dataModelLayoutChanged();
	void dataViewHorizontalScrollBarRangeChanged(int minimum, int maximum);
	void dataViewHorizontalScrollBarSilderMoved( int value );
	void dataViewHorizontalScrollBarValueChanged( int value );
	void filterAdded(const QModelIndex & parent, int start, int end);
	void headerViewHorizontalScrollBarRangeChanged( int min, int max );
	void headerViewHorizontalScrollBarSilderMoved( int value );
	void headerViewHorizontalScrollBarValueChanged( int value );
	void headerViewSectionResized( int logicalIndex, int oldSize, int newSize );
	void horizontalHeaderViewSectionMoved( int logicalIndex, int oldVisualIndex, int newVisualIndex );
	void horizontalHeaderSortIndicatorChanged( int logicalIndex, Qt::SortOrder order );
	void modelReset();
	void subviewReceivedFocus();
	void updateHeaderViewGeometries();
	void updateHeaderViewVerticalScrollBar( int min, int max );
	void updateHorizontalHeaderSectionSize();
	void verticalHeaderSectionClicked(int section);

	void verticalHeaderSectionResized(int logicalIndex, int oldSize, int newSize);
	void viewActivated(const QModelIndex & index);
	void viewClicked(const QModelIndex & index);
	void viewDoubleClicked(const QModelIndex & index);
	void viewEntered(const QModelIndex & index);
	void viewPressed(const QModelIndex & index);
private:
	QModelIndex mapToSource(const QModelIndex & index) const;

    QAdvancedTableViewPrivate* d;

    Ui::QAdvancedTableView *ui;
};

#endif // QADVANCEDTABLEVIEW_H
