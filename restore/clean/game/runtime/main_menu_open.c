#include "pharaoh/ui/main_menu_open.h"

#include <stdio.h>

enum {
    NATIVE_MENU_WINDOW_ID = 0x30,
    NATIVE_FILE_LIST_SEARCH_MODE = 0,
    MENU_SAVE_PATTERN_BYTES = 260
};

void pharaoh_main_menu_window_show(const PharaohMainMenuOpenContext *context)
{
    PharaohMainMenuWindowState *state = context->state;
    const PharaohMainMenuOpenOperations *operations = context->operations;
    state->campaign_period_open = pharaoh_campaign_any_period_open(
        context->campaign_index);
    state->menu_active = 1;
    state->save_directory_has_files = 0;
    state->selected_save_available = 0;

    if (context->selected_save_filename[0] != '\0') {
        PharaohReadOnlyFile *file = operations->open_file(
            operations->user_data, context->selected_save_filename, "rb");
        if (file != NULL) {
            (void)operations->close_file(operations->user_data, file);
            state->selected_save_available = 1;
        }
    }

    char save_pattern[MENU_SAVE_PATTERN_BYTES];
    (void)snprintf(save_pattern, sizeof(save_pattern), "save\\%s\\*.sav",
        context->save_directory);
    if (operations->count_matching_files(operations->user_data, save_pattern,
            NATIVE_FILE_LIST_SEARCH_MODE) > 0) {
        state->save_directory_has_files = 1;
    }

    state->active_window = NATIVE_MENU_WINDOW_ID;
    state->redraw_requested = 1;
}
