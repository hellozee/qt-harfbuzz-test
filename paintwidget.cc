#include "paintwidget.h"
#include <QPainter>

PaintWidget::PaintWidget(QWidget *parent):
    QWidget(parent), propertiesSet(false)
{ }

void PaintWidget::paintEvent(QPaintEvent *e)
{
    if(!propertiesSet){
        QWidget::paintEvent(e);
        return;
    }

    QPainter painter(this);
    painter.drawGlyphRun(QPointF(margin, font.ascent() + margin), glyph);
    resize(displacement/scale + margin*2, font.ascent() + font.descent() + margin*2);
}

void PaintWidget::setProperties(PropertyHolder p)
{
    glyph = p.glyph;
    margin = p.margin;
    scale = p.scale;
    font = p.font;
    displacement = p.displacement;
    propertiesSet = true;
}
