#define FONT_SIZE 36
#define MARGIN (FONT_SIZE * .5)
#define FONT_SCALE 64

#include "hbwrapper.h"

HBWrapper::~HBWrapper()
{
    hb_buffer_destroy(m_buffer);
    hb_font_destroy(m_font);
}

void HBWrapper::setFont(QString location)
{
    hb_blob_t *blob = hb_blob_create_from_file(location.toStdString().c_str());
    hb_face_t *face = hb_face_create(blob, 0);
    hb_blob_destroy(blob);

    m_font = hb_font_create(face);
    hb_ot_font_set_funcs(m_font);
    hb_font_set_scale(m_font, FONT_SIZE*FONT_SCALE, FONT_SIZE*FONT_SCALE);

    m_rawFont =  QRawFont(location, FONT_SIZE);
}

void HBWrapper::setText(QString text)
{
    m_buffer = hb_buffer_create ();
    hb_buffer_add_utf8 (m_buffer, text.toUtf8().data(), text.toUtf8().size(), 0, -1);
    hb_buffer_guess_segment_properties (m_buffer);
    hb_shape (m_font, m_buffer, nullptr, 0);
}

PropertyHolder HBWrapper::calculate()
{
    unsigned int nGlyphs = hb_buffer_get_length(m_buffer);
    hb_glyph_info_t *hbGlyphs = hb_buffer_get_glyph_infos(m_buffer, nullptr);
    hb_glyph_position_t *hbPositions = hb_buffer_get_glyph_positions(m_buffer, nullptr);

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

    Q_ASSERT(m_rawFont.isValid());

    glyphRun.setRawFont(m_rawFont);
    glyphRun.setGlyphIndexes(glyphIndexes);
    glyphRun.setPositions(glyphPositions);

    PropertyHolder p;
    p.font = m_rawFont;
    p.glyph = glyphRun;
    p.scale = FONT_SCALE;
    p.margin = MARGIN;
    p.displacement = x;

    return p;
}
