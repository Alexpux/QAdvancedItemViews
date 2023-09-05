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

#include "qfiltergroup.h"

#include <algorithm>

QFilterGroup::QFilterGroup()
{
}

QFilterGroup::QFilterGroup(const QString & name) :
    cName(name)
{
}

QFilterGroup::~QFilterGroup()
{
    qDeleteAll(cFilterList);
    cFilterList.clear();
}

void QFilterGroup::add(QAbstractFilter* filter)
{
    cFilterList.append(filter);
}

int QFilterGroup::count() const
{
    return cFilterList.size();
}

QAbstractFilter* QFilterGroup::filter(int index) const
{
    if (index < cFilterList.size()) {
        return cFilterList.at(index);
    }
    return nullptr;
}

QAbstractFilter* QFilterGroup::filterAtColumn(int column) const
{
    QList<QAbstractFilter*>::const_iterator it;
    it = std::find_if(cFilterList.cbegin(), cFilterList.cend(),
                      [&column](const QAbstractFilter* mFilter) { return mFilter->column() == column; });
    if (it != cFilterList.end()) {
        return *it;
    }
    /*for (QAbstractFilter* mFilter : cFilterList) {
        if (mFilter->column() == column) {
            return mFilter;
        }
    }*/
    return nullptr;
}

QList<QAbstractFilter*> QFilterGroup::filters() const
{
    return cFilterList;
}

bool QFilterGroup::hasFilter(int index) const
{
    return std::any_of(cFilterList.cbegin(), cFilterList.cend(),
                       [&index](const QAbstractFilter* mFilter) { return mFilter->column() == index; });

    /*for (const QAbstractFilter* mFilter : cFilterList) {
        if (mFilter->column() == index) {
            return true;
        }
    }
    return false;*/
}

QString QFilterGroup::name() const
{
    return cName;
}

bool QFilterGroup::remove(int column)
{
    for (int iFilter = 0; iFilter < cFilterList.size(); iFilter++) {
        if (cFilterList.at(iFilter)->column() == column) {
            cFilterList.removeAt(iFilter);
            return true;
        }
    }
    return false;
}

void QFilterGroup::setName(const QString & name)
{
    cName = name;
}
