#include "pharaoh/ui/custom_map_selection.h"

#include <string.h>

static const char custom_map_pattern[] = "maps\\*.map";

static void copy_string(char *destination, const char *source)
{
    while ((*destination++ = *source++) != '\0') { }
}

static void remove_extension(char *text)
{
    while (*text != '\0') {
        if (*text == '.') {
            *text = '\0';
            return;
        }
        ++text;
    }
}

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
    state->first_visible_file = 0;
    state->visible_rows = 15;
    state->campaign_mode = 0;
    state->selected_name[0] = '\0';
    state->preview.valid = 0;
    state->preview_invasion_count = 0;
    state->preview_current_event = 0;
    state->preview_invasion_search = 0;
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
    state->first_visible_file = 0;
    state->visible_rows = 13;
    state->campaign_mode = 1;
    state->selected_name[0] = '\0';
    state->preview.valid = 0;
    state->preview_invasion_count = 0;
    state->preview_current_event = 0;
    state->preview_invasion_search = 0;
    for (int32_t index = 0; index < state->files->count; ++index) {
        state->mission_ids[index] = index;
    }
    return state->files->count;
}

void pharaoh_map_selection_show(
    PharaohCustomMapSelectionState *state, const PharaohCampaign *campaign,
    const PharaohMapSelectionOperations *operations,
    uint8_t *current_window_id, uint8_t *redraw_requested)
{
    *current_window_id = 0x3d;
    if (state->campaign_mode != 0) {
        (void)pharaoh_campaign_mission_selection_load(state, campaign);
    } else {
        (void)pharaoh_custom_map_selection_load(state);
    }
    state->show_flag_a = 0;
    state->first_visible_file = 0;
    state->show_flag_b = 1;
    (void)pharaoh_map_selection_activate(state, state->selected_file,
        operations, redraw_requested);
    *redraw_requested = 1;
}

int32_t pharaoh_map_selection_entry_mission(
    const PharaohCustomMapSelectionState *state, int32_t index)
{
    if (index < 0 || index >= state->files->count) return -1;
    return state->mission_ids[index];
}

int32_t pharaoh_map_selection_entry_kind(
    const PharaohCustomMapSelectionState *state, int32_t index)
{
    if (index < 0 || index >= state->files->count) return 3;
    return state->campaign_mode != 0;
}

const char *pharaoh_map_selection_entry_name(
    const PharaohCustomMapSelectionState *state, int32_t index)
{
    return pharaoh_file_list_name(state->files, index);
}

int32_t pharaoh_map_selection_difficulty_buttons_visible(
    int32_t returns_to_front_menu, int32_t campaign_mode,
    int32_t map_mode)
{
    return returns_to_front_menu != 1 &&
        (campaign_mode != 1 || map_mode != 2);
}

int32_t pharaoh_map_selection_image_button_count(
    const PharaohCustomMapSelectionState *state)
{
    return 3 + (pharaoh_map_selection_entry_kind(
        state, state->selected_file) != 2);
}

int32_t pharaoh_map_selection_generic_button_count(int32_t campaign_mode)
{
    return campaign_mode == 1 ? 17 : 15;
}

int32_t pharaoh_map_selection_activate(
    PharaohCustomMapSelectionState *state, int32_t visible_row,
    const PharaohMapSelectionOperations *operations,
    uint8_t *redraw_requested)
{
    char path[100] = "maps\\";
    const int32_t selected = state->first_visible_file + visible_row;
    const char *name;
    int32_t kind;

    if (visible_row < 0 || selected < 0 || selected >= state->files->count)
        return 0;
    state->selected_file = selected;
    name = pharaoh_map_selection_entry_name(state, selected);
    if (name == NULL) return 0;

    /* The native destination is 64 bytes and relies on the file-list name
       capacity. Preserve that caller precondition instead of truncating. */
    memset(state->selected_name, 0, sizeof(state->selected_name));
    copy_string(state->selected_name, name);
    remove_extension(state->selected_name);
    kind = pharaoh_map_selection_entry_kind(state, selected);
    if (kind == 0) {
        copy_string(path + 5, state->selected_name);
        copy_string(path + strlen(path), ".map");
        (void)operations->load_custom_map(operations->user_data, path, 1);
        remove_extension(state->selected_name);
        *redraw_requested = 1;
        return 1;
    }
    if (kind > 0 && kind < 3) {
        state->warning_text_id =
            pharaoh_map_selection_entry_mission(state, selected);
        (void)operations->load_campaign_mission(
            operations->user_data, 0, 1);
        copy_string(state->selected_name, name);
        remove_extension(state->selected_name);
        *redraw_requested = 1;
        return 1;
    }
    return 0;
}

int32_t pharaoh_map_selection_start(
    const PharaohCustomMapSelectionState *selection,
    const PharaohMapSelectionOperations *operations,
    const PharaohMapSelectionStartState *state)
{
    const int32_t kind = pharaoh_map_selection_entry_kind(selection,
        selection->selected_file);

    operations->play_restore_sound(operations->user_data);
    if (kind > 1) return 0;
    *state->game_is_replay = kind != 0 ? 3 : 0;
    operations->prepare_start(operations->user_data, 1);
    operations->load_selected_scenario(operations->user_data,
        selection->selected_name, kind);
    if (*state->game_is_replay == 3) {
        operations->enter_campaign(operations->user_data);
    } else {
        *state->current_window_id = 1;
        *state->transition_mode = 2;
    }
    *state->redraw_requested = 1;
    return 1;
}

void pharaoh_map_selection_back(
    int32_t returns_to_front_menu, uint8_t *current_window_id,
    uint8_t *redraw_requested,
    const PharaohMapSelectionBackOperations *operations)
{
    if (returns_to_front_menu != 0) {
        *current_window_id = 0;
        *redraw_requested = 1;
        return;
    }
    operations->show_main_menu(operations->user_data);
}

void pharaoh_map_selection_toggle_mode(uint32_t *map_mode,
    uint8_t *redraw_requested)
{
    *redraw_requested = 1;
    *map_mode = (*map_mode != 3u) + 2u;
}

void pharaoh_map_selection_adjust_difficulty(
    int32_t decrement, int32_t *difficulty, uint8_t current_window_id,
    int32_t scenario_value, const int32_t difficulty_values[5],
    int32_t *player_value, uint8_t *redraw_requested,
    const PharaohMapSelectionOperations *operations)
{
    int32_t next = decrement != 0 ? *difficulty - 1 : *difficulty + 1;
    if (next < 0) next = 0;
    else if (next >= 5) next = 4;
    *difficulty = next;

    operations->refresh_difficulty_a(operations->user_data);
    operations->refresh_difficulty_b(operations->user_data);
    operations->refresh_difficulty_c(operations->user_data);
    operations->reload_map_assets(operations->user_data);
    if (current_window_id == 0x35) {
        *player_value = operations->adjust_value(operations->user_data,
            scenario_value, difficulty_values[next]);
    }
    *redraw_requested = 1;
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

int32_t pharaoh_map_selection_scroll_thumb_offset(
    const PharaohCustomMapSelectionState *state, int32_t *offset)
{
    const int32_t remaining = state->files->count - state->visible_rows;
    int32_t percent;
    if (remaining <= 0) return 0;
    if (state->first_visible_file < 1) percent = 0;
    else if (state->first_visible_file + state->visible_rows <
             state->files->count) {
        percent = state->first_visible_file * 100 / remaining;
    } else percent = 100;
    *offset = (state->visible_rows * 16 - 0x4c) * percent / 100;
    return 1;
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
