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
#ifndef QFILTEREDITORWIDGET_H
#define QFILTEREDITORWIDGET_H

#include <QWidget>
#include <QAbstractItemDelegate>

class QFilterEditorPopupWidget;

//! The class QFilterEditorBase provides the base class for implenting a filter editor
class QFilterEditorWidget : public QWidget
{
	Q_OBJECT
public:
	/**
	 * Constructs a QFilterEditorBase which is a child of parent.
	 */
	QFilterEditorWidget(QWidget *parent);
	/**
	 * Destroys the QFilterEditorBase.
	 */
	~QFilterEditorWidget();
	/**
	 * Returns the popup widget, or 0 if the editor does not have a popup widget.
	 * @see setPopup()
	 */
	QFilterEditorPopupWidget* popup() const;
	/**
	 * Sets the popup widget to @p widget.
	 * @sa popup();
	 */
	void setPopup(QFilterEditorPopupWidget* widget);	
signals:
	/**
	 * This signal must be emitted when the editor widget has completed editing the data, wants to write it back into the model.
	 */
	void commit();
	/**
	 * This signal must be emitted when the editor widget has completed editing the data, wants to write it back into the model
	 * and to close the editor.
	 *
	 * The hint provides a way for the delegate to influence how the model and view behave after editing is completed.
	 * It indicates to these components what action should be performed next to provide a comfortable editing experience for the user. 
	 * For example, if EditNextItem is specified, the view should use a delegate to open an editor on the next item in the model.
	 */
	void commitAndClose(QAbstractItemDelegate::EndEditHint hint = QAbstractItemDelegate::NoHint);
	/**
	 * This signal must be emitted when editing shall be canceled and to close the editor.
	 */
	void cancelAndClose(QAbstractItemDelegate::EndEditHint hint = QAbstractItemDelegate::NoHint);
public slots:
	/**
	 * If @p visible is true, the popop widget is shown.
	 */
	void showPopup(bool visible = true);
protected:
	/**
	 * @reimp QWidget::eventFilter()
	 */
	virtual bool eventFilter(QObject* object, QEvent *event);
	/**
	 * @reimp QWidget::moveEvent()
	 */
	void moveEvent(QMoveEvent* e);
private:
	void movePopup();

	QFilterEditorPopupWidget* m_popup;
};

#endif // QFILTEREDITORWIDGET_H
