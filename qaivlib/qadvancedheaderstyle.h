#ifndef QADVANCEDHEADERSTYLE_H
#define QADVANCEDHEADERSTYLE_H

#include <QCommonStyle>
#include <QPixmap>
#include <QProxyStyle>

class QAdvancedHeaderStyle : public QProxyStyle {
    Q_OBJECT

public:
    enum QControlElement {
        // CE_CustomBase = 0xf0000000
        CE_HeaderFilterButton = CE_CustomBase + 1,
        CE_HeaderLook
    };

    explicit QAdvancedHeaderStyle(QStyle *style = nullptr);

    void drawControl(ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *widget = nullptr) const override;
    QRect subElementRect(SubElement sr, const QStyleOption *opt, const QWidget *widget = nullptr) const override;

    QSize sizeFromContents(ContentsType ct, const QStyleOption *opt, const QSize &csz, const QWidget *widget) const override;

protected:
    virtual ~QAdvancedHeaderStyle();

    virtual QPixmap cached(const QString &img, const QSize &pixSize) const;
    void clearCache();

    Q_DISABLE_COPY(QAdvancedHeaderStyle)

private:
    QSize itemsSize;
    QSize iconsSize;
};

#endif // QADVANCEDHEADERSTYLE_H
