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
#include "qconditionaldecoration.h"
#include "qconditionaldecorationproxymodel.h"

QConditionalDecoration::QConditionalDecoration() :
    QAbstractItemModelDecoration()
{
    setProperty("dataRole", Qt::DisplayRole);
    setProperty("pixmapSize", QSize(16, 16));
    setProperty("column", -1);
}

QConditionalDecoration::QConditionalDecoration(int column) :
    QAbstractItemModelDecoration()
{
    setProperty("dataRole", Qt::DisplayRole);
    setProperty("column", column);
    setProperty("pixmapSize", QSize(16, 16));
}

void QConditionalDecoration::addCondition(QConditionalDecoration::MatchFlag matchFlag, const QVariant & value, const QString & set, const QString & name)
{
    QVariantList conditions  = property("conditions").toList();
    QVariantMap condition;
	condition["column"] = property("column").toInt();
    condition["matchFlag"] = matchFlag;
    condition["value"] = value;
    condition["set"] = set;
    condition["name"] = name;
    conditions.append(condition);
    setProperty("conditions", conditions);
}

QConditionalDecoration::MatchFlag QConditionalDecoration::matchFlag(int index) const
{
    if (index < property("conditions").toList().size()){
        return static_cast<QConditionalDecoration::MatchFlag>(property("conditions").toList().at(index).toMap().value("matchFlag").toInt());
    }
    return QConditionalDecoration::MatchFlagInvalid;
}

int QConditionalDecoration::count() const
{
    return property("conditions").toList().size();
}

QVariant QConditionalDecoration::decorate(const QModelIndex & index, int role) const
{
    const QConditionalDecorationProxyModel* model = qobject_cast<const QConditionalDecorationProxyModel*>(index.model());
    if (model == 0){
        return QVariant();
    }
    QVariantList definitions  = property("conditions").toList();
    QVariantMap m;
	QList<QPixmap> pl;
    for (int iDefinitions = 0; iDefinitions < definitions.size(); iDefinitions++){
        m = definitions.at(iDefinitions).toMap();
        if (matches(index, m)){
			pl << model->icon(m.value("set").toString(), m.value("name").toString()).pixmap(model->iconSize());
			//return model->icon(p.value("set").toString(), p.value("name").toString()).pixmap(property("pixmapSize").toSize());
        }
    }
	if (!pl.isEmpty()){
		QPixmap p(model->iconSize().width() * pl.size() + (pl.size() * model->iconSpacing()), model->iconSize().height());
		p.fill(Qt::transparent);
		QPainter painter(&p);	
		for (int i = 0; i < pl.size(); i++){
			painter.drawPixmap(model->iconSize().width() * i + (i * model->iconSpacing()), 0, pl.at(i));
		}
		return p;
	} else {
		if (property("defaultSet").isNull()){
			return QVariant();
		}
	    return model->icon(property("defaultSet").toString(), property("defaultName").toString());
	}
}

QString QConditionalDecoration::iconName(int index) const
{
    if (index < property("conditions").toList().size()){
        return property("conditions").toList().at(index).toMap().value("name").toString();
    }
    return QString::null;
}

bool QConditionalDecoration::matches(const QModelIndex & index, const QVariantMap & properties) const
{
    QVariant data;
	data = index.model()->index(index.row(), properties.value("column", index.column()).toInt()).data(property("dataRole").toInt());
    QConditionalDecoration::MatchFlag matchFlag = static_cast<QConditionalDecoration::MatchFlag>(properties.value("matchFlag").toInt());
    if (matchFlag == QConditionalDecoration::Contains){
        if (data.toString().contains(properties.value("value").toString())){
            return true;
        }
    } else if (matchFlag == QConditionalDecoration::ContainsNot){
        if (!data.toString().contains(properties.value("value").toString())){
            return true;
        }
    } else if (matchFlag == QConditionalDecoration::EndsNotWith){
        if (!data.toString().endsWith(properties.value("value").toString())){
            return true;
        }
    } else if (matchFlag == QConditionalDecoration::EndsWith){
        if (data.toString().endsWith(properties.value("value").toString())){
            return true;
        }
    } else if (matchFlag == QConditionalDecoration::IsBetween){
        if (greaterOrEqualThan(data, properties.value("from")) && lessOrEqualThan(data, properties.value("to"))){
            return true;
        }
    } else if (matchFlag == QConditionalDecoration::IsEmpty){
        if (data.toString().isEmpty()){
            return true;
        }
    } else if (matchFlag == QConditionalDecoration::IsEqual){
        if (data == properties.value("value")){
            return true;
        }
    } else if(matchFlag == QConditionalDecoration::IsGreater){
        if (greaterThan(data, properties.value("value"))){
            return true;
        }
    } else if (matchFlag == QConditionalDecoration::IsGreaterOrEqual){
        if (greaterOrEqualThan(data, properties.value("value"))){
            return true;
        }
    } else if (matchFlag == QConditionalDecoration::IsLess){
        if (lessThan(data, properties.value("value"))){
            return true;
        }
    } else if(matchFlag == QConditionalDecoration::IsLessOrEqual){
        if (lessOrEqualThan(data, properties.value("value"))){
            return true;
        }
    } else if (matchFlag == QConditionalDecoration::IsNotBetween){
        if (lessThan(data, properties.value("from")) || greaterThan(data, properties.value("to"))){
            return true;
        }
    } else if (matchFlag == QConditionalDecoration::IsNotEmpty){
        if (!data.toString().isEmpty()){
            return true;
        }
    } else if (matchFlag == QConditionalDecoration::IsNotEqual){
        if (data != properties.value("value")){
            return true;
        }
    } else if (matchFlag == QConditionalDecoration::StartsNotWith){
        if (!data.toString().startsWith(properties.value("value").toString())){
            return true;
        }
    } else if (matchFlag == QConditionalDecoration::StartsWith){
        if (data.toString().startsWith(properties.value("value").toString())){
            return true;
        }
    }
    return false;
}

QVariantMap QConditionalDecoration::properties(int index) const
{
    if (index < property("conditions").toList().size()){
        return property("conditions").toList().at(index).toMap();
    }
    return QVariantMap();
}

void QConditionalDecoration::setHighlightRole(int role)
{
    setProperty("dataRole", role);
}

void QConditionalDecoration::setDefaultDecoration(const QString & set, const QString & name)
{
    setProperty("defaultSet", set);
    setProperty("defaultName", name);
}

QVariant QConditionalDecoration::value(int index) const
{
    if (index < property("conditions").toList().size()){
        return property("conditions").toList().at(index).toMap().value("value");
    }
    return QVariant();
}

bool QConditionalDecoration::equal(const QVariant &left, const QVariant &right) const
{
    if (left.type() == QVariant::Char){
        if (left.toChar() == right.toString().at(0)){
            return true;
        }
    } else if (left.type() == QVariant::Date){
        if (left.toDate() == right.toDate()){
            return true;
        }
    } else if (left.type() == QVariant::DateTime){
        if (left.toDateTime() == right.toDateTime()){
            return true;
        }
    } else if (left.type() == QVariant::Double){
        if (left.toDouble() == right.toDouble()){
            return true;
        }
    } else if (left.type() == QVariant::Int){
        if (left.toInt() == right.toInt()){
            return true;
        }
    } else if (left.type() == QVariant::LongLong){
        if (left.toLongLong() == right.toLongLong()){
            return true;
        }
    } else if (left.type() == QVariant::String){
        if (left.toString() == right.toString()){
            return true;
        }
    } else if (left.type() == QVariant::Time){
        if (left.toTime() == right.toTime()){
            return true;
        }
    } else if (left.type() == QVariant::UInt){
        if (left.toUInt() == right.toUInt()){
            return true;
        }
    } else if (left.type() == QVariant::ULongLong){
        if (left.toULongLong() == right.toULongLong()){
            return true;
        }
    }
    return false;
}

bool QConditionalDecoration::greaterOrEqualThan(const QVariant &left, const QVariant &right) const
{
    if (greaterThan(left, right)){
        return true;
    }
    if (equal(left, right)){
        return true;
    }
    return false;
}

bool QConditionalDecoration::greaterThan(const QVariant & left, const QVariant & right) const
{
    if (left.type() == QVariant::Char){
        if (left.toChar() > right.toString().at(0)){
            return true;
        }
    } else if (left.type() == QVariant::Date){
        if (left.toDate() > right.toDate()){
            return true;
        }
    } else if (left.type() == QVariant::DateTime){
        if (left.toDateTime() > right.toDateTime()){
            return true;
        }
    } else if (left.type() == QVariant::Double){
        if (left.toDouble() > right.toDouble()){
            return true;
        }
    } else if (left.type() == QVariant::Int){
        if (left.toInt() > right.toInt()){
            return true;
        }
    } else if (left.type() == QVariant::LongLong){
        if (left.toLongLong() > right.toLongLong()){
            return true;
        }
    } else if (left.type() == QVariant::String){
        if (left.toString() > right.toString()){
            return true;
        }
    } else if (left.type() == QVariant::Time){
        if (left.toTime() > right.toTime()){
            return true;
        }
    } else if (left.type() == QVariant::UInt){
        if (left.toUInt() > right.toUInt()){
            return true;
        }
    } else if (left.type() == QVariant::ULongLong){
        if (left.toULongLong() > right.toULongLong()){
            return true;
        }
    }
    return false;
}

bool QConditionalDecoration::lessOrEqualThan(const QVariant & left, const QVariant & right) const
{
    if (lessThan(left, right)){
        return true;
    }
    if (equal(left, right)){
        return true;
    }
    return false;
}

bool QConditionalDecoration::lessThan(const QVariant & left, const QVariant & right) const
{
    if (left.type() == QVariant::Char){
        if (left.toChar() < right.toString().at(0)){
            return true;
        }
    } else if (left.type() == QVariant::Date){
        if (left.toDate() < right.toDate()){
            return true;
        }
    } else if (left.type() == QVariant::DateTime){
        if (left.toDateTime() < right.toDateTime()){
            return true;
        }
    } else if (left.type() == QVariant::Double){
        if (left.toDouble() < right.toDouble()){
            return true;
        }
    } else if (left.type() == QVariant::Int){
        if (left.toInt() < right.toInt()){
            return true;
        }
    } else if (left.type() == QVariant::LongLong){
        if (left.toLongLong() < right.toLongLong()){
            return true;
        }
    } else if (left.type() == QVariant::String){
        if (left.toString() < right.toString()){
            return true;
        }
    } else if (left.type() == QVariant::Time){
        if (left.toTime() < right.toTime()){
            return true;
        }
    } else if (left.type() == QVariant::UInt){
        if (left.toUInt() < right.toUInt()){
            return true;
        }
    } else if (left.type() == QVariant::ULongLong){
        if (left.toULongLong() < right.toULongLong()){
            return true;
        }
    }
    return false;
}
