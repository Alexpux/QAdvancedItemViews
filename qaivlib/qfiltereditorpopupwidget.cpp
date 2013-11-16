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
#include "qfiltereditorpopupwidget.h"

/**
 * Constructs a QFilterEditorPopupWidget with the given @p parent.
 * Sets the window flags Qt::Popup and Qt::Window.
 */
QFilterEditorPopupWidget::QFilterEditorPopupWidget(QWidget *parent)
	: QFrame(parent, Qt::Popup | Qt::Window)
{
}
/**
 * Destroys the QFilterEditorPopupWidget.
 */
QFilterEditorPopupWidget::~QFilterEditorPopupWidget()
{
}

bool QFilterEditorPopupWidget::cancelAndClose(QObject* obj, int key) const
{
	Q_UNUSED(obj);
	Q_UNUSED(key);
	return false;
}

bool QFilterEditorPopupWidget::commitAndClose(QObject* obj, int key) const
{
	Q_UNUSED(obj);
	Q_UNUSED(key);
	return false;
}
