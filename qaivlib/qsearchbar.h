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
#ifndef QSEARCHBAR_H
#define QSEARCHBAR_H

#include <QWidget>
#include <qaivlib_global.h>

namespace Ui {class QSearchBar;};

class QSearchBarPrivate;

class QAIVLIBSHARED_EXPORT QSearchBar : public QWidget
{
	Q_OBJECT
public:
	QSearchBar(QWidget* parent = 0);
	~QSearchBar();
signals:
	void expressionChanged(const QString & text);
	void searchNext();
	void searchPrevious();
public slots:
	void clear();
	void match(QAbstractItemModel* model);
	//bool match(const QModelIndex & start, int role, const QVariant & value, int hits = 1, Qt::MatchFlags flags = Qt::MatchFlags( Qt::MatchStartsWith | Qt::MatchWrap)) const;
private slots:
	void expressionChangedHandler(const QString & text);
private:
	QSearchBarPrivate* d;
	Ui::QSearchBar *ui;
};

#endif // QSEARCHBAR_H
