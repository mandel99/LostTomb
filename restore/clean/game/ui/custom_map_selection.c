#include "pharaoh/ui/custom_map_selection.h"

#include <string.h>

static const char custom_map_pattern[] = "maps\\*.map";

int32_t pharaoh_custom_map_selection_load(
    PharaohCustomMapSelectionState *state)
{
    (void)pharaoh_file_list_load(state->files, custom_map_pattern, 0);
    /* 0041F810 calls 004CE3A0 for each filename before sorting. */
    for (int32_t index = 0; index < state->files->count; ++index) {
        char *name = state->files->strings + state->files->offsets[index];
        char *extension = strchr(name, '.');
        if (extension != NULL) *extension = '\0';
    }
    pharaoh_file_list_sort(state->files);
    state->selected_file = 0;
    state->first_visible_file = 0;
    state->visible_rows = 15;
    return state->files->count;
}

int32_t pharaoh_campaign_mission_selection_load(
    PharaohCustomMapSelectionState *state, const PharaohCampaign *campaign)
{
    if (campaign->mission_name_count < 0 ||
        campaign->mission_name_count > PHARAOH_CAMPAIGN_INDEX_CAPACITY ||
        pharaoh_mission_file_list_copy_names(state->files,
            &campaign->mission_names[0][0], campaign->mission_name_count,
            sizeof(campaign->mission_names[0])) == 0) {
        return 0;
    }
    state->selected_file = 0;
    state->first_visible_file = 0;
    state->visible_rows = 13;
    return state->files->count;
}

int32_t pharaoh_custom_map_selection_select_visible(
    PharaohCustomMapSelectionState *state, int32_t visible_row)
{
    const int32_t selected = state->first_visible_file + visible_row;

    if (visible_row < 0 || visible_row >= state->visible_rows ||
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

    if (count > state->visible_rows) {
        if (direction == 0 && previous > 0) {
            state->first_visible_file = previous - 1;
        } else if (direction == 1 &&
                previous + state->visible_rows < count) {
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
        TRACK_INSET = 0x19
    };
    const int32_t count = state->files->count;
    const int32_t previous = state->first_visible_file;
    int32_t track_position;
    int32_t percentage;

    if (count <= state->visible_rows ||
        mouse_x < origin_x + TRACK_LEFT ||
        mouse_x > origin_x + TRACK_RIGHT) {
        return 0;
    }
    const int32_t vertical_shift = state->visible_rows == 13 ? 32 : 0;
    const int32_t range = state->visible_rows * 16 - 0x4c;
    if (mouse_y < origin_y + TRACK_TOP + vertical_shift + TRACK_INSET ||
        mouse_y > origin_y + TRACK_TOP + vertical_shift + 0x32 + range)
        return 0;
    track_position = mouse_y - origin_y - TRACK_TOP - vertical_shift - TRACK_INSET;
    if (track_position > range) track_position = range;
    percentage = track_position * 100 / range;
    state->first_visible_file = (count - state->visible_rows) *
        percentage / 100;
    return state->first_visible_file != previous;
}
