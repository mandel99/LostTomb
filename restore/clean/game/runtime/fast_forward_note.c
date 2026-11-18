#include "pharaoh/runtime/fast_forward_note.h"

enum {
    FAST_FORWARD_NOTE_PANEL_X = 0x80,
    FAST_FORWARD_NOTE_PANEL_Y = 0xa0,
    FAST_FORWARD_NOTE_PANEL_WIDTH_BLOCKS = 0x18,
    FAST_FORWARD_NOTE_PANEL_HEIGHT_BLOCKS = 7,
    FAST_FORWARD_NOTE_TEXT_GROUP = 0x0b,
    FAST_FORWARD_NOTE_FIRST_ENTRY = 5,
    FAST_FORWARD_NOTE_SECOND_ENTRY = 6,
    FAST_FORWARD_NOTE_TEXT_X = 0x80,
    FAST_FORWARD_NOTE_FIRST_TEXT_Y = 0xbc,
    FAST_FORWARD_NOTE_SECOND_TEXT_Y = 0xdc,
    FAST_FORWARD_NOTE_TEXT_WIDTH = 0x180,
    FAST_FORWARD_NOTE_TEXT_FLAGS = 0
};

void pharaoh_fast_forward_note_draw(
    const PharaohFastForwardNoteContext *context)
{
    const PharaohFastForwardNoteOperations *const operations =
        context->operations;

    operations->draw_panel(operations->user_data,
        context->ui_origin_x + FAST_FORWARD_NOTE_PANEL_X,
        context->ui_origin_y + FAST_FORWARD_NOTE_PANEL_Y,
        FAST_FORWARD_NOTE_PANEL_WIDTH_BLOCKS,
        FAST_FORWARD_NOTE_PANEL_HEIGHT_BLOCKS);
    operations->draw_wrapped_localized_text(operations->user_data,
        FAST_FORWARD_NOTE_TEXT_GROUP, FAST_FORWARD_NOTE_FIRST_ENTRY,
        context->ui_origin_x + FAST_FORWARD_NOTE_TEXT_X,
        context->ui_origin_y + FAST_FORWARD_NOTE_FIRST_TEXT_Y,
        FAST_FORWARD_NOTE_TEXT_WIDTH, context->first_line_font_id,
        FAST_FORWARD_NOTE_TEXT_FLAGS);
    operations->draw_wrapped_localized_text(operations->user_data,
        FAST_FORWARD_NOTE_TEXT_GROUP, FAST_FORWARD_NOTE_SECOND_ENTRY,
        context->ui_origin_x + FAST_FORWARD_NOTE_TEXT_X,
        context->ui_origin_y + FAST_FORWARD_NOTE_SECOND_TEXT_Y,
        FAST_FORWARD_NOTE_TEXT_WIDTH, context->second_line_font_id,
        FAST_FORWARD_NOTE_TEXT_FLAGS);
}
