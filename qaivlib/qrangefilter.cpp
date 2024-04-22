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

#include "qrangefilter.h"

#include "qfilterviewitemdelegate.h"
#include "qrangefilter_p.h"

#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QEvent>
#include <QFormLayout>
#include <QKeyEvent>
#include <QLineEdit>

QRangeFilterEditorPopup::QRangeFilterEditorPopup(QWidget *parent) :
    QFilterEditorPopupWidget(parent),
    m_rangeFrom(new QLineEdit(this)),
    m_rangeTo(new QLineEdit(this))
{
    auto *l = new QFormLayout(this);
    l->addRow(tr("From:"), m_rangeFrom);
    l->addRow(tr("To:"), m_rangeTo);
    setLayout(l);

    m_rangeFrom->installEventFilter(parent);
    m_rangeTo->installEventFilter(parent);

    m_rangeFrom->setFocus();
}

QString QRangeFilterEditorPopup::rangeFrom() const
{
    return m_rangeFrom->text();
}

QString QRangeFilterEditorPopup::rangeTo() const
{
    return m_rangeTo->text();
}

void QRangeFilterEditorPopup::setRangeFrom(const QString &text)
{
    m_rangeFrom->setText(text);
}

void QRangeFilterEditorPopup::setRangeTo(const QString &text)
{
    m_rangeTo->setText(text);
}

QRangeFilterEditor::QRangeFilterEditor(QWidget *parent) :
    QFilterEditorWidget(parent)
{
    setPopup(new QRangeFilterEditorPopup(this));
}

bool QRangeFilterEditor::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        auto *keyEvent = dynamic_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return) {
            emit commitAndClose();
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

//-----------------------------------------------
// class QRangeFilter
//-----------------------------------------------

QRangeFilter::QRangeFilter(int row, int column) :
    QAbstractFilter(QRangeFilter::Type, row, column)
{
}

QWidget *QRangeFilter::createEditor(QFilterViewItemDelegate *delegate, QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    auto *e = new QRangeFilterEditor(parent);
    QObject::connect(e, &QRangeFilterEditor::commitAndClose, delegate, &QFilterViewItemDelegate::commitAndClose);
    return e;
}

QVariant QRangeFilter::data(int role) const
{
    if (role == Qt::DisplayRole) {
        QString propFrom = property("rangeFrom").toString();
        QString propTo = property("rangeTo").toString();
        if (propFrom.isNull() && propTo.isNull()) {
            return QObject::tr("<any> - <any>");
        }
        if (propFrom.isNull() && !propTo.isNull()) {
            return QString(QObject::tr("<any> - %1")).arg(propTo);
        }
        if (!propFrom.isNull() && propTo.isNull()) {
            return QString(QObject::tr("%1 - <any>")).arg(propFrom);
        }
        return QString("%1 - %2").arg(propFrom, propTo);
    }
    return QVariant();
}

bool QRangeFilter::matches(const QVariant &value, int type) const
{
    Q_UNUSED(type);
    QVariant propFrom = property("rangeFrom");
    QVariant propTo = property("rangeTo");

    switch (value.type()) {
    case QVariant::Char: {
        if (propFrom.isValid() && propTo.isValid()) {
            return value.toChar() >= propFrom.toString().at(0) && value.toChar() <= propTo.toString().at(0);
        }
        if (propFrom.isValid()) {
            return value.toChar() >= propFrom.toString().at(0);
        }
        if (propTo.isValid()) {
            return value.toChar() <= propTo.toString().at(0);
        }
        break;
    }
    case QVariant::Date: {
        if (propFrom.isValid() && propTo.isValid()) {
            return value.toDate() >= propFrom.toDate() && value.toDate() <= propTo.toDate();
        }
        if (propFrom.isValid()) {
            return value.toDate() >= propFrom.toDate();
        }
        if (propTo.isValid()) {
            return value.toDate() <= propTo.toDate();
        }
        break;
    }
    case QVariant::DateTime: {
        if (propFrom.isValid() && propTo.isValid()) {
            return value.toDateTime() >= propFrom.toDateTime() && value.toDateTime() <= propTo.toDateTime();
        }
        if (propFrom.isValid()) {
            return value.toDateTime() >= propFrom.toDateTime();
        }
        if (propTo.isValid()) {
            return value.toDateTime() <= propTo.toDateTime();
        }
        break;
    }
    case QVariant::Double: {
        if (propFrom.isValid() && propTo.isValid()) {
            return value.toDouble() >= propFrom.toDouble() && value.toDouble() <= propTo.toDouble();
        }
        if (propFrom.isValid()) {
            return value.toDouble() >= propFrom.toDouble();
        }
        if (propTo.isValid()) {
            return value.toDouble() <= propTo.toDouble();
        }
        break;
    }
    case QVariant::Int: {
        if (propFrom.isValid() && propTo.isValid()) {
            return value.toInt() >= propFrom.toInt() && value.toInt() <= propTo.toInt();
        }
        if (propFrom.isValid()) {
            return value.toInt() >= propFrom.toInt();
        }
        if (propTo.isValid()) {
            return value.toInt() <= propTo.toInt();
        }
        break;
    }
    case QVariant::LongLong: {
        if (propFrom.isValid() && propTo.isValid()) {
            return value.toLongLong() >= propFrom.toLongLong() && value.toLongLong() <= propTo.toLongLong();
        }
        if (propFrom.isValid()) {
            return value.toLongLong() >= propFrom.toLongLong();
        }
        if (propTo.isValid()) {
            return value.toLongLong() <= propTo.toLongLong();
        }
        break;
    }
    case QVariant::String: {
        if (propFrom.isValid() && propTo.isValid()) {
            return value.toString() >= propFrom.toString() && value.toString() <= propTo.toString();
        }
        if (propFrom.isValid()) {
            return value.toString() >= propFrom.toString();
        }
        if (propTo.isValid()) {
            return value.toString() <= propTo.toString();
        }
        break;
    }
    case QVariant::Time: {
        if (propFrom.isValid() && propTo.isValid()) {
            return value.toTime() >= propFrom.toTime() && value.toTime() <= propTo.toTime();
        }
        if (propFrom.isValid()) {
            return value.toTime() >= propFrom.toTime();
        }
        if (propTo.isValid()) {
            return value.toTime() <= propTo.toTime();
        }
        break;
    }
    case QVariant::UInt: {
        if (propFrom.isValid() && propTo.isValid()) {
            return value.toUInt() >= propFrom.toUInt() && value.toUInt() <= propTo.toUInt();
        }
        if (propFrom.isValid()) {
            return value.toUInt() >= propFrom.toUInt();
        }
        if (propTo.isValid()) {
            return value.toUInt() <= propTo.toUInt();
        }
        break;
    }
    case QVariant::ULongLong: {
        if (propFrom.isValid() && propTo.isValid()) {
            return value.toULongLong() >= propFrom.toULongLong() && value.toULongLong() <= propTo.toULongLong();
        }
        if (propFrom.isValid()) {
            return value.toULongLong() >= propFrom.toULongLong();
        }
        if (propTo.isValid()) {
            return value.toULongLong() <= propTo.toULongLong();
        }
        break;
    }
    default:
        break;
    }
    return true;
}

void QRangeFilter::setEditorData(QWidget *editor, const QModelIndex &index)
{
    Q_UNUSED(index);
    auto *w = qobject_cast<QRangeFilterEditor *>(editor);
    if (w) {
        auto *p = qobject_cast<QRangeFilterEditorPopup *>(w->popup());
        p->setRangeFrom(property("rangeFrom").toString());
        p->setRangeTo(property("rangeTo").toString());
    }
}

void QRangeFilter::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)
{
    Q_UNUSED(index);
    const QRangeFilterEditor *w = qobject_cast<QRangeFilterEditor *>(editor);
    if (w) {
        const QRangeFilterEditorPopup *p = qobject_cast<QRangeFilterEditorPopup *>(w->popup());
        QVariantMap properties(index.data(Qt::EditRole).toMap());
        if (p->rangeFrom().isEmpty()) {
            properties["rangeFrom"] = QVariant();
        } else {
            properties["rangeFrom"] = p->rangeFrom();
        }
        if (p->rangeTo().isEmpty()) {
            properties["rangeTo"] = QVariant();
        } else {
            properties["rangeTo"] = p->rangeTo();
        }
        model->setData(index, properties);
    }
}

void QRangeFilter::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(index)
    auto *e = qobject_cast<QRangeFilterEditor *>(editor);
    e->setGeometry(option.rect);
    e->showPopup();
}

QDebug operator<<(QDebug dbg, const QRangeFilter &f)
{
    dbg << "(QRangeFilter:"
        << "row:" << f.row()
        << "column:" << f.column()
        << "enabled:" << f.isEnabled()
        << "from:" << f.property("rangeFrom")
        << "to:" << f.property("rangeTo")
        << ")";
    return dbg.space();
}
