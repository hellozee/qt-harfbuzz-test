#ifndef HBWRAPPER_H
#define HBWRAPPER_H

#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ot.h>
#include <QString>
#include <QRawFont>
#include <QGlyphRun>
#include <QPainter>
#include <QDebug>

struct PropertyHolder{
    QGlyphRun glyph;
    qreal margin, scale, displacement;
    QRawFont font;
};

class HBWrapper
{
public:
    HBWrapper() = default;
    ~HBWrapper();
    void setFont(QString location);
    void setText(QString text);
    PropertyHolder calculate();
private:
    hb_font_t *font;
    hb_buffer_t *buffer;
    QRawFont rawFont;
};

#endif // HBWRAPPER_H
