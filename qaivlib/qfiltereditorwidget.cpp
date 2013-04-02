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
#include "qfiltereditorwidget.h"
#include "qfiltereditorpopupwidget.h"

QFilterEditorWidget::QFilterEditorWidget(QWidget *parent)
	: QWidget(parent)
{
	m_popup = 0;
}

QFilterEditorWidget::~QFilterEditorWidget()
{
}

bool QFilterEditorWidget::eventFilter(QObject* object, QEvent* event)
{
	if (m_popup){
		if (event->type() == QEvent::Hide){
			emit commitAndClose();
			return true;
		} else if ( event->type() == QEvent::KeyPress){
			QKeyEvent* keyEvent = static_cast<QKeyEvent *>(event);
			if (popup()->cancelAndClose(object, keyEvent->key())){
				emit cancelAndClose();
				return true;
			}
			if (popup()->commitAndClose(object, keyEvent->key())){
				emit commitAndClose();
				return true;
			}
		}
	}
	return QObject::eventFilter(object, event);
}

void QFilterEditorWidget::moveEvent(QMoveEvent* e)
{
	QWidget::moveEvent(e);
	if (m_popup){
		movePopup();
	}
}

void QFilterEditorWidget::movePopup()
{
	if (m_popup){
		QPoint p;
		if (m_popup->geometry().height() + mapToGlobal(pos()).y() > QApplication::desktop()->availableGeometry(this).height()){
			p = mapToGlobal(rect().topLeft());
			p.setY(p.y() - m_popup->geometry().height());
		} else {
			p = mapToGlobal(rect().bottomLeft());
		}
		m_popup->move(p);
	}
}

QFilterEditorPopupWidget* QFilterEditorWidget::popup() const
{
	return m_popup;
}

void QFilterEditorWidget::setPopup(QFilterEditorPopupWidget* widget)
{
	m_popup = widget;
}

void QFilterEditorWidget::showPopup(bool visible)
{
	if (m_popup){
		if (visible){
			m_popup->show();
			movePopup();
		} else {
			m_popup->hide();
		}
	}
}
