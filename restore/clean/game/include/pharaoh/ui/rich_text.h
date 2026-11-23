#ifndef PHARAOH_UI_RICH_TEXT_H
#define PHARAOH_UI_RICH_TEXT_H

#include "pharaoh/assets/sprite_libraries.h"

#include <stdint.h>

enum { PHARAOH_RICH_TEXT_LINK_LIMIT = 50 };

typedef struct PharaohRichTextLink {
    int32_t left;
    int32_t right;
    int32_t top;
    int32_t bottom;
    int32_t id;
} PharaohRichTextLink;

typedef struct PharaohRichTextOperations {
    void *user_data;
    void (*draw_sprite)(void *user_data, uint32_t image, int32_t x,
        int32_t y);
    void (*draw_glyph)(void *user_data, uint32_t image, int32_t x,
        int32_t y, uint16_t color);
} PharaohRichTextOperations;

typedef struct PharaohRichTextState {
    const PharaohSpriteImageRecord *images;
    uint32_t image_count;
    uint32_t normal_font;
    uint32_t link_font;
    uint32_t large_font;
    uint32_t inline_image_first;
    int32_t text_width_blocks;
    int32_t scroll_position;
    int32_t viewport_lines;
    int32_t line_count;
    int32_t consumed_bytes;
    int32_t current_line_width; /* native E20778 live renderer-state alias */
    int32_t maximum_line_width;
    int32_t current_y_advance;
    uint16_t color;
    int32_t color_variant;
    PharaohRichTextLink links[PHARAOH_RICH_TEXT_LINK_LIMIT];
    int32_t link_count;
    int32_t total_link_count;
    uint32_t current_glyph; /* native DWORDD3B5C0, live across sprite calls */
    uint32_t draw_mode; /* native DWORDE20780 */
    uint8_t draw_active; /* native BYTEE20799 */
    const uint8_t *decimal_tail; /* borrowed file_read_part.media_prefixed_path */
    uint8_t line_buffer[200]; /* native E3439C; original valid lines fit */
} PharaohRichTextState;

typedef struct PharaohRichTextScrollbarState {
    int32_t mouse_x;
    int32_t mouse_y;
    uint8_t mouse_left_down;
    int32_t panel_x;
    int32_t panel_y;
    int32_t panel_width_blocks;
    int32_t panel_height_blocks;
    int32_t scroll_maximum;
    int32_t scroll_position;
    int32_t drag_position;
    uint8_t dragging;
    uint8_t redraw_requested;
} PharaohRichTextScrollbarState;

/* Complete typed translations of native 0x004CC070, 0x004C8040,
   0x004C7FD0, 0x004CC020, 0x004C8480, 0x004C8680, 0x004C8590 and
   0x004C8070. */
/* Exact native004CC070: tail is the live byte storage immediately after the
 * eight DWORD powers (native005E05AC, shared media-prefixed filename).
 * Callers must provide enough readable tail bytes for the digit count. */
int32_t pharaoh_parse_decimal_with_tail(const uint8_t *text,const uint8_t *tail);
/* Convenience for at most eight digits, excluding an optional leading minus. */
int32_t pharaoh_parse_decimal(const uint8_t *text);
void pharaoh_rich_text_reset_links(PharaohRichTextState *state);
void pharaoh_rich_text_add_link(PharaohRichTextState *state, int32_t id,
    int32_t left, int32_t right, int32_t y);
int32_t pharaoh_rich_text_character_width(PharaohRichTextState *state,
    uint8_t character, uint32_t font);
/* Draw callbacks are required when calculate is zero. The callback shares
 * current_glyph, color and metric fields with its renderer; they stay live.
 * Native numeric link markers require their following delimiter/text. */
int32_t pharaoh_rich_text_draw_glyph(PharaohRichTextState *state,
    const PharaohRichTextOperations *operations,uint32_t font,uint8_t glyph,
    int32_t x,int32_t y,int32_t calculate,int32_t flags);
void pharaoh_rich_text_draw_line(PharaohRichTextState *state,
    const PharaohRichTextOperations *operations,const uint8_t *text,
    int32_t x,int32_t y,int32_t calculate,int32_t flags);
int32_t pharaoh_rich_text_word_width(PharaohRichTextState *state,
    const uint8_t *text);
void pharaoh_rich_text_draw(PharaohRichTextState *state,
    const PharaohRichTextOperations *operations, const uint8_t *text,
    int32_t x, int32_t y, int32_t width, int32_t calculate,
    int32_t flags);

/* Complete typed translation of native 0x004C7DB0. */
int32_t pharaoh_rich_text_handle_scrollbar(
    PharaohRichTextScrollbarState *scrollbar,
    PharaohRichTextState *rich_text);

#endif
