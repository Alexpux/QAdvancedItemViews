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
#ifndef QFILTERGROUP_H
#define QFILTERGROUP_H

#include <QString>

#include <qaivlib_global.h>

#include <qabstractfilter.h>

//! The QFilterGroup class groups filter definitions.
class QAIVLIBSHARED_EXPORT QFilterGroup
{
public:
    /**
     * Constructs an invalid QFilterGroup.
     */
    QFilterGroup();
    /**
     * Constructs a QFilterGroup with the given @p name.
     */
    QFilterGroup(const QString & name);
    /**
     * Destroys the QFilterGroup.
     */
    ~QFilterGroup();
    /**
      * Adds the given @p filter to this group. The group takes the ownership of the @p filter.
      */
    void add(QAbstractFilter* filter);
    /**
     * Returns the number of filter defined in this group.
     */
    int count() const;
    /**
     * Returns the filter at the given @p index. If @p index is out of range, an invalid QFilterTableViewColumnFilter is returned.
     * @see QFilterGroup::isValid()
     */
    QAbstractFilter* filter(int index) const;
    /**
     * Returns the filter for the specified @p column. If no filter is defined for the specified @p column, an invalid filter is returned.
     * @see QFilterGroup::isValid()
     */
    QAbstractFilter* filterAtColumn(int column) const;
    /**
      * Returns a list of filter included in this gorup.
      */
    QList<QAbstractFilter*> filters() const;
    /**
     * Returns true if the column specified by the given @p index has a filter. Returns false if the column @p index does not exist or
     * no filter is defined.
     */
    bool hasFilter(int index) const;
    /**
     * Returns the filter group's name.
     * @see setName()
     */
    QString name() const;
    /**
     * Removes the filter for the speicified @p column.
     */
    bool remove(int column);
    /**
     * Sets the filter group's @p name.
     * @see name()
     */
    void setName(const QString & name);
private:
    QList<QAbstractFilter*> cFilterList;
    bool cEnabled;
    QString cName;
};

#endif // QFILTERGROUP_H
