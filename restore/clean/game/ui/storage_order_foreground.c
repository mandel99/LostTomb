#include "pharaoh/ui/storage_order_foreground.h"

static void draw_orders_button(
    const PharaohStorageOrderForegroundState *state,
    const PharaohStorageOrderForegroundOperations *operations,
    int32_t language_group, int32_t language_entry)
{
    operations->draw_button_border(operations->user_data, 0,
        state->building_info_x + 100,
        state->building_info_height * 16 - 34 + state->building_info_y,
        (state->info_width_blocks - 12) * 16, 20,
        state->focus_button_id == 1,
        state->building_info_x, state->building_info_y);
    operations->draw_centered_language(operations->user_data,
        language_group, language_entry,
        state->building_info_x + 80,
        state->building_info_height * 16 - 30 + state->building_info_y,
        (state->info_width_blocks - 10) * 16,
        state->normal_font, 0,
        state->building_info_x, state->building_info_y);
}

/* Complete two-callee C translation of Pharaoh.exe 0x00501140. */
void pharaoh_building_info_draw_bazaar_foreground(
    const PharaohStorageOrderForegroundState *state,
    const PharaohStorageOrderForegroundOperations *operations)
{
    draw_orders_button(state, operations, 97, 6);
}

/* Complete two-callee C translation of Pharaoh.exe 0x00501A40. */
void pharaoh_building_info_draw_granary_foreground(
    const PharaohStorageOrderForegroundState *state,
    const PharaohStorageOrderForegroundOperations *operations)
{
    draw_orders_button(state, operations, 98, 5);
}

/* Complete two-callee C translation of Pharaoh.exe 0x005026F0. */
void pharaoh_building_info_draw_storage_yard_foreground(
    const PharaohStorageOrderForegroundState *state,
    const PharaohStorageOrderForegroundOperations *operations)
{
    draw_orders_button(state, operations, 99, 2);
}
