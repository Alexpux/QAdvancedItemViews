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
#include "qrangefilter.h"

#include <QDate>
#include <QDateTime>
#include <QFormLayout>
#include <QLineEdit>

#include <qrangefilter_p.h>
#include <qfilterviewitemdelegate.h>

QRangeFilterEditorPopup::QRangeFilterEditorPopup(QWidget* parent) :
	QFilterEditorPopupWidget(parent)
{
    m_rangeFrom = new QLineEdit(this);
    m_rangeTo = new QLineEdit(this);

    QFormLayout* l = new QFormLayout(this);
    l->addRow(tr("From:"), m_rangeFrom);
    l->addRow(tr("To:"), m_rangeTo);
    setLayout(l);

	m_rangeFrom->installEventFilter(parent);
	m_rangeTo->installEventFilter(parent);

	m_rangeFrom->setFocus();
}

QRangeFilterEditorPopup::~QRangeFilterEditorPopup()
{
}

QString QRangeFilterEditorPopup::rangeFrom() const
{
	return m_rangeFrom->text();
}

QString QRangeFilterEditorPopup::rangeTo() const
{
	return m_rangeTo->text();
}

void QRangeFilterEditorPopup::setRangeFrom( const QString & text )
{
	m_rangeFrom->setText(text);
}

void QRangeFilterEditorPopup::setRangeTo( const QString & text )
{
	m_rangeTo->setText(text);
}


QRangeFilterEditor::QRangeFilterEditor( QWidget* parent )
    : QFilterEditorWidget(parent)
{
	setPopup(new QRangeFilterEditorPopup(this));
}

QRangeFilterEditor::~QRangeFilterEditor()
{
}

bool QRangeFilterEditor::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if (keyEvent->key() == Qt::Key_Return){
			emit commitAndClose();
			return true;
		} else {
			return QObject::eventFilter(obj, event);
		}
 	} else {
		return QObject::eventFilter(obj, event);
	}
}
//-----------------------------------------------
// class QRangeFilter
//-----------------------------------------------

QRangeFilter::QRangeFilter(int row, int column) :
    QAbstractFilter(QRangeFilter::Type, row, column)
{

}

QRangeFilter::~QRangeFilter()
{
}

QWidget* QRangeFilter::createEditor(QFilterViewItemDelegate* delegate, QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
	QRangeFilterEditor* e = new QRangeFilterEditor(parent);
	QObject::connect(e, SIGNAL(commitAndClose(QAbstractItemDelegate::EndEditHint)), delegate, SLOT(commitAndClose(QAbstractItemDelegate::EndEditHint)));
    return e;
}

QVariant QRangeFilter::data(int role) const
{
    if (role == Qt::DisplayRole){
		if (property("rangeFrom").toString().isNull() && property("rangeTo").toString().isNull()){
			return QObject::tr("<any> - <any>");
		} else if (property("rangeFrom").toString().isNull() && !property("rangeTo").toString().isNull()){
	        return QString(QObject::tr("<any> - %1")).arg(property("rangeTo").toString());
		} else if (!property("rangeFrom").toString().isNull() && property("rangeTo").toString().isNull()){
	        return QString(QObject::tr("%1 - <any>")).arg(property("rangeFrom").toString());
		}
        return QString("%1 - %2").arg(property("rangeFrom").toString()).arg(property("rangeTo").toString());
    }
    return QVariant();
}

bool QRangeFilter::matches(const QVariant & value, int type) const
{
	Q_UNUSED(type);
    if (value.type() == QVariant::Char){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toChar() >= property("rangeFrom").toString().at(0) && value.toChar() <= property("rangeTo").toString().at(0);
        } else if (property("rangeFrom").isValid()){
            return value.toChar() >= property("rangeFrom").toString().at(0);
        } else if (property("rangeTo").isValid()){
            return value.toChar() <= property("rangeTo").toString().at(0);
        }
    } else if (value.type() == QVariant::Date){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toDate() >= property("rangeFrom").toDate() && value.toDate() <= property("rangeTo").toDate();
        } else if (property("rangeFrom").isValid()){
            return value.toDate() >= property("rangeFrom").toDate();
        } else if (property("rangeTo").isValid()){
            return value.toDate() <= property("rangeTo").toDate();
        }
    } else if (value.type() == QVariant::DateTime){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toDateTime() >= property("rangeFrom").toDateTime() && value.toDateTime() <= property("rangeTo").toDateTime();
        } else if (property("rangeFrom").isValid()){
            return value.toDateTime() >= property("rangeFrom").toDateTime();
        } else if (property("rangeTo").isValid()){
            return value.toDateTime() <= property("rangeTo").toDateTime();
        }
    } else if (value.type() == QVariant::Double){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toDouble() >= property("rangeFrom").toDouble() && value.toDouble() <= property("rangeTo").toDouble();
        } else if (property("rangeFrom").isValid()){
            return value.toDouble() >= property("rangeFrom").toDouble();
        } else if (property("rangeTo").isValid()){
            return value.toDouble() <= property("rangeTo").toDouble();
        }
    } else if (value.type() == QVariant::Int){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toInt() >= property("rangeFrom").toInt() && value.toInt() <= property("rangeTo").toInt();
        } else if (property("rangeFrom").isValid()){
            return value.toInt() >= property("rangeFrom").toInt();
        } else if (property("rangeTo").isValid()){
            return value.toInt() <= property("rangeTo").toInt();
        }
    } else if (value.type() == QVariant::LongLong){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toLongLong() >= property("rangeFrom").toLongLong() && value.toLongLong() <= property("rangeTo").toLongLong();
        } else if (property("rangeFrom").isValid()){
            return value.toLongLong() >= property("rangeFrom").toLongLong();
        } else if (property("rangeTo").isValid()){
            return value.toLongLong() <= property("rangeTo").toLongLong();
        }
    } else if (value.type() == QVariant::String){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toString() >= property("rangeFrom").toString() && value.toString() <= property("rangeTo").toString();
        } else if (property("rangeFrom").isValid()){
            return value.toString() >= property("rangeFrom").toString();
        } else if (property("rangeTo").isValid()){
            return value.toString() <= property("rangeTo").toString();
        }
    } else if (value.type() == QVariant::Time){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toTime() >= property("rangeFrom").toTime() && value.toTime() <= property("rangeTo").toTime();
        } else if (property("rangeFrom").isValid()){
            return value.toTime() >= property("rangeFrom").toTime();
        } else if (property("rangeTo").isValid()){
            return value.toTime() <= property("rangeTo").toTime();
        }
    } else if (value.type() == QVariant::UInt){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toUInt() >= property("rangeFrom").toUInt() && value.toUInt() <= property("rangeTo").toUInt();
        } else if (property("rangeFrom").isValid()){
            return value.toUInt() >= property("rangeFrom").toUInt();
        } else if (property("rangeTo").isValid()){
            return value.toUInt() <= property("rangeTo").toUInt();
        }
    } else if (value.type() == QVariant::ULongLong){
        if (property("rangeFrom").isValid() && property("rangeTo").isValid()){
            return value.toULongLong() >= property("rangeFrom").toULongLong() && value.toULongLong() <= property("rangeTo").toULongLong();
        } else if (property("rangeFrom").isValid()){
            return value.toULongLong() >= property("rangeFrom").toULongLong();
        } else if (property("rangeTo").isValid()){
            return value.toULongLong() <= property("rangeTo").toULongLong();
        }
    }
    return true;
}

void QRangeFilter::setEditorData(QWidget * editor, const QModelIndex & index)
{
	Q_UNUSED(index);
    QRangeFilterEditor* w = qobject_cast<QRangeFilterEditor*>(editor);
    if (w){
		QRangeFilterEditorPopup* p = qobject_cast<QRangeFilterEditorPopup*>(w->popup());
        p->setRangeFrom(property("rangeFrom").toString());
        p->setRangeTo(property("rangeTo").toString());
    }
}

void QRangeFilter::setModelData(QWidget* editor, QAbstractItemModel * model, const QModelIndex & index)
{
	Q_UNUSED(index);
    QRangeFilterEditor* w = qobject_cast<QRangeFilterEditor*>(editor);
    if (w){
		QRangeFilterEditorPopup* p = qobject_cast<QRangeFilterEditorPopup*>(w->popup());
        QVariantMap properties(index.data(Qt::EditRole).toMap());
        if (p->rangeFrom().isEmpty()){
            properties["rangeFrom"] = QVariant();
        } else {
            properties["rangeFrom"] = p->rangeFrom();
        }
        if (p->rangeTo().isEmpty()){
            properties["rangeTo"] =  QVariant();
        } else {
            properties["rangeTo"] = p->rangeTo();
        }
        model->setData(index, properties);
    }
}

void QRangeFilter::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem & option, const QModelIndex & index)
{
	QRangeFilterEditor* e = qobject_cast<QRangeFilterEditor*>(editor);
	e->setGeometry(option.rect);
	e->showPopup();
}

QDebug operator<<(QDebug d, const QRangeFilter & f)
{
    d << "(QRegExpFilter:"
      << "row:" << f.row()
      << "column:" << f.column()
      << "enabled:" << f.isEnabled()
      << "from:" << f.property("rangeFrom")
      << "to:" << f.property("rangeTo")
      << ")";
    return d.space();
}
