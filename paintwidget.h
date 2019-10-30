#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include "hbwrapper.h"

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    PaintWidget(QWidget *parent=nullptr);

    void paintEvent(QPaintEvent *e) override;
    void setProperties(PropertyHolder p);

private:
    QGlyphRun glyph;
    qreal margin, scale, displacement;
    QRawFont font;
    bool propertiesSet;
};

#endif // PAINTWIDGET_H
