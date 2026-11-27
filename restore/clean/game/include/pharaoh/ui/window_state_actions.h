#ifndef PHARAOH_UI_WINDOW_STATE_ACTIONS_H
#define PHARAOH_UI_WINDOW_STATE_ACTIONS_H
#include <stdint.h>
void pharaoh_tooltip_invalidate(int32_t current_tick,int32_t *tooltip_id,int32_t *tooltip_tick);
struct PharaohMenuBarFocusRecord;
typedef struct PharaohMenuBarState {
    const uint8_t *left_pressed; /* E38E03 */
    const int32_t *hovered_header; /* 7056A8; not an item count */
    uint8_t *current_window,*flag_de0,*flag_de1,*redraw_requested;
    uint32_t *return_window,*selected_header,*hovered_item;
    const struct PharaohMenuBarFocusRecord **active_header;
    int32_t *refresh_mode;
} PharaohMenuBarState;

/* Complete native00426030/6090/60D0. Supply a relocated backing catalogue
 * including the opaque preceding16 bytes: native index0 stores headers-1.
 * Pointer offset uses the original DWORD multiply. Native direct producers
 * keep selected_header0..count; resulting C references must be in that backing.
 * The active header is a relocated C pointer, never a native integer VA. */
int32_t pharaoh_menu_bar_select(const PharaohMenuBarState *,
    const struct PharaohMenuBarFocusRecord *headers);
void pharaoh_menu_bar_close(const PharaohMenuBarState *);
void pharaoh_menu_bar_clear(const PharaohMenuBarState *);

void pharaoh_city_ratings_store_culture_lowest(
    int32_t *culture_explanation, int32_t culture_lowest);

void pharaoh_game_undo_finish_build(uint32_t placement_cursor,
    uint32_t *undo_cursor, int32_t *undo_ticks,
    int32_t *undo_ready, uint8_t *redraw_requested);
void pharaoh_build_sidebar_hover_update(int32_t mouse_x, int32_t mouse_y,
    int32_t left, int32_t top, int32_t right, int32_t bottom,
    int32_t *hovered);
#endif
