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
#ifndef QCLICKABLELABEL_H
#define QCLICKABLELABEL_H

#include <QLabel>

#include <qaivlib_global.h>

//! The QClickableLabel widget provides a text or image display and responses to mouse button clicks.
/** @ingroup widgets
  * QClickableLabel is used for displaying text or an image. User interaction functionality is provided.
  * The visual appearance of the label can be configured in various ways, and it can be used for specifying a focus mnemonic key for another widget.
  *
  * A QClickableLabel can contain any of the content types specified for QLabel.
  *
  * @code
  * QClickableLabel* clickableLabel = new QClickableLabel(this);
  * clickableLabel->setText(tr("Clickable Label"));
  * connect(clickableLabel, SIGNAL(clicked(Qt::MouseButtons)), this, SLOT(clickedOnLabel(Qt::MouseButtons)));
  * @endcode
  */
class QAIVLIBSHARED_EXPORT QClickableLabel : public QLabel
{
    Q_OBJECT
public:
    /**
      * Constructs an empty label.
      *
      * The @p parent is passed to the QLabel constructor.
      */
    QClickableLabel(QWidget* parent = 0);
    /**
      * Destroys the clickable label.
      */
    ~QClickableLabel();
signals:
    /**
      * This signal is emitted when the button is activated (i.e. pressed down then released while the mouse cursor is inside the label.
      * @p buttons contains the state of the mouse buttons. E.g. left mouse button clicked.
      */
    void clicked(Qt::MouseButtons buttons);
protected:
    /**
      * Reimplemented from QLabel::mousePressEvent(QMouseEvent* event).
      */
    void mousePressEvent(QMouseEvent* event);
};

#endif // QCLICKABLELABEL_H
