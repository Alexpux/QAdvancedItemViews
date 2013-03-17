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
#ifndef QABSTRACTFILTER_H
#define QABSTRACTFILTER_H

#include <QMap>
#include <QMenu>
#include <QStyleOptionViewItem>
#include <QVariant>

#include <qaivlib_global.h>

class QFilterGroup;
class QFilterViewItemDelegate;
class QAbstractFilterPrivate;

//! The QAbstractFilter class provides a base class for filter definitions used with a QAdvancedTableView.
/**
  * A QAbstractFilter provides the interface and common functionality for filters used with the QAdvancedTableView or QFilterModel.
  * @section Properties
  * Properties of a QAbstractFilter can be saved and retrieved using setProperty() and property().
  */
class QAIVLIBSHARED_EXPORT QAbstractFilter
{
public:
	/**
	 * The lowest permitted type value for custom items (subclasses of QAbstractFilter or any of the standard filter). This value is used in conjunction with a reimplementation of QGraphicsItem::type() and declaring a Type enum value. Example:
	 * @code
	 * class CustomFilter : public QAbstractFilter
	 * {
	 *		....
	 *		enum { Type = UserType + 1};
	 *		...
	 * };
	 * @endcode
	 */
	static const int UserType = 65536;
    enum {
        Type = -1        
    };
    /**
      * Constructs an abstract (invalid) filter.
      */
    QAbstractFilter();
    /**
     * Constructs a QAbstractFilter with the given @p properties.
     */
    QAbstractFilter(const QMap<QString,QVariant> & properties);
    /**
     * Destroys the abstract filter.
     */
    ~QAbstractFilter();
    /**
      * Returns the column for which the filter is defined.
      */
    int column() const;
    /**
      * Adds the context menu actions of this filter to given @p menu. The actions are connected to the specified @p receiver.
      * @remarks
      * The default implementation does nothing.
      */
    virtual void addContextMenuActions(QMenu *menu, QWidget *receiver);
    /**
      * Returns the editor to be used for editing the filter data.
      * The editor's parent widget is specified by @p parent, and the item options by @p option.
      */
    virtual QWidget* createEditor(QFilterViewItemDelegate* delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const = 0;
    /**
      * Returns the filter's data for the given @p role, or an invalid QVariant if there is no data for the role.
      */
    virtual QVariant data(int role = Qt::DisplayRole) const = 0;
    /**
      * Returns the color used to highlight rows matching this filter.
      */
    QColor highlightColor() const;
    /**
      * Returns true if the filter is enabled; Otherwise false.
      * @setEnabled()
      */
    bool isEnabled() const;
    /**
     * Returns true if the filter definition is valid. Otherwise false.
     */
    bool isValid() const;
    /**
      * Returns true if the given @p value with the conditions defined by this filter. Otherwise false.
      */
    virtual bool matches(const QVariant & value, int type = -1) const = 0;
    /**
      * Returns the value of the property specified by the given @p key.
      */
    QVariant property(const QString & key, const QVariant & defaultValue = QVariant()) const;
    /**
      * Returns the filter's properties.
      */
    QMap<QString,QVariant> properties() const;
    /**
      * Returns the number of the row (group) that contains this filter.
      */
    int row() const;
    /**
      * Sets the contents of the given @p editor to the data for the filter at the given @p index. Note that the index contains information about the model being used.
      */
    virtual void setEditorData(QWidget * editor, const QModelIndex & index) = 0;
    /**
      * Sets the @p color to hightlight rows matching this filter.
      * @see highlightColor();
      */
    void setHighlightColor(const QColor & color);

    virtual void setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index) = 0;
    /**
     * If @p on is true the filter is set to enabled, otherwise the filter is set to disabled.
     * @see isEnabled()
     */
    void setEnabled( bool on );
    /**
      * Sets the filter definition's property specified by @p key to the given @p value.
      */
    void setProperty(const QString & name, const QVariant & value);
    /**
     * Returns the filter defintion's type.
     */
    int type() const;

	virtual void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index) = 0;
protected:
    /**
      * Constructs a filter definition with the given @p type and @p column index.
      */
    QAbstractFilter(int type, int row, int column);
private:
    friend class QAbstractFilterModel;
    void setRow(int row);

    QAbstractFilterPrivate* d;
};

/**
 * @ingroup utils
 * Returns the given @p filter cast to type T if @p filter is of type T; otherwise, 0 is returned.
 * @note  To make this function work correctly with custom filter, reimplement the type() function for each custom QAbstractFilter subclass.
 */
template <class T> inline T qfilter_cast(QAbstractFilter* filter)
{
    return int(static_cast<T>(0)->Type) == int(QAbstractFilter::Type)
        || (filter && int(static_cast<T>(0)->Type) == filter->type()) ? static_cast<T>(filter) : 0;
}

/**
 * @ingroup utils
 * Returns the given const @p filter cast to type T if @p filter is of type T; otherwise, 0 is returned.
 * @note  To make this function work correctly with custom filter, reimplement the type() function for each custom QAbstractFilter subclass.
 */
template <class T> inline T qfilter_cast(const QAbstractFilter* filter)
{
    return int(static_cast<T>(0)->Type) == int(QAbstractFilter::Type)
        || (filter && int(static_cast<T>(0)->Type) == filter->type()) ? static_cast<T>(filter) : 0;
}

#endif // QABSTRACTFILTER_H
