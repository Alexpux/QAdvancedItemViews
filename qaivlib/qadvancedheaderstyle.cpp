#include "qadvancedheaderstyle.h"

#include "qadvancedconstants.h"

#include <QPainter>
#include <QPixmapCache>
#include <QStyleOptionHeader>
#include <qdrawutil.h>

static QWindow *qt_getWindow(const QWidget *widget)
{
    return widget ? widget->window()->windowHandle() : nullptr;
}

static QPixmap cachedPixmap(const QString &img, const QSize &pixSize)
{
    QPixmap pm;
    if (!QPixmapCache::find(img, &pm)) {
        pm = QIcon(img).pixmap(pixSize);
        // pm = QPixmap::fromImage(QImage(img), Qt::OrderedDither | Qt::OrderedAlphaDither);
        // pm = pm.scaled(HeaderIconSize);

        QPixmapCache::insert(img, pm);
    }
    return pm;
}

QAdvancedHeaderStyle::QAdvancedHeaderStyle(QStyle *style) :
    QProxyStyle(style),
    itemsSize(QSize(16, 16)),
    iconsSize(QSize(32, 32))
{
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
        qDrawShadePanel(p, opt->rect, opt->palette,
                        opt->state & State_Sunken, 1,
                        &opt->palette.brush(QPalette::Button));

        /*QBrush fill;
        if (opt->state & State_On)
            fill = QBrush(opt->palette.light().color(), Qt::Dense4Pattern);
        else
            fill = opt->palette.brush(QPalette::Button);

        if (opt->state & (State_Raised | State_Sunken)) {
            qDrawWinButton(p, opt->rect, opt->palette, opt->state & State_Sunken, &fill);
        } else {
            p->fillRect(opt->rect, fill);
        }*/
        break;
    }
    case CE_HeaderLabel: {
        if (const QStyleOptionHeader *header = qstyleoption_cast<const QStyleOptionHeader *>(opt)) {
            QRect rect = header->rect;
            if (!header->icon.isNull()) {
                // int iconExtent = proxy()->pixelMetric(PM_LargeIconSize);
                QSize iconElemSize = header->icon.actualSize(iconsSize);
                QPixmap pixmap = header->icon.pixmap(qt_getWindow(widget), iconElemSize, (header->state & State_Enabled) ? QIcon::Normal : QIcon::Disabled);
                // QPixmap pixmap = header->icon.pixmap(qt_getWindow(widget), iconElemSize);
                int pixw = pixmap.width() / pixmap.devicePixelRatio();
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
        qDrawShadePanel(p, opt->rect, opt->palette,
                        opt->state & State_Sunken, 1,
                        &opt->palette.brush(QPalette::Button));
        break;
        // p->fillRect(opt->rect, opt->palette.window());
        // break;
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
