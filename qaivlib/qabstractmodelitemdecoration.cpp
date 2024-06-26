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

#include "qabstractmodelitemdecoration.h"

class QAbstractItemModelDecorationPrivate {
public:
    explicit QAbstractItemModelDecorationPrivate(QAbstractItemModelDecoration *md);
    ~QAbstractItemModelDecorationPrivate() = default;

    QAbstractItemModelDecoration *m { nullptr };
    QMap<QString, QVariant> properties;
};

QAbstractItemModelDecorationPrivate::QAbstractItemModelDecorationPrivate(QAbstractItemModelDecoration *md) :
    m { md }
{
}

QAbstractItemModelDecoration::QAbstractItemModelDecoration() :
    d(new QAbstractItemModelDecorationPrivate(this))
{
}

QAbstractItemModelDecoration::QAbstractItemModelDecoration(const QAbstractItemModelDecoration &c) :
    d(new QAbstractItemModelDecorationPrivate(this))
{
    d->properties = c.d->properties;
    d->properties.detach();
    d->m = c.d->m;
}

QAbstractItemModelDecoration &QAbstractItemModelDecoration::operator=(const QAbstractItemModelDecoration &c)
{
    d->properties.clear();
    d->properties = c.d->properties;
    d->properties.detach();
    d->m = c.d->m;
    return *this;
}

QAbstractItemModelDecoration::~QAbstractItemModelDecoration()
{
    delete d;
}

QIcon QAbstractItemModelDecoration::defaultIcon() const
{
    return QIcon();
}

QVariant QAbstractItemModelDecoration::property(const QString &key, const QVariant &defaultValue) const
{
    return d->properties.value(key, defaultValue);
}

QMap<QString, QVariant> QAbstractItemModelDecoration::properties() const
{
    return d->properties;
}

bool QAbstractItemModelDecoration::restoreState(const QByteArray &state)
{
    QDataStream s(state);
    QVariantMap m;
    s >> m;
    setProperties(m);
    return true;
}

QByteArray QAbstractItemModelDecoration::saveState() const
{
    QByteArray ba;
    QDataStream s(ba);
    s << d->properties;
    return ba;
}

void QAbstractItemModelDecoration::setProperty(const QString &name, const QVariant &value)
{
    d->properties[name] = value;
}

void QAbstractItemModelDecoration::setProperties(const QVariantMap &properties)
{
    d->properties = properties;
}
