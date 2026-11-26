#include "pharaoh/ui/bitmap_font.h"

#include "pharaoh/runtime/text_loading.h"
#include "pharaoh/runtime/text_input_state.h"
#include "pharaoh/runtime/status_log_format.h"

#include <string.h>

/* Pinned EXE character-to-glyph data at 005E049C, bytes for codes 32..255.
   This is copied data, not an address used by the restored process. */
static const uint8_t glyph_ids[224] = {
    0x00, 0x3f, 0x40, 0x00, 0x00, 0x41, 0x00, 0x4a, 0x43, 0x44, 0x42, 0x46, 0x4e, 0x45, 0x4f, 0x4d,
    0x3e, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x48, 0x49, 0x00, 0x47, 0x00, 0x4b,
    0x00, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
    0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x00, 0x63, 0x00, 0x00, 0x50,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x65, 0x61, 0x56, 0x54, 0x51, 0x53, 0x01, 0x67, 0x81, 0x55, 0x57, 0x59, 0x6e, 0x5d, 0x69, 0x1b,
    0x6a, 0x67, 0x6d, 0x60, 0x5d, 0x5f, 0x64, 0x63, 0x19, 0x7b, 0x6b, 0x00, 0x6f, 0x00, 0x00, 0x00,
    0x52, 0x7f, 0x5e, 0x62, 0x66, 0x6c, 0x01, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0x72, 0x70, 0x71, 0x71, 0x69, 0x83, 0x6d, 0x65, 0x74, 0x6a, 0x73, 0x73, 0x77, 0x75, 0x76, 0x76,
    0x00, 0x6c, 0x7a, 0x78, 0x79, 0x79, 0x7b, 0x00, 0x84, 0x7e, 0x7c, 0x7d, 0x6b, 0x33, 0x00, 0x68,
    0x53, 0x52, 0x54, 0x51, 0x51, 0x85, 0x67, 0x65, 0x57, 0x56, 0x58, 0x55, 0x5b, 0x5a, 0x5c, 0x59,
    0x00, 0x66, 0x5f, 0x5e, 0x60, 0x60, 0x5d, 0x00, 0x86, 0x63, 0x62, 0x64, 0x61, 0x19, 0x00, 0x19
};

/* Printable startup-label metrics. The full native owner below also exposes
   the live glyph write, control-byte map and image0 fallback. */
int32_t pharaoh_bitmap_text_width(const PharaohBitmapFont *font,
    const uint8_t *text)
{
    int32_t width = 0;
    for (uint32_t i = 0; i < 10000 && text[i] != 0; ++i) {
        const uint8_t c = text[i];
        if (c == ' ') width += font->measure_space;
        else if (c >= 32 && glyph_ids[c - 32] != 0) {
            const uint32_t image = font->first_image + glyph_ids[c - 32] - 1u;
            if (image < font->image_count)
                width += font->images[image].width + font->measure_gap;
        }
    }
    return width;
}

/* Complete native004CBF30 over extracted font metrics and shared glyph state.
   Native font selection supplies space10/gap1 for groups640/641, space4/gap1
   for634, otherwise space6/gap0. The map is indexed by raw character bytes. */
int32_t pharaoh_bitmap_text_width_native(const PharaohBitmapFont *font,
    const uint8_t *text,const uint8_t glyph_map[256],uint32_t *current_glyph)
{
    uint32_t width=0;
    for (uint32_t i=0;i<10000 && text[i];++i) {
        const uint8_t c=text[i];
        if (c==' ') width+=(uint32_t)font->measure_space;
        else {
            const uint32_t glyph=glyph_map[c];*current_glyph=glyph;
            if (glyph) {
                int32_t image=(int32_t)(font->first_image+glyph-1u);
                if (image>=(int32_t)font->image_count) image=0;
                width+=(uint32_t)(int32_t)font->images[image].width+
                    (uint32_t)font->measure_gap;
            }
        }
    }
    return (int32_t)width;
}

/* Startup metrics adapter for004CC6C0. Full glyph bookkeeping is in the native owner below. The selected text
 * remains current, matching the native write to 0x00E20768. */
int32_t pharaoh_bitmap_text_entry_width(const PharaohBitmapFont *font,
    PharaohTextSelection *selection, int32_t group, int32_t entry)
{
    pharaoh_text_select(selection, group, entry);
    return pharaoh_bitmap_text_width(font,
        (const uint8_t *)selection->current);
}

/* Native004CC6C0 inlines004CC110 then calls004CBF30. */
int32_t pharaoh_bitmap_text_entry_width_native(const PharaohBitmapFont *font,
    PharaohTextSelection *selection,int32_t group,int32_t entry,
    const uint8_t glyph_map[256],uint32_t *current_glyph)
{
    pharaoh_text_select(selection,group,entry);
    return pharaoh_bitmap_text_width_native(font,
        (const uint8_t *)selection->current,glyph_map,current_glyph);
}

int32_t pharaoh_bitmap_text_draw_advance(const PharaohBitmapFont *font,
    const uint8_t *text)
{
    int32_t width = 0;
    for (uint32_t i = 0; i < 10000 && text[i] != 0; ++i) {
        uint8_t c = text[i];
        if (c == '#' || c == '_') c = ' ';
        if (c < 32) continue;
        const uint8_t glyph = glyph_ids[c - 32];
        if (glyph == 0) {
            width += font->draw_space;
        } else {
            const uint32_t image = font->first_image + glyph - 1u;
            if (image < font->image_count) {
                width += font->images[image].width + font->draw_gap;
            }
        }
    }
    return width;
}

/* Complete typed C translation of Pharaoh.exe 0x004CCA60. The native glyph
   table is indexed by (source byte - 32); invalid metric indices use image 0,
   while the original computed image ID is still passed to the draw operation. */
int32_t pharaoh_bitmap_font_draw_glyph(const PharaohBitmapFont *font,
    uint8_t character_index, int32_t x, int32_t y, uint16_t color,
    uint32_t *current_glyph)
{
    uint32_t glyph;
    uint32_t image;
    uint32_t metric_image;
    int32_t accent;

    if (character_index >= sizeof(glyph_ids)) {
        *current_glyph = 0;
        return 0;
    }
    glyph = glyph_ids[character_index];
    *current_glyph = glyph;
    if (glyph == 0) return 0;

    image = font->first_image + glyph - 1u;
    metric_image = image;
    if (metric_image >= font->image_count) metric_image = 0;
    if (font->image_count == 0) return 0;

    accent = (int32_t)font->images[metric_image].height - font->baseline;
    if (accent < 0 || character_index < 0x60 ||
        character_index == 199 || character_index == 231) {
        accent = 0;
    }
    font->draw_glyph(font->user_data, image, x, y - 3 - accent, color);
    return font->images[metric_image].width;
}

/* Complete typed translation of Pharaoh.exe 0x004CC770. Mutable globals are
   carried explicitly so startup labels and interactive text share the native
   algorithm without hidden process-wide state. */
void pharaoh_bitmap_text_draw_native(const PharaohBitmapFont *font,
    PharaohBitmapTextDrawState *state, const uint8_t *text,
    int32_t x, int32_t y, uint16_t color)
{
    PharaohBitmapFont draw_font = *font;
    int32_t space, spacing = 0;
    uint8_t character;

    if (state->font_id == state->font_id_large_a) {
        space = 8; state->baseline = 23;
    } else if (state->font_id == state->font_id_large_b) {
        space = 8; state->baseline = 24;
    } else if (state->font_id == state->font_id_small) {
        space = 4; spacing = 1; state->baseline = 9;
    } else {
        space = 6; state->baseline = 11;
    }
    draw_font.draw_space = space;
    draw_font.draw_gap = spacing;
    draw_font.baseline = state->baseline;
    state->color = color;
    character = *text;
    for (;;) {
        const uint8_t previous = character;
        uint8_t draw_character = character;
        int32_t advance = 0;
        if (character == 0) {
            if (!state->anchor_captured) {
                state->anchor_advance = 4;
                state->anchor_captured = 1;
                state->anchor_x = state->cursor_x;
                state->anchor_y = state->cursor_y;
            }
            state->hash_mode = 0;
            state->cursor_x += space;
            state->encoded_character_index = 0;
            state->duplicate_space = 0;
            state->reset_flag = 0;
            return;
        }
        if (character == '#') {
            draw_character = state->hash_mode == 1 ?
                pharaoh_text_next_character((const char *)text,
                    &state->encoded_character_index) : (uint8_t)' ';
        }
        if (draw_character == '_') draw_character = ' ';
        if (draw_character > 31) {
            const uint8_t glyph = (uint8_t)(draw_character - 32);
            advance = space;
            if (glyph_ids[glyph] != 0) {
                if (!state->outline && state->vertical_shadow) {
                    state->color = 0;
                    (void)pharaoh_bitmap_font_draw_glyph(&draw_font, glyph,
                        x, y - 1, state->color, state->current_glyph);
                    state->color = 0;
                    (void)pharaoh_bitmap_font_draw_glyph(&draw_font, glyph,
                        x, y + 1, state->color, state->current_glyph);
                } else if (!state->outline && state->diagonal_shadow) {
                    state->color = 0;
                    (void)pharaoh_bitmap_font_draw_glyph(&draw_font, glyph,
                        x + 1, y + 1, state->color, state->current_glyph);
                    state->color = color;
                    advance = pharaoh_bitmap_font_draw_glyph(&draw_font,
                        glyph, x - 1, y, state->color, state->current_glyph) + spacing;
                    goto positioned;
                }
                state->color = color;
                advance = pharaoh_bitmap_font_draw_glyph(&draw_font, glyph,
                    x, y, state->color, state->current_glyph) + spacing;
            }
positioned:
            if (state->character_index ==
                    (uint32_t)state->cursor_limits[state->cursor_slot] &&
                !state->anchor_captured) {
                state->anchor_x = state->cursor_x;
                state->anchor_y = state->cursor_y;
                state->anchor_captured = 1;
                state->anchor_advance = advance;
            }
            x += advance;
            state->cursor_x += advance;
        }
        if (state->encoded_character_index == 0) {
            ++text;
            character = *text;
        } else {
            character = pharaoh_text_next_character((const char *)text,
                &state->encoded_character_index);
        }
        ++state->character_index;
        if (!state->duplicate_space && state->previous_space &&
            previous == character && (character == ' ' || character == '_')) {
            state->cursor_x -= advance;
            x -= advance;
        }
    }
}

/* Complete typed C translation of native 0x004CCBB0. Its inline catalog
   traversal is the already mapped 0x004CC110 selector. */
void pharaoh_bitmap_text_draw_selection_native(
    const PharaohBitmapFont *font, PharaohBitmapTextDrawState *state,
    PharaohTextSelection *selection, int32_t group, int32_t entry,
    int32_t x, int32_t y, int32_t font_id, uint16_t color)
{
    pharaoh_text_select(selection, group, entry);
    state->font_id = (uint32_t)font_id;
    pharaoh_bitmap_text_draw_native(font, state,
        (const uint8_t *)selection->current, x, y, color);
    state->reset_flag = 0;
}

/* Complete typed C translation of native 0x004CCE00. The 256-byte scratch
   buffer and formatter are the same explicit storage/owner used by 0x004CC5C0. */
void pharaoh_bitmap_text_draw_number_native(
    const PharaohBitmapFont *font, PharaohBitmapTextDrawState *state,
    char scratch_text[256], int32_t value, char prefix,
    const char *suffix, int32_t x, int32_t y, int32_t font_id,
    uint16_t color)
{
    int32_t index = 0;
    (void)pharaoh_status_log_integer_to_text(scratch_text, value, 1, 0);

    char current = scratch_text[0];
    if (prefix != 0) {
        scratch_text[0] = prefix;
        current = prefix;
    }
    while (current != 0) {
        current = scratch_text[index + 1];
        ++index;
    }
    current = *suffix;
    while (current != 0) {
        scratch_text[index] = current;
        ++index;
        ++suffix;
        current = *suffix;
    }
    scratch_text[index] = 0;
    state->font_id = (uint32_t)font_id;
    pharaoh_bitmap_text_draw_native(font, state,
        (const uint8_t *)scratch_text, x, y, color);
    state->reset_flag = 0;
}

void pharaoh_bitmap_text_draw_at(const PharaohBitmapFont *font,
    const uint8_t *text, int32_t x, int32_t y, uint16_t color)
{
    for (uint32_t i = 0; i < 10000 && text[i] != 0; ++i) {
        uint8_t c = text[i];
        /* Startup labels use native hash mode zero, where # and _ are spaces. */
        if (c == '#' || c == '_') c = ' ';
        if (c < 32) continue;
        const uint8_t glyph = glyph_ids[c - 32];
        if (glyph == 0) { x += font->draw_space; continue; }
        uint32_t unused_current_glyph;
        uint32_t *const current_glyph = font->current_glyph != NULL ?
            font->current_glyph : &unused_current_glyph;
        const int32_t width = pharaoh_bitmap_font_draw_glyph(font,
            (uint8_t)(c - 32), x, y, color, current_glyph);
        x += width + font->draw_gap;
    }
}

void pharaoh_bitmap_text_draw_centered(const PharaohBitmapFont *font,
    const uint8_t *text, int32_t x, int32_t y, int32_t width, uint16_t color)
{
    const int32_t measured = pharaoh_bitmap_text_width(font, text);
    if (width > measured) x += (width - measured) / 2;
    pharaoh_bitmap_text_draw_at(font, text, x, y, color);
}

void pharaoh_bitmap_text_draw_centered_native(
    const PharaohBitmapTextCenteredNativeContext *context,
    const uint8_t *text, int32_t x, int32_t y, int32_t width,
    int32_t font_id, uint16_t color)
{
    int32_t offset = (width - pharaoh_bitmap_text_width_native(
        context->font, text, context->glyph_map,
        context->draw_state->current_glyph)) / 2;
    if (offset < 0) offset = 0;
    *context->center_offset_00e20770 = offset;
    context->draw_state->font_id = (uint32_t)font_id;
    pharaoh_bitmap_text_draw_native(context->font, context->draw_state,
        text, x + offset, y, color);
}

void pharaoh_bitmap_text_draw_catalog_centered_native(
    const PharaohBitmapTextCatalogCenteredNativeContext *context,
    int32_t group, int32_t entry, int32_t x, int32_t y, int32_t width,
    int32_t font_id, uint16_t color)
{
    pharaoh_text_select(context->selection, group, entry);
    *context->current_text_00e20768 = context->selection->current;
    pharaoh_bitmap_text_draw_centered_native(context->drawing,
        (const uint8_t *)*context->current_text_00e20768, x, y, width,
        font_id, color);
    *context->reset_flag_00d3b504 = 0;
}

void pharaoh_bitmap_text_draw_catalog_wrapped_native(
    const PharaohBitmapTextCatalogWrappedNativeContext *context,
    int32_t group, int32_t entry, int32_t x, int32_t y, int32_t width,
    uint16_t color)
{
    pharaoh_text_select(context->selection, group, entry);
    *context->current_text_00e20768 = context->selection->current;
    pharaoh_bitmap_text_draw_wrapped_native(context->drawing,
        (const uint8_t *)*context->current_text_00e20768,
        x, y, width, color);
}

int32_t pharaoh_bitmap_wrapped_word_width_native(PharaohRichTextState *metrics,
    const uint8_t *text, uint32_t font_id)
{
    int32_t width = 0;
    int32_t has_glyph = 0;
    metrics->consumed_bytes = 0;
    for (int32_t count = 0; count < 1999; ++count) {
        const uint8_t character = *text++;
        if (character == 0) return width;
        if (character == ' ') {
            if (has_glyph) return width;
            width += 4;
        } else if (character == '$') {
            if (has_glyph) return width;
        } else if (character > 31) {
            width += pharaoh_rich_text_character_width(metrics,
                character, font_id);
            has_glyph = 1;
        }
        ++metrics->consumed_bytes;
    }
    return width;
}

void pharaoh_bitmap_text_draw_wrapped_native(
    const PharaohBitmapWrappedNativeContext *context, const uint8_t *text,
    int32_t x, int32_t y, int32_t width, uint16_t color)
{
    PharaohBitmapTextDrawState *const state = context->draw_state;
    int32_t has_more = 1;
    for (int32_t line = 1; line < 100; ++line) {
        int32_t line_width = 0;
        int32_t line_bytes = 0;
        memset(context->line_buffer_00e3439c, 0, 200);
        if (has_more) {
            for (;;) {
                if (line_width >= width) break;
                line_width += pharaoh_bitmap_wrapped_word_width_native(
                    context->word_metrics, text, context->font_id);
                if (line_width >= width) {
                    if (line_width == 0) has_more = 0;
                    break;
                }
                for (int32_t byte = context->word_metrics->consumed_bytes;
                        byte > 0; --byte) {
                    context->line_buffer_00e3439c[line_bytes++] = *text++;
                }
                if (*text == 0) {
                    has_more = 0;
                    break;
                }
            }
        }
        state->hash_mode = 1;
        state->cursor_x = 0;
        state->duplicate_space = 1;
        state->font_id = context->font_id;
        pharaoh_bitmap_text_draw_native(context->font, state,
            context->line_buffer_00e3439c, x, y, color);
        if (*context->maximum_width_00e2076c < state->cursor_x) {
            *context->maximum_width_00e2076c = state->cursor_x;
        }
        y += 5 + state->baseline;
        state->cursor_y += 5 + state->baseline;
        if (!has_more) return;
    }
}

int32_t pharaoh_bitmap_wrapped_word_width(const PharaohBitmapFont *font,
    const uint8_t *text, int32_t *byte_count)
{
    int32_t width = 0;
    int32_t count = 0;
    int32_t has_glyph = 0;

    while (count < 1999) {
        const uint8_t c = text[count];
        if (c == 0) break;
        if (c == ' ') {
            if (has_glyph != 0) break;
            width += 4;
        } else if (c != '$' && c >= 32) {
            const uint8_t glyph = glyph_ids[c - 32];
            if (glyph != 0) {
                const uint32_t image = font->first_image + glyph - 1u;
                if (image < font->image_count) {
                    width += font->images[image].width + 1;
                }
            }
            has_glyph = 1;
        }
        ++count;
    }
    *byte_count = count;
    return width;
}

void pharaoh_bitmap_text_draw_wrapped(const PharaohBitmapFont *font,
    const uint8_t *text, int32_t x, int32_t y, int32_t width,
    uint16_t color)
{
    const uint8_t *cursor = text;
    int32_t has_more = 1;

    for (int32_t line_index = 0; line_index < 100; ++line_index) {
        uint8_t line[200] = { 0 };
        int32_t line_bytes = 0;
        int32_t line_width = 0;

        if (has_more != 0) {
            do {
                int32_t word_bytes;
                int32_t word_width;
                if (line_width >= width) break;
                word_width = pharaoh_bitmap_wrapped_word_width(font,
                    cursor, &word_bytes);
                line_width += word_width;
                if (line_width >= width) break;
                if (word_bytes > 0) {
                    int32_t copy_bytes = word_bytes;
                    if (copy_bytes > (int32_t)sizeof(line) - 1 - line_bytes) {
                        copy_bytes = (int32_t)sizeof(line) - 1 - line_bytes;
                    }
                    memcpy(line + line_bytes, cursor, (size_t)copy_bytes);
                    line_bytes += copy_bytes;
                    cursor += word_bytes;
                }
                if (*cursor == 0) {
                    has_more = 0;
                    break;
                }
            } while (line_width != 0);
        }
        pharaoh_bitmap_text_draw_at(font, line, x, y, color);
        y += font->baseline + 5;
        if (has_more == 0) return;
    }
}
