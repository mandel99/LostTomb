#include "pharaoh/ui/custom_map_selection.h"

static const char custom_map_pattern[] = "maps\\*.map";

int32_t pharaoh_custom_map_selection_load(
    PharaohCustomMapSelectionState *state)
{
    (void)pharaoh_file_list_load(state->files, custom_map_pattern, 0);
    pharaoh_file_list_sort(state->files);
    state->first_visible_file = 0;
    return state->files->count;
}

int32_t pharaoh_custom_map_selection_select_visible(
    PharaohCustomMapSelectionState *state, int32_t visible_row)
{
    const int32_t selected = state->first_visible_file + visible_row;

    if (visible_row < 0 || visible_row >= PHARAOH_CUSTOM_MAP_VISIBLE_ROWS ||
        selected < 0 || selected >= state->files->count) {
        return 0;
    }
    state->selected_file = selected;
    return 1;
}

int32_t pharaoh_custom_map_selection_scroll(
    PharaohCustomMapSelectionState *state, int32_t direction)
{
    const int32_t count = state->files->count;
    const int32_t previous = state->first_visible_file;

    if (count > PHARAOH_CUSTOM_MAP_VISIBLE_ROWS) {
        if (direction == 0 && previous > 0) {
            state->first_visible_file = previous - 1;
        } else if (direction == 1 &&
                previous + PHARAOH_CUSTOM_MAP_VISIBLE_ROWS < count) {
            state->first_visible_file = previous + 1;
        }
    }
    return state->first_visible_file != previous;
}

int32_t pharaoh_custom_map_selection_drag_scrollbar(
    PharaohCustomMapSelectionState *state, int32_t mouse_x, int32_t mouse_y,
    int32_t origin_x, int32_t origin_y)
{
    enum {
        TRACK_LEFT = 0x118,
        TRACK_RIGHT = 0x138,
        TRACK_TOP = 0xDC,
        TRACK_INSET = 0x19,
        TRACK_RANGE = 0xA4
    };
    const int32_t count = state->files->count;
    const int32_t previous = state->first_visible_file;
    int32_t track_position;
    int32_t percentage;

    if (count <= PHARAOH_CUSTOM_MAP_VISIBLE_ROWS ||
        mouse_x < origin_x + TRACK_LEFT ||
        mouse_x > origin_x + TRACK_RIGHT ||
        mouse_y < origin_y + TRACK_TOP + TRACK_INSET ||
        mouse_y > origin_y + TRACK_TOP + 0x32 + TRACK_RANGE) {
        return 0;
    }
    track_position = mouse_y - origin_y - TRACK_TOP - TRACK_INSET;
    if (track_position > TRACK_RANGE) track_position = TRACK_RANGE;
    percentage = track_position * 100 / TRACK_RANGE;
    state->first_visible_file = (count - PHARAOH_CUSTOM_MAP_VISIBLE_ROWS) *
        percentage / 100;
    return state->first_visible_file != previous;
}
