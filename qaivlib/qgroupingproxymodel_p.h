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
#ifndef QGROUPINGPROXYMODEL_P_H
#define QGROUPINGPROXYMODEL_P_H

class QGroupingProxyModelGroup
{
public:
    QGroupingProxyModelGroup(QGroupingProxyModelGroup* parent = 0);
    QGroupingProxyModelGroup(int sourceModelRow, QGroupingProxyModelGroup* parent = 0);
    QGroupingProxyModelGroup(const QString & name, QGroupingProxyModelGroup* parent = 0);
    ~QGroupingProxyModelGroup();

    void addSourceModelRow(int row);

    void clear();

    QGroupingProxyModelGroup* child(int index) const;

    int childCount() const;

    QVariant data(int role) const;

    QGroupingProxyModelGroup* findSourceModelRow(int sourceModelRow) const;

    QGroupingProxyModelGroup *group(int sourceModelRow) const;

    int indexOf(QGroupingProxyModelGroup* group) const;

    int sourceModelRowIndexAt(int sourceModelRow) const;
    /**
      * Returns the group matching the given @p value.
      */
    QGroupingProxyModelGroup* matches(const QVariant & value) const;

    void moveSourceRow(int count);

    QGroupingProxyModelGroup* parent() const;

    void removeChild(int index);

    void removeChildAtSourceModelRow(int sourceModelRow);

    void removeSourceModelRow(int row);

    int row() const;

    int row(int sourceModelRow) const;

    void setData(const QVariant & data, int role);

    int sourceModelRow() const;

    QList<int> sourceModelRows() const;
private:
    QList<QGroupingProxyModelGroup*> cChildren;
    QMap<int,QVariant> cData;
    QGroupingProxyModelGroup* cParent;

    int cSourceModelRow;

    QList<int> cSourceModelRows;
};

#endif // QGROUPINGPROXYMODEL_P_H
