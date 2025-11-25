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
#ifndef QCOLORFILTER_H
#define QCOLORFILTER_H

#include "qabstractfilter.h"
#include "qaivlib_global.h"

#include <QColor>

//! The QColorFilter class implements a color-based filter using Qt::BackgroundRole.
/**
 * @ingroup filter
 * QColorFilter allows filtering table rows based on their background color (Qt::BackgroundRole).
 * The filter extracts available colors from the model and allows the user to select which colors
 * to include in the view.
 *
 * The filter operates in three modes:
 * - Mode 0 (Selected colors): Show only rows with selected background colors
 * - Mode 1 (Empty): Show only rows with no background color
 * - Mode 2 (Not Empty): Show only rows with any background color
 *
 * @section Properties
 * - mode (int): Filter mode (0=selected, 1=empty, 2=not empty)
 * - selectedColors (QVariantList): List of selected colors to filter by
 *
 * Example usage:
 * @code
 * auto* filter = new QColorFilter(0, 2); // row 0, column 2
 * QVariantList colors;
 * colors << QColor(Qt::red) << QColor(Qt::green);
 * filter->setProperty("selectedColors", colors);
 * filter->setProperty("mode", 0);  // Selected colors mode
 * filter->setEnabled(true);
 * @endcode
 */
class QAIVLIBSHARED_EXPORT QColorFilter : public QAbstractFilter {
public:
    enum {
        Type = 7
    };

    /**
     * Constructs a color filter for the given @p row and @p column.
     * @param row The filter row index
     * @param column The column index to filter
     */
    QColorFilter(int row, int column);

    /**
     * Copy constructor is deleted to prevent accidental copies.
     */
    QColorFilter(const QColorFilter &) = delete;

    /**
     * Copy assignment is deleted to prevent accidental copies.
     */
    QColorFilter &operator=(const QColorFilter &) = delete;

    /**
     * Destroys the color filter.
     */
    ~QColorFilter() override = default;

    /**
     * Returns the widget used to edit the filter specified by @p index for editing.
     * The @p parent widget and style @p option are used to control how the editor widget appears.
     */
    QWidget *createEditor(QFilterViewItemDelegate *delegate, QWidget *parent,
                          const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    /**
     * Returns the data stored under the given @p role for this filter.
     * @param role The data role to retrieve
     * @return Filter data for display/edit roles, invalid QVariant otherwise
     */
    QVariant data(int role = Qt::DisplayRole) const override;

    /**
     * Tests whether the given @p value matches this filter's criteria.
     * The value is expected to be a QColor or convertible to QBrush.
     * @param value The value to test (typically from Qt::BackgroundRole)
     * @param type The type hint (unused for color matching)
     * @return true if the value matches the filter criteria, false otherwise
     */
    bool matches(const QVariant &value, int type) const override;

    /**
     * Sets the contents of the given @p editor to the data for the filter at the given @p index.
     * @param editor The editor widget to populate
     * @param index The model index containing the filter data
     */
    void setEditorData(QWidget *editor, const QModelIndex &index) override;

    /**
     * Sets the data for the filter at the given @p index in the filter @p model
     * to the contents of the given @p editor.
     * @param editor The editor widget containing user input
     * @param model The filter model to update
     * @param index The model index to update
     */
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) override;

    /**
     * Updates the filter's @p editor geometry specified by index according to the style option given.
     * @param editor The editor widget to position
     * @param option Style options containing geometry information
     * @param index The model index (unused)
     */
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) override;

private:
    /**
     * Extracts a QColor from a QVariant value.
     * Handles QColor, QBrush, and invalid variants.
     * @param value The variant to extract color from
     * @return The extracted color, or invalid color if extraction fails
     */
    static QColor extractColor(const QVariant &value);

    /**
     * Collects unique colors from the source model for the filter's column.
     * @param model The source model to scan
     * @param column The column to extract colors from
     * @return List of unique colors found in the column
     */
    static QVariantList collectUniqueColors(QAbstractItemModel *model, int column);
};

/**
 * Debug stream operator for QColorFilter.
 * @param dbg Debug stream
 * @param f Filter to output
 * @return Debug stream for chaining
 */
QDebug operator<<(QDebug dbg, const QColorFilter &f);

#endif // QCOLORFILTER_H
