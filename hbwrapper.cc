#define FONT_SIZE 36
#define MARGIN (FONT_SIZE * .5)
#define FONT_SCALE 64

#include "hbwrapper.h"

HBWrapper::~HBWrapper()
{
    hb_buffer_destroy(buffer);
    hb_font_destroy(font);
}

void HBWrapper::setFont(QString location)
{
    hb_blob_t *blob = hb_blob_create_from_file(location.toStdString().c_str());
    hb_face_t *face = hb_face_create(blob, 0);
    hb_blob_destroy(blob);

    font = hb_font_create(face);
    hb_ot_font_set_funcs(font);
    hb_font_set_scale(font, FONT_SIZE*FONT_SCALE, FONT_SIZE*FONT_SCALE);

    QRawFont rawFont(location, FONT_SIZE);
    qDebug() << location;
}

void HBWrapper::setText(QString text)
{
    buffer = hb_buffer_create ();
    hb_buffer_add_utf8 (buffer, text.toStdString().c_str(), -1, 0, -1);
    hb_buffer_guess_segment_properties (buffer);
    hb_shape (font, buffer, nullptr, 0);
}

PropertyHolder HBWrapper::calculate()
{
    unsigned int nGlyphs = hb_buffer_get_length(buffer);
    hb_glyph_info_t *hbGlyphs = hb_buffer_get_glyph_infos(buffer, nullptr);
    hb_glyph_position_t *hbPositions = hb_buffer_get_glyph_positions(buffer, nullptr);

    QVector<quint32> glyphIndexes(static_cast<int>(nGlyphs));
    QVector<QPointF> glyphPositions(static_cast<int>(nGlyphs));

    qreal x = 0.0, y = 0.0;
    for (int i = 0; i < static_cast<int>(nGlyphs); i++, hbGlyphs++, hbPositions++) {
        glyphIndexes[i] = hbGlyphs->codepoint;
        glyphPositions[i] = QPointF(x + hbPositions->x_offset, y - hbPositions->y_offset) / FONT_SCALE;
        x += hbPositions->x_advance;
        y -= hbPositions->y_advance;
    }

    QGlyphRun glyphRun = QGlyphRun();

    glyphRun.setRawFont(rawFont);
    glyphRun.setGlyphIndexes(glyphIndexes);
    glyphRun.setPositions(glyphPositions);

    PropertyHolder p;
    p.font = rawFont;
    p.glyph = glyphRun;
    p.scale = FONT_SCALE;
    p.margin = MARGIN;
    p.displacement = x;

    return p;
}
