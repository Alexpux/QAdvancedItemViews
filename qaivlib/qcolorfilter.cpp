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

#include "qcolorfilter.h"

#include "qcheckstateproxymodel.h"
#include "qcolorfilter_p.h"
#include "qfiltermodel.h"
#include "qfilterviewitemdelegate.h"

#include <QBrush>
#include <QCheckBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFrame>
#include <QPainter>
#include <QSet>
#include <QStandardItemModel>
#include <QToolButton>
#include <QVBoxLayout>

// ============================================================================
// QColorFilterEditorPopup Implementation
// ============================================================================

QColorFilterEditorPopup::QColorFilterEditorPopup(QWidget *parent) : QFilterEditorPopupWidget(parent), m_mode(0)
{
    auto *layout = new QVBoxLayout();
    layout->setContentsMargins(6, 6, 6, 6);

    // Empty/Not Empty buttons
    auto *buttonLayout = new QVBoxLayout();
    buttonLayout->setSpacing(3);

    m_emptyToolButton = new QToolButton(this);
    m_emptyToolButton->setText(tr("Empty"));
    m_emptyToolButton->setAutoRaise(true);
    m_emptyToolButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    buttonLayout->addWidget(m_emptyToolButton);
    connect(m_emptyToolButton, &QToolButton::clicked,
            this, &QColorFilterEditorPopup::emptyToolButtonClicked);

    m_notEmptyToolButton = new QToolButton(this);
    m_notEmptyToolButton->setText(tr("Not Empty"));
    m_notEmptyToolButton->setAutoRaise(true);
    m_notEmptyToolButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    buttonLayout->addWidget(m_notEmptyToolButton);
    connect(m_notEmptyToolButton, &QToolButton::clicked,
            this, &QColorFilterEditorPopup::notEmptyToolButtonClicked);

    layout->addLayout(buttonLayout);

    // Separator
    auto *separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    // Color list view
    m_listView = new QListView(this);
    m_listView->setMinimumWidth(200);
    m_listView->setMinimumHeight(150);
    m_listView->installEventFilter(parent);
    m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(m_listView);

    // Select/Deselect all checkbox
    m_selectCheckBox = new QCheckBox(this);
    m_selectCheckBox->setText(tr("Select/Deselect all"));
    m_selectCheckBox->setTristate(true);
    m_selectCheckBox->installEventFilter(parent);

#if (QT_VERSION >= QT_VERSION_CHECK(6, 7, 0))
    connect(m_selectCheckBox, &QCheckBox::checkStateChanged, this, &QColorFilterEditorPopup::selectCheckBoxStateChanged);
#else
    connect(m_selectCheckBox, &QCheckBox::stateChanged, this, &QColorFilterEditorPopup::selectCheckBoxStateChanged);
#endif
    layout->addWidget(m_selectCheckBox);

    // Setup models
    m_model = new QStandardItemModel(this);
    m_checkStateProxy = new QCheckStateProxyModel(this);
    connect(m_checkStateProxy, &QCheckStateProxyModel::dataChanged,
            this, &QColorFilterEditorPopup::checkStateProxyDataChanged);
    m_checkStateProxy->setSourceModel(m_model);
    m_checkStateProxy->setColumnCheckable(0);
    m_listView->setModel(m_checkStateProxy);

    // Dialog buttons
    auto *buttonBox = new QDialogButtonBox(this);
    buttonBox->addButton(QDialogButtonBox::Ok);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QColorFilterEditorPopup::accepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QColorFilterEditorPopup::rejected);
    layout->addWidget(buttonBox);

    setLayout(layout);
    setFocusProxy(m_listView);
}

QVariantList QColorFilterEditorPopup::selectedColors() const
{
    QVariantList colors;
    for (const QModelIndex &index : m_checkStateProxy->checkedIndexes()) {
        colors.append(index.data(Qt::UserRole)); // Color stored in UserRole
    }
    return colors;
}

void QColorFilterEditorPopup::setSelectedColors(const QVariantList &colors)
{
    m_checkStateProxy->setCheckedValues(0, colors);
}

void QColorFilterEditorPopup::setAvailableColors(const QVariantList &colors)
{
    m_model->clear();
    m_model->setRowCount(colors.size());
    m_model->setColumnCount(1);

    for (int i = 0; i < colors.size(); ++i) {
        const QColor color = colors.at(i).value<QColor>();

        auto *item = new QStandardItem();

        // Set display text (color name or RGB values)
        if (color.isValid()) {
            QString text = color.name();
            // Add RGB values for clarity
            text += QString(" (R:%1 G:%2 B:%3)")
                        .arg(color.red())
                        .arg(color.green())
                        .arg(color.blue());
            item->setText(text);

            // Create color swatch icon
            QPixmap pixmap(32, 16);
            pixmap.fill(Qt::transparent);
            QPainter painter(&pixmap);
            painter.setRenderHint(QPainter::Antialiasing);

            // Draw color rectangle with border
            painter.setBrush(color);
            painter.setPen(QPen(Qt::black, 1));
            painter.drawRect(0, 0, 31, 15);

            item->setIcon(QIcon(pixmap));

            // Store color in UserRole for later retrieval
            item->setData(color, Qt::UserRole);
        } else {
            item->setText(tr("<No color>"));
        }

        m_model->setItem(i, 0, item);
    }
}

void QColorFilterEditorPopup::emptyToolButtonClicked()
{
    m_mode = 1;
    emit modeChanged();
}

void QColorFilterEditorPopup::notEmptyToolButtonClicked()
{
    m_mode = 2;
    emit modeChanged();
}

void QColorFilterEditorPopup::checkStateProxyDataChanged(const QModelIndex &topLeft,
                                                         const QModelIndex &bottomRight)
{
    Q_UNUSED(topLeft)
    Q_UNUSED(bottomRight)

    m_selectCheckBox->blockSignals(true);
    const int checkedCount = m_checkStateProxy->checkedIndexes().size();
    const int totalCount = m_checkStateProxy->checkableColumnsCount() * m_checkStateProxy->rowCount();

    if (checkedCount == 0) {
        m_selectCheckBox->setCheckState(Qt::Unchecked);
    } else if (checkedCount == totalCount) {
        m_selectCheckBox->setCheckState(Qt::Checked);
    } else {
        m_selectCheckBox->setCheckState(Qt::PartiallyChecked);
    }
    m_selectCheckBox->blockSignals(false);
}

#if (QT_VERSION >= QT_VERSION_CHECK(6, 7, 0))
void QColorFilterEditorPopup::selectCheckBoxStateChanged(Qt::CheckState state)
#else
void QColorFilterEditorPopup::selectCheckBoxStateChanged(int state)
#endif
{
    if (state == Qt::Checked) {
        m_checkStateProxy->setAllChecked(true);
    } else if (state == Qt::Unchecked) {
        m_checkStateProxy->setAllChecked(false);
    } else if (state == Qt::PartiallyChecked) {
        m_selectCheckBox->setChecked(true);
    }
}

bool QColorFilterEditorPopup::cancelAndClose([[maybe_unused]] QObject *obj, int key) const
{
    return key == Qt::Key_Escape;
}

bool QColorFilterEditorPopup::commitAndClose(QObject *obj, int key) const
{
    if (key == Qt::Key_Return && obj == m_listView) {
        return true;
    }
    return false;
}

// ============================================================================
// QColorFilterEditor Implementation
// ============================================================================

QColorFilterEditor::QColorFilterEditor(QWidget *parent) : QFilterEditorWidget(parent)
{
    auto *popup = new QColorFilterEditorPopup(this);
    setPopup(popup);
    setFocusProxy(popup);

    connect(popup, &QColorFilterEditorPopup::modeChanged,
            this, &QColorFilterEditor::modeSelected);
    connect(popup, &QColorFilterEditorPopup::accepted,
            this, [this]() { emit commitAndClose(); });
    connect(popup, &QColorFilterEditorPopup::rejected,
            this, [this]() { emit cancelAndClose(); });

    setFocusPolicy(Qt::StrongFocus);
}

void QColorFilterEditor::modeSelected()
{
    emit commitAndClose();
}

// ============================================================================
// QColorFilter Implementation
// ============================================================================

QColorFilter::QColorFilter(int row, int column) : QAbstractFilter(QColorFilter::Type, row, column)
{
    setProperty("mode", 0);
    setProperty("selectedColors", QVariantList());
}

QWidget *QColorFilter::createEditor(QFilterViewItemDelegate *delegate,
                                    QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    auto *editor = new QColorFilterEditor(parent);
    QObject::connect(editor, &QColorFilterEditor::cancelAndClose,
                     delegate, &QFilterViewItemDelegate::cancelAndClose);
    QObject::connect(editor, &QColorFilterEditor::commitAndClose,
                     delegate, &QFilterViewItemDelegate::commitAndClose);
    return editor;
}

QVariant QColorFilter::data(int role) const
{
    if (role == Qt::DisplayRole) {
        const int mode = property("mode").toInt();

        if (mode == 1) {
            return QString("Empty");
        }
        if (mode == 2) {
            return QString("Not Empty");
        }

        // Mode 0: Selected colors
        const QVariantList selectedColors = property("selectedColors").toList();
        if (selectedColors.isEmpty()) {
            return QString("<none>");
        }
        if (selectedColors.size() == 1) {
            return QString("%1 color").arg(selectedColors.size());
        }
        return QString("%1 colors").arg(selectedColors.size());
    }

    return QVariant();
}

bool QColorFilter::matches(const QVariant &value, int type) const
{
    Q_UNUSED(type);
    const int mode = property("mode").toInt();
    const QColor itemColor = extractColor(value);

    // Mode 1: Empty (no color)
    if (mode == 1) {
        return !itemColor.isValid();
    }

    // Mode 2: Not Empty (has color)
    if (mode == 2) {
        return itemColor.isValid();
    }

    // Mode 0: Selected colors
    if (!itemColor.isValid()) {
        return false;
    }

    const QVariantList selectedColors = property("selectedColors").toList();
    for (const QVariant &colorVariant : selectedColors) {
        const QColor selectedColor = colorVariant.value<QColor>();
        if (selectedColor.isValid() && itemColor == selectedColor) {
            return true;
        }
    }

    return false;
}

void QColorFilter::setEditorData(QWidget *editor, const QModelIndex &index)
{
    auto *colorEditor = qobject_cast<QColorFilterEditor *>(editor);
    if (!colorEditor) {
        return;
    }

    auto *popup = qobject_cast<QColorFilterEditorPopup *>(colorEditor->popup());
    if (!popup) {
        return;
    }

    // Get source model to extract available colors
    const auto *filterModel = qobject_cast<const QFilterModel *>(index.model());
    if (filterModel && filterModel->sourceModel()) {
        QVariantList availableColors = collectUniqueColors(
            filterModel->sourceModel(),
            index.column());
        popup->setAvailableColors(availableColors);
    }

    // Set previously selected colors
    const QVariantMap properties = index.data(Qt::EditRole).toMap();
    popup->setSelectedColors(properties.value("selectedColors").toList());
}

void QColorFilter::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)
{
    const auto *colorEditor = qobject_cast<QColorFilterEditor *>(editor);
    if (!colorEditor) {
        return;
    }

    const auto *popup = qobject_cast<QColorFilterEditorPopup *>(colorEditor->popup());
    if (!popup) {
        return;
    }

    QVariantMap properties = index.data(Qt::EditRole).toMap();
    properties["mode"] = popup->mode();

    if (popup->mode() == 0) {
        // Selected colors mode
        properties["selectedColors"] = popup->selectedColors();
    } else {
        // Empty or Not Empty mode - clear selected colors
        properties["selectedColors"] = QVariantList();
    }

    if (property("enableOnCommit").toBool()) {
        properties["enabled"] = true;
    }

    model->setData(index, properties);
}

void QColorFilter::updateEditorGeometry(QWidget *editor,
                                        const QStyleOptionViewItem &option,
                                        [[maybe_unused]] const QModelIndex &index)
{
    auto *colorEditor = qobject_cast<QColorFilterEditor *>(editor);
    if (colorEditor) {
        colorEditor->setGeometry(option.rect);
        colorEditor->showPopup();
    }
}

// ============================================================================
// Helper Methods
// ============================================================================

QColor QColorFilter::extractColor(const QVariant &value)
{
    // Try direct conversion to QColor
    if (value.canConvert<QColor>()) {
        return value.value<QColor>();
    }

    // Try extracting from QBrush
    if (value.canConvert<QBrush>()) {
        const QBrush brush = value.value<QBrush>();
        return brush.color();
    }

    // Return invalid color if extraction fails
    return QColor();
}

QVariantList QColorFilter::collectUniqueColors(QAbstractItemModel *model, int column)
{
    if (!model) {
        return QVariantList();
    }

    QSet<QRgb> uniqueRgbValues;
    QVariantList uniqueColors;

    const int rowCount = model->rowCount();
    for (int row = 0; row < rowCount; ++row) {
        const QModelIndex index = model->index(row, column);
        const QVariant bgValue = model->data(index, Qt::BackgroundRole);
        const QColor color = extractColor(bgValue);

        if (color.isValid()) {
            const QRgb rgb = color.rgb();
            if (!uniqueRgbValues.contains(rgb)) {
                uniqueRgbValues.insert(rgb);
                uniqueColors.append(color);
            }
        }
    }

    return uniqueColors;
}

// ============================================================================
// Debug Support
// ============================================================================

QDebug operator<<(QDebug dbg, const QColorFilter &f)
{
    const int mode = f.property("mode").toInt();
    const QVariantList selectedColors = f.property("selectedColors").toList();

    dbg.nospace() << "(QColorFilter: "
                  << "row=" << f.row() << ", "
                  << "column=" << f.column() << ", "
                  << "enabled=" << f.isEnabled() << ", "
                  << "mode=" << mode << " (";

    if (mode == 0) {
        dbg.nospace() << "selected: " << selectedColors.size() << " colors";
    } else if (mode == 1) {
        dbg.nospace() << "empty";
    } else if (mode == 2) {
        dbg.nospace() << "not empty";
    }

    dbg.nospace() << "))";
    return dbg.space();
}
