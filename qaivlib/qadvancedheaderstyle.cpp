#include "qadvancedheaderstyle.h"

#include "qadvancedconstants.h"

#include <QPainter>
#include <QPixmapCache>
#include <QStyleOptionHeader>
#include <QPainterPath>
#include <QStyleOptionButton>

static QWindow *qt_getWindow(const QWidget *widget)
{
    return widget ? widget->window()->windowHandle() : nullptr;
}

static QPixmap cachedPixmap(const QString &img, const QSize &pixSize)
{
    QPixmap pm;
    // Create unique cache key including size for HiDPI support
    QString cacheKey = QString("%1_%2x%3").arg(img).arg(pixSize.width()).arg(pixSize.height());
    
    if (!QPixmapCache::find(cacheKey, &pm)) {
        pm = QIcon(img).pixmap(pixSize);
        QPixmapCache::insert(cacheKey, pm);
    }
    return pm;
}

QAdvancedHeaderStyle::QAdvancedHeaderStyle(QStyle *style) :
    QProxyStyle(style),
    itemsSize(QSize(16, 16)),
    iconsSize(QSize(32, 32))
{
    // Set appropriate cache size for Qt6 - 10MB should be enough for table icons
    // This prevents cache thrashing with large tables while not using too much memory
    QPixmapCache::setCacheLimit(10240);
}

QAdvancedHeaderStyle::~QAdvancedHeaderStyle()
{
    clearCache();
}

void QAdvancedHeaderStyle::clearCache()
{
    QPixmapCache::clear();
}

QPixmap QAdvancedHeaderStyle::cached(const QString &img, const QSize &pixSize) const
{
    return ::cachedPixmap(img, pixSize);
}

void QAdvancedHeaderStyle::drawControl(ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *widget) const
{
    switch (element) {
    case CE_HeaderLook: {
        if (const QStyleOptionHeader *header = qstyleoption_cast<const QStyleOptionHeader *>(opt)) {
            QRegion clipRegion = p->clipRegion();
            p->setClipRect(opt->rect);
            proxy()->drawControl(CE_HeaderSection, header, p, widget);
            QStyleOptionHeader subopt = *header;
            subopt.rect = subElementRect(SE_HeaderLabel, header, widget);
            if (subopt.rect.isValid()) {
                proxy()->drawControl(CE_HeaderLabel, &subopt, p, widget);
            }
            if (header->sortIndicator != QStyleOptionHeader::None) {
                subopt.rect = subElementRect(SE_HeaderArrow, opt, widget);
                QPixmap arrow;
                if (header->sortIndicator == QStyleOptionHeader::SortUp) {
                    arrow = cached(HeaderSortUpIcon, itemsSize);
                } else {
                    arrow = cached(HeaderSortDownIcon, itemsSize);
                }
                proxy()->drawItemPixmap(p, subopt.rect, subopt.iconAlignment, arrow.copy());
                // proxy()->drawPrimitive(PE_IndicatorHeaderArrow, &subopt, p, widget);
            }

            p->setClipRegion(clipRegion);
        }
        break;
    }
    case CE_HeaderSection: {
        // Modern Qt6 style drawing instead of deprecated qDrawShadePanel
        p->save();
        p->setPen(Qt::NoPen);
        p->setBrush(opt->palette.button());
        p->drawRect(opt->rect);
        
        // Draw border with proper shading for Qt6
        QPen borderPen(opt->palette.mid().color());
        borderPen.setWidth(1);
        p->setPen(borderPen);
        
        if (opt->state & State_Sunken) {
            // Sunken appearance - dark on top/left, light on bottom/right
            p->setPen(opt->palette.dark().color());
            p->drawLine(opt->rect.topLeft(), opt->rect.topRight());
            p->drawLine(opt->rect.topLeft(), opt->rect.bottomLeft());
            p->setPen(opt->palette.light().color());
            p->drawLine(opt->rect.bottomLeft(), opt->rect.bottomRight());
            p->drawLine(opt->rect.topRight(), opt->rect.bottomRight());
        } else {
            // Raised appearance - light on top/left, dark on bottom/right
            p->setPen(opt->palette.light().color());
            p->drawLine(opt->rect.topLeft(), opt->rect.topRight());
            p->drawLine(opt->rect.topLeft(), opt->rect.bottomLeft());
            p->setPen(opt->palette.dark().color());
            p->drawLine(opt->rect.bottomLeft(), opt->rect.bottomRight());
            p->drawLine(opt->rect.topRight(), opt->rect.bottomRight());
        }
        p->restore();
        break;
    }
    case CE_HeaderLabel: {
        if (const QStyleOptionHeader *header = qstyleoption_cast<const QStyleOptionHeader *>(opt)) {
            QRect rect = header->rect;
            if (!header->icon.isNull()) {
                // int iconExtent = proxy()->pixelMetric(PM_LargeIconSize);
                QSize iconElemSize = header->icon.actualSize(iconsSize);
                QPixmap pixmap = header->icon.pixmap(qt_getWindow(widget), iconElemSize, (header->state & State_Enabled) ? QIcon::Normal : QIcon::Disabled);
                // Proper HiDPI handling for Qt6.8+
                qreal dpr = pixmap.devicePixelRatio();
                int pixw = qRound(pixmap.width() / dpr);
                QRect aligned = alignedRect(header->direction, QFlag(header->iconAlignment), pixmap.size() / pixmap.devicePixelRatio(), rect);
                QRect inter = aligned.intersected(rect);
                p->drawPixmap(inter.x(), inter.y(), pixmap,
                              inter.x() - aligned.x(), inter.y() - aligned.y(),
                              aligned.width() * pixmap.devicePixelRatio(),
                              pixmap.height() * pixmap.devicePixelRatio());
                const int margin = proxy()->pixelMetric(QStyle::PM_HeaderMargin, opt, widget);
                if (header->direction == Qt::LeftToRight) {
                    rect.setLeft(rect.left() + pixw + margin);
                } else {
                    rect.setRight(rect.right() - pixw - margin);
                }
            }
            if (header->state & QStyle::State_On) {
                QFont fnt = p->font();
                fnt.setBold(true);
                p->setFont(fnt);
            }
            proxy()->drawItemText(p, rect, header->textAlignment, header->palette,
                                  (header->state & State_Enabled), header->text, QPalette::ButtonText);
        }
        break;
    }
    case CE_HeaderEmptyArea: {
        // Modern Qt6 style for empty area
        p->save();
        p->fillRect(opt->rect, opt->palette.button());
        
        // Draw simple border
        p->setPen(opt->palette.mid().color());
        p->drawRect(opt->rect.adjusted(0, 0, -1, -1));
        p->restore();
        break;
    }
    default:
        QProxyStyle::drawControl(element, opt, p, widget);
    }
}

QRect QAdvancedHeaderStyle::subElementRect(SubElement sr, const QStyleOption *opt, const QWidget *widget) const
{
    QRect r;
    int margin = proxy()->pixelMetric(QStyle::PM_HeaderMargin, opt, widget);

    switch (sr) {
    case SE_HeaderLabel: {
        r.setRect(opt->rect.x() + margin, opt->rect.y() + margin,
                  opt->rect.width() - margin * 2, opt->rect.height() - margin * 2);
        if (const QStyleOptionHeader *header = qstyleoption_cast<const QStyleOptionHeader *>(opt)) {
            // Subtract width needed for arrow, if there is one
            if (header->sortIndicator != QStyleOptionHeader::None) {
                if (opt->state & State_Horizontal) {
                    r.setWidth(r.width() - itemsSize.width() - margin);
                } else {
                    r.setHeight(r.height() - itemsSize.height() - margin);
                }
            }
        }
        r = visualRect(opt->direction, opt->rect, r);
        break;
    }
    case SE_HeaderArrow: {
        int h = opt->rect.height();
        int w = opt->rect.width();
        int x = opt->rect.x();
        int y = opt->rect.y();
        if (opt->state & State_Horizontal) {
            /*int horiz_size = h / 2;
            r.setRect(x + w - margin * 2 - horiz_size, y + 5,
                      horiz_size, h - margin * 2 - 5);*/
            r.setRect(x + w - margin * 2 - itemsSize.width(),
                      y + h / 2 - itemsSize.height() / 2,
                      itemsSize.width(), itemsSize.height());
            /*r.setRect(x + w - margin - itemsSize.width(),
                      y + h - margin - itemsSize.height(),
                      itemsSize.width(), itemsSize.height());*/
        } else {
            int vert_size = w / 2;
            r.setRect(x + 5, y + h - margin * 2 - vert_size,
                      w - margin * 2 - 5, vert_size);
        }
        r = visualRect(opt->direction, opt->rect, r);
        break;
    }
    default:
        return QProxyStyle::subElementRect(sr, opt, widget);
    }
    return r;
}

QSize QAdvancedHeaderStyle::sizeFromContents(ContentsType ct, const QStyleOption *opt,
                                             const QSize &csz, const QWidget *widget) const
{
    QSize sz(csz);
    switch (ct) {
    case CT_HeaderSection: {
        if (const QStyleOptionHeader *hdr = qstyleoption_cast<const QStyleOptionHeader *>(opt)) {
            int margin = proxy()->pixelMetric(QStyle::PM_HeaderMargin, hdr, widget);
            // int iconSize = nullIcon ? 0 : proxy()->pixelMetric(QStyle::PM_SmallIconSize, hdr, widget);
            QSize iconSize = hdr->icon.isNull() ? QSize(0, 0) : iconsSize;

            QSize txt = hdr->fontMetrics.size(0, hdr->text);
            sz.setHeight(margin + qMax(iconSize.height(), txt.height()) + margin);
            sz.setWidth((hdr->text.isNull() ? 0 : margin) + txt.width() + (hdr->icon.isNull() ? 0 : margin) + iconSize.width() + margin);

            // int itemsHeight = margin;
            if (hdr->sortIndicator != QStyleOptionHeader::None) {
                // itemsHeight += itemsSize.height() + margin;
                if (hdr->orientation == Qt::Horizontal) {
                    sz.rwidth() += itemsSize.width() + margin;
                } else {
                    sz.rheight() += itemsSize.height() + margin;
                }
            }

            /*if (hdr->orientation == Qt::Horizontal) {
                sz.setHeight(qMax(sz.height(), itemsHeight));
            } else {
                sz.setWidth(qMax(sz.width(), itemsHeight));
            }*/
        }
        break;
    }
    default:
        return QProxyStyle::sizeFromContents(ct, opt, csz, widget);
    }
    return sz;
}
