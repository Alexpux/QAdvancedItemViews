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
#include "stdafx.h"
#include "qabstractfilter.h"

#include <qadvancedtableview.h>
//#include <qfiltergroup.h>

class QAbstractFilterPrivate
{
public:
    QAbstractFilterPrivate(QAbstractFilter* af);
    ~QAbstractFilterPrivate();

    QIcon icon;
    QFilterGroup* group;
    QMap<QString,QVariant> properties;

    QAbstractFilter* f;
};

QAbstractFilterPrivate::QAbstractFilterPrivate(QAbstractFilter *af)
{
    f = af;
}

QAbstractFilterPrivate::~QAbstractFilterPrivate()
{

}

QAbstractFilter::QAbstractFilter() :
    d(new QAbstractFilterPrivate(this))
{
//    cGroup = 0;
    d->properties["enabled"] = false;
    d->properties["type"] = QAbstractFilter::Type;
    d->properties["column"] = -1;
    d->properties["highlightColor"] = QColor("red");
}

QAbstractFilter::QAbstractFilter(int type, int row, int column) :
    d(new QAbstractFilterPrivate(this))
{
    d->properties["enabled"] = false;
    d->properties["type"] = type;
    d->properties["row"] = row;
    d->properties["column"] = column;
    d->properties["highlightColor"] = QColor("red");
}

QAbstractFilter::QAbstractFilter(const QMap<QString,QVariant> & properties) :
    d(new QAbstractFilterPrivate(this))
{
    QMapIterator<QString,QVariant> mIt(properties);
    while(mIt.hasNext()){
        mIt.next();
        d->properties[mIt.key()] = mIt.value();
    }
}

QAbstractFilter::~QAbstractFilter()
{
    delete d;
}

int QAbstractFilter::column() const
{
    return d->properties.value("column", -1).toInt();
}

void QAbstractFilter::addContextMenuActions(QMenu* menu, QWidget* receiver)
{
    Q_UNUSED(menu);
    Q_UNUSED(receiver);
}

QColor QAbstractFilter::highlightColor() const
{
    return qvariant_cast<QColor>(property("highlightColor"));
}

bool QAbstractFilter::isEnabled() const
{
    return d->properties.value("enabled", false).toBool();
}

bool QAbstractFilter::isValid() const
{
    return d->properties.value("type").toInt() != QAbstractFilter::Type &&
            d->properties.value("column").toInt() > -1;
}

QMap<QString,QVariant> QAbstractFilter::properties() const
{
    return d->properties;
}

int QAbstractFilter::row() const
{
    return d->properties.value("row", -1).toInt();
}

QVariant QAbstractFilter::property(const QString & key, const QVariant & defaultValue) const
{
    return d->properties.value(key, defaultValue);
}

void QAbstractFilter::setEnabled(bool on)
{
    d->properties["enabled"] = on;
}

void QAbstractFilter::setHighlightColor(const QColor & color)
{
    setProperty("highlightColor", color);
}

void QAbstractFilter::setProperty(const QString & name, const QVariant & value)
{
	d->properties[name] = value;
}

void QAbstractFilter::setRow(int row)
{
    d->properties["row"] = row;
}

int QAbstractFilter::type() const
{
    return d->properties.value("type", QAbstractFilter::Type).toInt();
}
