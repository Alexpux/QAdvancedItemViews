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
#ifndef QCONDITIONALDECORATIONPROXYMODEL_H
#define QCONDITIONALDECORATIONPROXYMODEL_H

#include <Qt>

#include <QSortFilterProxyModel>

#include <qaivlib_global.h>

class QAbstractItemModelDecoration;

class QConditionalDecorationProxyModelPrivate;

//! The QConditionalDecorationProxyModel class implements a proxy model for conditional decorations.
/** @ingroup proxy
  * Lets assume we want to decorate colum 'Maturity Level' in the source model show below:
  * @image html qconditionaldecorationproxymodel01.png "Source Model"
  * The code to set up the model and the view would look like this:
  * @code
  * QTableView* tableView = new QTableView(this);
  * QStandardItemModel* model = new QStandardItemModel();
  * ...
  * QConditionalDecorationProxyModel* proxy = new QConditionalDecorationProxyModel(this);
  * proxy->setSourceModel(model);
  * view->setModel(proxy);
  * @endcode
  * To add a decoration, we need to create a QConditionalDecoration and add condition defining the match conditions and values:
  * @code
  * QConditionalDecoration* decoration = new QConditionalDecoration();
  * decoration->addCondition(QConditionalDecoration::IsEqual, "Active", "leds", "green (on)");
  * decoration->addCondition(QConditionalDecoration::IsEqual, "Deprecated", "leds", "red (on)");
  * decoration->addCondition(QConditionalDecoration::IsEqual, "Done", "leds", "yellow (on)");
  * decoration->addCondition(QConditionalDecoration::IsEqual, "Maintained", "leds", "orange (on)");
  * @endcode
  * For all items matching none of the previously defined conditions, we set a default decoration:
  * @code
  * decoration->setDefaultDecoration("leds", "white (on)");
  * @endcode
  * Finally we add the decoration to the proxy model:
  * @code
  * proxy->addDecoration(3, decoration);
  * @endcode
  * After adding the decoration the view is updated and shows the decorations.
  * @image html qconditionaldecorationproxymodel02.png "Decorated Model"
  *
  * <h2>Default Icon Sets</h2>
  * <table>
  * <tr><th>Icon Set</th><th>Icon Name</th><th>Icon</th></tr>
  * <tr><td rowspan="3">emoticons</td><td>happy</td><td></td>@image html emoticon_smile.ico</tr>
  * <tr><td>neutral</td><td></td>@image html emoticon_neutral.ico</tr>
  * <tr><td>unhappy</td><td></td>@image html emoticon_unhappy.ico</tr>
  * <tr><td rowspan="7">flags</td><td>red (on)</td><td></td>@image html flag_red.ico</tr>
  * <tr><td>yellow</td><td></td>@image html flag_yellow.ico</tr>
  * <tr><td>green</td><td></td>@image html flag_green.ico</tr>
  * <tr><td>blue</td><td></td>@image html flag_blue.ico</tr>
  * <tr><td>orange</td><td></td>@image html flag_orange.ico</tr>
  * <tr><td>pink</td><td></td>@image html flag_pink.ico</tr>
  * <tr><td>purple</td><td></td>@image html flag_purple.ico</tr>
  * <tr><td rowspan="6">leds</td><td>red (on)</td><td></td>@image html red-on.ico</tr>
  * <tr><td>yellow (on)</td><td></td>@image html yellow-on.ico</tr>
  * <tr><td>green (on)</td><td></td>@image html green-on.ico</tr>
  * <tr><td>blue (on)</td><td></td>@image html blue-on.ico</tr>
  * <tr><td>orange (on)</td><td></td>@image html amber-on.ico</tr>
  * <tr><td>white (on)</td><td></td>@image html white-on.ico</tr>
  * <tr><td rowspan="7">tags</td><td>red (on)</td><td></td>@image html tag_red.ico</tr>
  * <tr><td>yellow</td><td></td>@image html tag_yellow.ico</tr>
  * <tr><td>green</td><td></td>@image html tag_green.ico</tr>
  * <tr><td>blue</td><td></td>@image html tag_blue.ico</tr>
  * <tr><td>orange</td><td></td>@image html tag_orange.ico</tr>
  * <tr><td>pink</td><td></td>@image html tag_pink.ico</tr>
  * <tr><td>purple</td><td></td>@image html tag_purple.ico</tr>
  * </table>
  * @remarks
  * Thanks to FatCow (www.fatcow.com) for providing the icons under CC Attribution 3.0 United States.
  */
class QAIVLIBSHARED_EXPORT QConditionalDecorationProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
	//! @property(iconSize: QSize)
	/**
	 * This property holds the size of item's icons.
	 * @remark The default is QSize(16, 16)
	 */
	Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
	//! @property(iconSize: int)
	/**
	 * This property holds the spacing between the icons of an item.
	 * @remark The default is 3.
	 */
	Q_PROPERTY(int iconSpacing READ iconSpacing WRITE setIconSpacing)
public:
    enum ItemDataRole {
        ConditionalDecorationRole = Qt::UserRole + 421,
        IconSetsRole
    };
    /**
      * Constructs a QConditionalDecorationProxyModel with the given @p parent.
      */
    explicit QConditionalDecorationProxyModel(QObject *parent = 0);
    /**
      * Destroys the QConditionalDecorationProxyModel object.
      */
    ~QConditionalDecorationProxyModel();
	/**
	 * Adds a @p decoration for the specified @p column.
	 */
    void addDecoration(int column, QAbstractItemModelDecoration* decoration);
    /**
      *
      */
    void addIcon(const QString & set, const QString & name, const QString & resource);
    /**
      * Adds the icon set @p name with the given @p icons to the proxy model.
      * @code
      * QConditionalDecorationProxyModel* proxy = new QConditionalDecorationProxyModel(this);
      *
      * QMap<QString,QIcon> icons;
      * icons["Custom1"] = QIcon(":/custom1");
      * icons["Custom2"] = QIcon(":/custom2");
      *
      * proxy->addIconSet("Custom", icons);
      * @endcode
      * @see addIcon()
      */
    void addIconSet(const QString & name, const QVariantMap icons);
	/**
	 * @reimp QSortFilterProxyModel::data()
	 */
    virtual QVariant data(const QModelIndex & index, int role) const;
	/**
	 * Returns the icon specified by the given @p set and @p name.
	 * An invalid QIcon is returned if the iconset @p set or the icon @p name cannot be retrieved.
	 */
    QIcon icon(const QString & set, const QString & name) const;
	/**
	 * Returns the size of the icons.
	 * @see setIconSize()
	 */
	QSize iconSize() const;
	/**
	 * Returns the spacing between the icons of an item.
	 */
	int iconSpacing() const;
    /**
      * Returns a map of icon sets defined for this proxy.
      */
    QMap<QString, QVariant> iconSets() const;
	/**
	 * Restores the proxy model's state and decoratrions.
	 */
	bool restoreState(const QByteArray & state);
	/**
	 * Saves the proxy model's state and decorations.
	 */
	QByteArray stateState() const;
	/**
	 * @reimp QSortFilterProxyModel::setData()
	 */
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role);
	/**
	 * Sets the icon size to @p size.
	 * @see iconSize()
	 */
	void setIconSize(const QSize & size);
	/**
	 * Sets the spacing between the icons of an item to @p spacing.
	 */
	void setIconSpacing(int spacing);
private:
    QConditionalDecorationProxyModelPrivate* d;
};

#endif // QCONDITIONALDECORATIONPROXYMODEL_H
