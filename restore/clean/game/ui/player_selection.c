#include "pharaoh/ui/player_selection.h"

#include <string.h>

#include "pharaoh/runtime/file_extension_operations.h"
#include "pharaoh/ui/menu_button_frame.h"

enum {
    PLAYER_SELECTION_TEXT_GROUP = 292,
    PLAYER_SELECTION_TITLE_ENTRY = 3,
    PLAYER_SELECTION_NORMAL_FONT = 636,
    PLAYER_SELECTION_SELECTED_FONT = 637,
    PLAYER_SELECTION_BUTTON_FONT = 635,
    PLAYER_SELECTION_TITLE_FONT = 640,
    PLAYER_SELECTION_PANEL_X = 128,
    PLAYER_SELECTION_PANEL_Y = 40,
    PLAYER_SELECTION_PANEL_WIDTH = 24,
    PLAYER_SELECTION_PANEL_HEIGHT = 23,
    PLAYER_SELECTION_INNER_X = 144,
    PLAYER_SELECTION_INNER_Y = 120,
    PLAYER_SELECTION_INNER_WIDTH = 20,
    PLAYER_SELECTION_INNER_HEIGHT = 13,
    PLAYER_SELECTION_TITLE_X = 128,
    PLAYER_SELECTION_TITLE_Y = 58,
    PLAYER_SELECTION_TITLE_WIDTH = 384,
    PLAYER_SELECTION_NAME_X = 160,
    PLAYER_SELECTION_NAME_Y = 128,
    PLAYER_SELECTION_NAME_STEP = 16,
    PLAYER_SELECTION_SCROLL_TRACK_X = 467,
    PLAYER_SELECTION_SCROLL_TRACK_Y = 146,
    PLAYER_SELECTION_SCROLL_TRACK_WIDTH = 2,
    PLAYER_SELECTION_SCROLL_TRACK_HEIGHT = 10,
    PLAYER_SELECTION_SCROLL_THUMB_X = 472,
    PLAYER_SELECTION_SCROLL_THUMB_Y = 145,
    PLAYER_SELECTION_SCROLL_RANGE = 0x82,
    PLAYER_SELECTION_SCROLLBAR_THUMB_OFFSET = 0x27,
    PLAYER_SELECTION_BUTTON_COUNT = 4
};

typedef struct PharaohPlayerSelectionButtonDraw {
    int16_t left, top, right, bottom;
    int32_t label;
} PharaohPlayerSelectionButtonDraw;

static const PharaohPlayerSelectionButtonDraw
    player_selection_buttons[PLAYER_SELECTION_BUTTON_COUNT] = {
    {142, 332, 267, 357, 0},
    {276, 332, 406, 357, 1},
    {418, 332, 498, 357, 2},
    {196, 362, 446, 387, 4}
};

static const char profile_pattern[] = "Save\\*.dat";

static void set_player_name(PharaohPlayerSelectionState *state,
                            int32_t profile_index)
{
    const char *name = pharaoh_file_list_name(state->profiles, profile_index);
    size_t length;

    memset(state->player_name, 0, sizeof(state->player_name));
    if (name == NULL) return;
    length = strlen(name);
    if (length >= sizeof(state->player_name)) {
        length = sizeof(state->player_name) - 1;
    }
    memcpy(state->player_name, name, length);
}

int32_t pharaoh_player_selection_load_profiles(
    PharaohPlayerSelectionState *state, int32_t previous_profile)
{
    PharaohFileList *const profiles = state->profiles;

    memset(state->player_name, 0, sizeof(state->player_name));
    state->selected_profile = -1;
    state->first_visible_profile = 0;
    (void)pharaoh_file_list_load(profiles, profile_pattern, 0);
    for (int32_t index = 0; index < profiles->count; ++index) {
        char *name = (char *)pharaoh_file_list_name(profiles, index);
        pharaoh_file_remove_extension(name);
    }
    pharaoh_file_list_sort(profiles);

    if (previous_profile >= 0 && previous_profile < profiles->count) {
        state->selected_profile = previous_profile;
        set_player_name(state, previous_profile);
    }
    return profiles->count;
}

int32_t pharaoh_player_selection_select_visible(
    PharaohPlayerSelectionState *state, int32_t visible_row)
{
    const int32_t selected = state->first_visible_profile + visible_row;
    if (visible_row < 0 || visible_row >=
            PHARAOH_PLAYER_SELECTION_VISIBLE_ROWS ||
        selected < 0 || selected >= state->profiles->count) {
        return 0;
    }
    state->selected_profile = selected;
    set_player_name(state, selected);
    return 1;
}

int32_t pharaoh_player_selection_scroll(
    PharaohPlayerSelectionState *state, int32_t direction)
{
    const int32_t count = state->profiles->count;
    const int32_t previous = state->first_visible_profile;

    if (count > PHARAOH_PLAYER_SELECTION_VISIBLE_ROWS) {
        if (direction == 0 && previous > 0) {
            state->first_visible_profile = previous - 1;
        } else if (direction == 1 &&
                previous + PHARAOH_PLAYER_SELECTION_VISIBLE_ROWS < count) {
            state->first_visible_profile = previous + 1;
        }
    }
    return state->first_visible_profile != previous;
}

int32_t pharaoh_player_selection_drag_scrollbar(
    PharaohPlayerSelectionState *state, int32_t mouse_y, int32_t origin_y)
{
    const int32_t count = state->profiles->count;
    const int32_t previous = state->first_visible_profile;
    int32_t track_position = mouse_y - origin_y - 0x91;
    int32_t percentage;

    if (count < 13 || track_position < 0) {
        return 0;
    }
    if (track_position > 0x82) track_position = 0x82;
    percentage = track_position * 100 / 0x82;
    state->first_visible_profile =
        (count - PHARAOH_PLAYER_SELECTION_VISIBLE_ROWS) * percentage / 100;
    return state->first_visible_profile != previous;
}

void pharaoh_player_selection_draw(
    const PharaohPlayerSelectionState *state,
    const PharaohPlayerSelectionDrawOperations *operations,
    int32_t origin_x, int32_t origin_y, int32_t focused_button)
{
    const PharaohPanelFrameOperations outer_frame = {
        operations->user_data, operations->draw_sprite,
        operations->panel_image_base, 0
    };
    const PharaohPanelFrameOperations inner_frame = {
        operations->user_data, operations->draw_sprite,
        operations->inner_panel_image_base, 0
    };

    pharaoh_panel_frame_draw(&outer_frame,
        origin_x + PLAYER_SELECTION_PANEL_X,
        origin_y + PLAYER_SELECTION_PANEL_Y,
        PLAYER_SELECTION_PANEL_WIDTH, PLAYER_SELECTION_PANEL_HEIGHT);
    pharaoh_inner_panel_frame_draw(&inner_frame,
        origin_x + PLAYER_SELECTION_INNER_X,
        origin_y + PLAYER_SELECTION_INNER_Y,
        PLAYER_SELECTION_INNER_WIDTH, PLAYER_SELECTION_INNER_HEIGHT);
    operations->draw_language(operations->user_data,
        PLAYER_SELECTION_TEXT_GROUP, PLAYER_SELECTION_TITLE_ENTRY,
        origin_x + PLAYER_SELECTION_TITLE_X,
        origin_y + PLAYER_SELECTION_TITLE_Y,
        PLAYER_SELECTION_TITLE_WIDTH, PLAYER_SELECTION_TITLE_FONT, 0);

    for (int32_t row = 0; row < PHARAOH_PLAYER_SELECTION_VISIBLE_ROWS; ++row) {
        const int32_t profile = state->first_visible_profile + row;
        const char *const name = pharaoh_file_list_name(state->profiles,
            profile);
        if (name != NULL) {
            const int32_t font = focused_button == row + 1 ||
                state->selected_profile == profile ?
                PLAYER_SELECTION_SELECTED_FONT : PLAYER_SELECTION_NORMAL_FONT;
            operations->draw_text(operations->user_data, name,
                origin_x + PLAYER_SELECTION_NAME_X,
                origin_y + PLAYER_SELECTION_NAME_Y +
                    row * PLAYER_SELECTION_NAME_STEP,
                font, 0);
        }
    }

    for (int32_t index = 0; index < PLAYER_SELECTION_BUTTON_COUNT; ++index) {
        const int32_t focus_index = 13 + index;
        const int32_t image_base = operations->button_image_base;
        const PharaohMenuButtonFrameOperations button_frame = {
            operations->user_data, operations->draw_sprite
        };
        const PharaohPlayerSelectionButtonDraw *const button =
            &player_selection_buttons[index];

        pharaoh_menu_button_frame_draw(&button_frame, image_base,
            origin_x + button->left, origin_y + button->top,
            button->right - button->left,
            button->bottom - button->top,
            focused_button == focus_index);
        operations->draw_language(operations->user_data,
            PLAYER_SELECTION_TEXT_GROUP, button->label,
            origin_x + button->left + 4, origin_y + button->top + 7,
            button->right - button->left, PLAYER_SELECTION_BUTTON_FONT, 0);
    }

    operations->draw_sprite(operations->user_data,
        operations->arrow_image_base + 8, origin_x + 464, origin_y + 120);
    operations->draw_sprite(operations->user_data,
        operations->arrow_image_base + 12, origin_x + 464, origin_y + 300);
    pharaoh_inner_panel_frame_draw(&inner_frame,
        origin_x + PLAYER_SELECTION_SCROLL_TRACK_X,
        origin_y + PLAYER_SELECTION_SCROLL_TRACK_Y,
        PLAYER_SELECTION_SCROLL_TRACK_WIDTH,
        PLAYER_SELECTION_SCROLL_TRACK_HEIGHT);

    if (state->profiles->count > PHARAOH_PLAYER_SELECTION_VISIBLE_ROWS) {
        const int32_t maximum = state->profiles->count -
            PHARAOH_PLAYER_SELECTION_VISIBLE_ROWS;
        const int32_t percent = state->first_visible_profile * 100 / maximum;
        const int32_t offset = PLAYER_SELECTION_SCROLL_RANGE * percent / 100;
        operations->draw_sprite(operations->user_data,
            operations->scrollbar_thumb_image,
            origin_x + PLAYER_SELECTION_SCROLL_THUMB_X,
            origin_y + PLAYER_SELECTION_SCROLL_THUMB_Y + offset);
    }
}
