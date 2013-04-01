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
#include "qconditionaldecorationproxymodel.h"

#include "qabstractmodelitemdecoration.h"
#include "qconditionaldecoration.h"

class QConditionalDecorationProxyModelPrivate
{
public:
    QConditionalDecorationProxyModelPrivate(QConditionalDecorationProxyModel* pm);
    ~QConditionalDecorationProxyModelPrivate();

    QMap<int,QAbstractItemModelDecoration*> columnDecorationMap;

    QMap<QString,QVariant> iconSets;
	QSize iconSize;
	int iconSpacing;
    QConditionalDecorationProxyModel* m;
};

QConditionalDecorationProxyModelPrivate::QConditionalDecorationProxyModelPrivate(QConditionalDecorationProxyModel *pm)
{
	iconSize = QSize(16, 16);
	iconSpacing = 3;
    m = pm;
}

QConditionalDecorationProxyModelPrivate::~QConditionalDecorationProxyModelPrivate()
{
    qDeleteAll(columnDecorationMap);
}

QConditionalDecorationProxyModel::QConditionalDecorationProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent), d(new QConditionalDecorationProxyModelPrivate(this))
{
    QVariantMap m;
    m["blue"] = ":/qaiv/flag/blue";
    m["green"] = ":/qaiv/flag/green";
    m["orange"] = ":/qaiv/flag/orange";
    m["pink"] = ":/qaiv/flag/pink";
    m["purple"] = ":/qaiv/flag/purple";
    m["red"] = ":/qaiv/flag/red";
    m["yellow"] = ":/qaiv/flag/yellow";
    d->iconSets["flags"] = m;

    m.clear();
    m["red (on)"] = ":/qaiv/led/red.on";
    m["orange (on)"] = ":/qaiv/led/amber.on";
    m["blue (on)"] = ":/qaiv/led/blue.on";
    m["green (on)"] = ":/qaiv/led/green.on";
    m["white (on)"] = ":/qaiv/led/white.on";
    m["yellow (on)"] = ":/qaiv/led/yellow.on";
    d->iconSets["leds"] = m;

    m.clear();
    m["Blue"] = ":/qaiv/tag/blue";
    m["Green"] = ":/qaiv/tag/green";
    m["Orange"] = ":/qaiv/tag/orange";
    m["Pink"] = ":/qaiv/tag/pink";
    m["Purple"] = ":/qaiv/tag/purple";
    m["Red"] = ":/qaiv/tag/red";
    m["Yellow"] = ":/qaiv/tag/yellow";
    d->iconSets["tags"] = m;

    m.clear();
    m["Happy"] = ":/qaiv/emoticon/smiley.happy";
    m["Neutral"] = ":/qaiv/emoticon/smiley.neutral";
    m["Unhappy"] = ":/qaiv/emoticon/smiley.unhappy";
    d->iconSets["emoticon"] = m;
}

QConditionalDecorationProxyModel::~QConditionalDecorationProxyModel()
{
    delete d;
}

void QConditionalDecorationProxyModel::addDecoration(int column, QAbstractItemModelDecoration *highlighter)
{
    d->columnDecorationMap[column] = highlighter;
    invalidate();
}

void QConditionalDecorationProxyModel::addIcon(const QString & set, const QString & name, const QString & resource)
{
    QVariantMap m = d->iconSets.value(set).toMap();
    m[name] = resource;
    d->iconSets[set] = m;
}

void QConditionalDecorationProxyModel::addIconSet(const QString &name, const QVariantMap icons)
{
    d->iconSets[name] = icons;
}

QVariant QConditionalDecorationProxyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
        return QVariant();
    }
    if (role == Qt::DecorationRole){
        QAbstractItemModelDecoration* decoration = d->columnDecorationMap.value(index.column(), 0);
        if (decoration){
            return decoration->decorate(index);
        }
    } else if (role == QConditionalDecorationProxyModel::ConditionalDecorationRole){
        QAbstractItemModelDecoration* decoration = d->columnDecorationMap.value(index.column(), 0);
        if (decoration){
            return decoration->properties();
        }
        return QVariant();
    } else if (role == QConditionalDecorationProxyModel::IconSetsRole){
        return d->iconSets;
    }
    return QSortFilterProxyModel::data(index, role);
}

QIcon QConditionalDecorationProxyModel::icon(const QString &set, const QString &name) const
{
    QVariantMap m = d->iconSets.value(set).toMap();
    if (!m.isEmpty()){
        QVariant i = m.value(name);
        if (i.type() == QVariant::Icon){
            return qvariant_cast<QIcon>(i);
        }
        return QIcon(i.toString());
    }
    qWarning() << "icon set" << set << "not found";
    return QIcon();
}

QMap<QString, QVariant> QConditionalDecorationProxyModel::iconSets() const
{
    return d->iconSets;
}

QSize QConditionalDecorationProxyModel::iconSize() const
{
	return d->iconSize;
}

int QConditionalDecorationProxyModel::iconSpacing() const
{
	return d->iconSpacing;
}

bool QConditionalDecorationProxyModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == QConditionalDecorationProxyModel::ConditionalDecorationRole){
        QAbstractItemModelDecoration* decoration = d->columnDecorationMap.value(index.column(), 0);
        if (decoration){
            decoration->setProperties(value.toMap());
        } else {
            decoration = new QConditionalDecoration(index.column());
            decoration->setProperties(value.toMap());
            d->columnDecorationMap[index.column()] = decoration;
        }
        return true;
    }
    return QSortFilterProxyModel::setData(index, value, role);
}

void QConditionalDecorationProxyModel::setIconSize(const QSize & size)
{
	if (size != d->iconSize){
		d->iconSize = size;
		invalidateFilter();
	}
}

void QConditionalDecorationProxyModel::setIconSpacing(int spacing)
{
	if (spacing != d->iconSpacing){
		d->iconSpacing = spacing;
		invalidateFilter();
	}
}
