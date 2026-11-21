#include "pharaoh/ui/rich_text.h"

#include <string.h>

/* Pinned EXE table at 0x005E049C, indexed by character - 32. */
static const uint8_t glyph_ids[224] = {
    0x00,0x3f,0x40,0x00,0x00,0x41,0x00,0x4a,0x43,0x44,0x42,0x46,0x4e,0x45,0x4f,0x4d,
    0x3e,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x48,0x49,0x00,0x47,0x00,0x4b,
    0x00,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,
    0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x00,0x63,0x00,0x00,0x50,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x00,0x00,0x00,0x00,0x00,
    0x65,0x61,0x56,0x54,0x51,0x53,0x01,0x67,0x81,0x55,0x57,0x59,0x6e,0x5d,0x69,0x1b,
    0x6a,0x67,0x6d,0x60,0x5d,0x5f,0x64,0x63,0x19,0x7b,0x6b,0x00,0x6f,0x00,0x00,0x00,
    0x52,0x7f,0x5e,0x62,0x66,0x6c,0x01,0x0f,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x82,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
    0x72,0x70,0x71,0x71,0x69,0x83,0x6d,0x65,0x74,0x6a,0x73,0x73,0x77,0x75,0x76,0x76,
    0x00,0x6c,0x7a,0x78,0x79,0x79,0x7b,0x00,0x84,0x7e,0x7c,0x7d,0x6b,0x33,0x00,0x68,
    0x53,0x52,0x54,0x51,0x51,0x85,0x67,0x65,0x57,0x56,0x58,0x55,0x5b,0x5a,0x5c,0x59,
    0x00,0x66,0x5f,0x5e,0x60,0x60,0x5d,0x00,0x86,0x63,0x62,0x64,0x61,0x19,0x00,0x19
};

static const PharaohSpriteImageRecord *image_record(
    const PharaohRichTextState *state, uint32_t image)
{
    if (image >= state->image_count) image = 0;
    return &state->images[image];
}

int32_t pharaoh_parse_decimal(const uint8_t *text)
{
    const int32_t negative = *text == '-';
    uint32_t value = 0;
    if (negative) ++text;
    while (*text >= '0' && *text <= '9') {
        value = value * 10u + (uint32_t)(*text - '0');
        ++text;
    }
    if (negative) value = 0u - value;
    return (int32_t)value;
}

void pharaoh_rich_text_reset_links(PharaohRichTextState *state)
{
    memset(state->links, 0, sizeof(state->links));
    state->link_count = 0;
    state->total_link_count = 0;
}

static void add_link(PharaohRichTextState *state, int32_t id,
    int32_t left, int32_t right, int32_t y)
{
    if (state->total_link_count >= PHARAOH_RICH_TEXT_LINK_LIMIT) return;
    PharaohRichTextLink *const link = &state->links[state->link_count];
    link->left = left - 2;
    link->right = right + 2;
    link->top = y - 1;
    link->bottom = y + 13;
    link->id = id;
    ++state->link_count;
    ++state->total_link_count;
}

int32_t pharaoh_rich_text_character_width(PharaohRichTextState *state,
    uint8_t character, uint32_t font)
{
    if (character == 0) return 0;
    if (character == ' ') return 4;
    if (character < 32) return 0;
    const uint8_t glyph = glyph_ids[character - 32];
    if (glyph == 0) return 0;
    return image_record(state, font + glyph - 1u)->width + 1;
}

int32_t pharaoh_rich_text_word_width(PharaohRichTextState *state,
    const uint8_t *text)
{
    const uint8_t *p = text;
    int32_t width = 0;
    int32_t iterations = 1;
    uint8_t nonspace = 0;
    state->consumed_bytes = 0;
    do {
        uint8_t c = *p++;
        if (c == 0) return width;
        if (c == '@') {
            c = *p++;
            if (!nonspace) {
                if (c == 'P' || c == 'L') {
                    state->consumed_bytes += 2;
                    return 0;
                }
                if (c == 'G') {
                    state->consumed_bytes += 2;
                    c = *p++;
                    while (c >= '0' && c <= '9') {
                        ++state->consumed_bytes;
                        c = *p++;
                    }
                    return 0;
                }
                do {
                    ++state->consumed_bytes;
                    if (c < '0' || c > '9') break;
                    c = *p++;
                } while (1);
            }
        }
        if (c == ' ') {
            if (nonspace) return width;
            width += 4;
        } else if (c > ' ') {
            width += pharaoh_rich_text_character_width(state, c,
                state->normal_font);
            nonspace = 1;
        }
        ++state->consumed_bytes;
        ++iterations;
    } while (iterations < 2000);
    return width;
}

static int32_t draw_glyph(PharaohRichTextState *state,
    const PharaohRichTextOperations *operations, uint32_t font,
    uint8_t glyph_index, int32_t x, int32_t y, int32_t calculate,
    int32_t flags, uint16_t color)
{
    const uint8_t glyph = glyph_ids[glyph_index];
    if (glyph == 0) return 0;
    const uint32_t image = font + glyph - 1u;
    const PharaohSpriteImageRecord *const record = image_record(state, image);
    int32_t offset = record->height - (flags ? 16 : 11);
    if (offset < 0 || glyph_index < 96 || glyph_index == 199 ||
        glyph_index == 231) offset = 0;
    if (!calculate && operations->draw_glyph != 0)
        operations->draw_glyph(operations->user_data, image, x,
            y - offset, color);
    return image_record(state, image)->width;
}

static void draw_line(PharaohRichTextState *state,
    const PharaohRichTextOperations *operations, const uint8_t *text,
    int32_t x, int32_t y, int32_t calculate, int32_t flags,
    uint16_t color)
{
    const uint8_t *p = text;
    int32_t highlighted = 0;
    const int32_t start_x = x;
    uint8_t c = *p++;
    while (c != 0) {
        if (c == '@') {
            const int32_t link = pharaoh_parse_decimal(p);
            c = *p++;
            while (c >= '0' && c <= '9') c = *p++;
            const int32_t width = pharaoh_rich_text_word_width(state, p - 1);
            highlighted = state->consumed_bytes + 1;
            add_link(state, link, x, x + width, y);
        }
        if (c >= ' ') {
            uint32_t font = state->normal_font;
            if (highlighted > 0) {
                --highlighted;
                font = state->link_font;
            }
            const uint8_t glyph_index = (uint8_t)(c - 32);
            int32_t advance;
            if (glyph_ids[glyph_index] == 0) advance = 6;
            else {
                advance = draw_glyph(state, operations, font, glyph_index,
                    x, y, calculate, flags, color);
                if (font == state->large_font) advance += 2;
            }
            x += advance;
        }
        c = *p++;
    }
    state->current_line_width = x - start_x;
}

void pharaoh_rich_text_draw(PharaohRichTextState *state,
    const PharaohRichTextOperations *operations, const uint8_t *text,
    int32_t x, int32_t y, int32_t width, int32_t calculate,
    int32_t flags)
{
    const uint8_t *p = text;
    int32_t remaining = 0, image = 0, delay = 0, line = 0;
    int32_t iterations = 0;
    uint8_t more = 1, indent = 0;
    if (calculate == 1) state->line_count = 0;
    for (;;) {
        uint8_t buffer[200] = { 0 };
        int32_t length = 0;
        uint8_t trim = 1, skip = 0;
        if ((!more && !remaining) || ++iterations >= 1000) return;
        int32_t used = indent ? 50 : 0;
        const int32_t x_indent = used;
        indent = 0;
        while ((more || remaining) && used < width) {
            if (remaining) {
                --remaining;
                break;
            }
            used += pharaoh_rich_text_word_width(state, p);
            if (used >= width) break;
            const int32_t consumed = state->consumed_bytes;
            for (int32_t n = 0; n < consumed; ++n) {
                uint8_t c = *p++;
                if (c == '@') {
                    if (*p == 'P') {
                        indent = 1; ++p; used = width; break;
                    }
                    if (*p == 'L') {
                        ++p; used = width; break;
                    }
                    if (*p == 'G') {
                        if (length && calculate == 1) ++state->line_count;
                        ++p;
                        image = (int32_t)state->inline_image_first +
                            pharaoh_parse_decimal(p) - 1;
                        c = *p++;
                        while (c >= '0' && c <= '9') c = *p++;
                        remaining = image_record(state,
                            (uint32_t)image)->height / 16 + 2;
                        used = width;
                        if (line > 0) delay = 1;
                        break;
                    }
                }
                if (!trim || c != ' ') {
                    trim = 0;
                    if (length < (int32_t)sizeof(buffer) - 1)
                        buffer[length++] = c;
                }
            }
            if (*p == 0) more = 0;
        }
        state->current_line_width = 0;
        if (!calculate)
            skip = line < state->scroll_position ||
                line >= state->viewport_lines + state->scroll_position;
        if (calculate || !skip) {
            draw_line(state, operations, buffer, x + x_indent, y,
                calculate, flags, state->color);
            if (state->normal_font == state->large_font) {
                const uint16_t overlay = state->color_variant == 2 ?
                    0xff3a : 0x7f9a;
                draw_line(state, operations, buffer, x + x_indent - 1,
                    y - 1, calculate, flags, overlay);
            }
        }
        if (!calculate && image != 0) {
            if (delay) --delay;
            else {
                const PharaohSpriteImageRecord *const record =
                    image_record(state, (uint32_t)image);
                remaining = record->height / 16 + 2;
                if (line < state->viewport_lines + state->scroll_position &&
                    operations->draw_sprite != 0) {
                    int32_t sprite_y = y + 8;
                    if (line < state->scroll_position)
                        sprite_y += (line - state->scroll_position) * 16;
                    const int32_t center =
                        state->text_width_blocks * 16 - record->width - 4;
                    operations->draw_sprite(operations->user_data,
                        (uint32_t)image, x + center / 2 - 4, sprite_y);
                }
                image = 0;
            }
        }
        if (state->current_line_width > state->maximum_line_width)
            state->maximum_line_width = state->current_line_width;
        ++line;
        if (calculate == 1) ++state->line_count;
        if (!skip) {
            y += 16;
            state->current_y_advance += 16;
        }
    }
}
