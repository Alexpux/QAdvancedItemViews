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

bool QFilterEditorWidget::eventFilter(QObject *obj, QEvent *event)
{
	if (m_popup && event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if (popup()->cancelAndClose(obj, keyEvent->key())){
			emit cancelAndClose();
			return true;
		}
		if (popup()->commitAndClose(obj, keyEvent->key())){
			emit commitAndClose();
			return true;
		}
	}
	return QObject::eventFilter(obj, event);
}

void QFilterEditorWidget::moveEvent(QMoveEvent* e)
{
	QWidget::moveEvent(e);
	if (m_popup){
		m_popup->move(mapToGlobal(rect().bottomLeft()));
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
			m_popup->move(mapToGlobal(rect().bottomLeft()));
		} else {
			m_popup->hide();
		}
	}
}
