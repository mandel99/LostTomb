#ifndef PHARAOH_RUNTIME_FAST_FORWARD_NOTE_H
#define PHARAOH_RUNTIME_FAST_FORWARD_NOTE_H

#include <stdint.h>

typedef struct PharaohFastForwardNoteOperations {
    void *user_data;
    void (*draw_panel)(void *user_data, int32_t x, int32_t y,
        int32_t width_blocks, int32_t height_blocks);
    void (*draw_wrapped_localized_text)(void *user_data, int32_t text_group,
        int32_t text_entry, int32_t x, int32_t y, int32_t width,
        int32_t font_id, int32_t flags);
} PharaohFastForwardNoteOperations;

typedef struct PharaohFastForwardNoteContext {
    int32_t ui_origin_x;
    int32_t ui_origin_y;
    int32_t first_line_font_id;
    int32_t second_line_font_id;
    const PharaohFastForwardNoteOperations *operations;
} PharaohFastForwardNoteContext;

/* Original Pharaoh.exe entry 0x00414C40. */
void pharaoh_fast_forward_note_draw(
    const PharaohFastForwardNoteContext *context);

#endif
