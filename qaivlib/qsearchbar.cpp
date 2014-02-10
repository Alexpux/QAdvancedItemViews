/******************************************************************************
** This file is part of qadvanceditemviews.
**
** Copyright (c) 2011-2014 Martin Hoppe martin@2x2hoppe.de
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
#include "qsearchbar.h"
#include "qsearchbar_p.h"
#include "ui_qsearchbar.h"

QSearchBarPrivate::QSearchBarPrivate()
{
}

QSearchBarPrivate::~QSearchBarPrivate()
{
}

QSearchBar::QSearchBar(QWidget *parent)
	: QWidget(parent), d(new QSearchBarPrivate())
{
	ui = new Ui::QSearchBar();
	ui->setupUi(this);
}

QSearchBar::~QSearchBar()
{
	delete ui;
}
/**
 * Clears the current expression and result.
 */
void QSearchBar::clear()
{
	ui->expressionLineEdit->clear();
}

void QSearchBar::expressionChangedHandler(const QString & text)
{
	emit expressionChanged(text);
}

void QSearchBar::match(QAbstractItemModel* model)
{
	if (model == 0){
		return;
	}
	d->results = model->match(model->index(0, 0), Qt::DisplayRole, ui->expressionLineEdit->text());
}
