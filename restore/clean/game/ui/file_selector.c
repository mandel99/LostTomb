#include "pharaoh/ui/file_selector.h"
#include "pharaoh/ui/player_selection.h"
#include "pharaoh/runtime/file_extension_operations.h"

#include <string.h>

static void copy_native_string(char *destination, const char *source)
{
    memcpy(destination, source, strlen(source) + 1u);
}

void pharaoh_file_selector_initialize(PharaohFileSelectorState *state,
    uint8_t maps)
{
    if (maps == 0) {
        char pattern[260];
        copy_native_string(pattern, pharaoh_player_directory_path(
            state->family_directory, state->player_name));
        copy_native_string(pattern + strlen(pattern), "\\*.sav");
        (void)pharaoh_file_list_load(state->files, pattern, 0);
    } else {
        (void)pharaoh_file_list_load(state->files, "maps\\*.map", 0);
    }
    *state->file_count = state->files->count;
    for (int32_t index = 0; index < *state->file_count; ++index) {
        /* This native parent inlines the regular-list lookup, without the
           campaign-name branch of0041E010. Invalid backing is not repaired. */
        char *const name = (char *)pharaoh_file_list_name(state->files, index);
        pharaoh_file_remove_extension(name);
    }
    pharaoh_file_list_sort(state->files);
    *state->first_visible = 0;
    *state->selected_file = 0;
    *state->selection_active = 0;
    *state->slot_6059ec = 0;
    *state->slot_e36efc = 0;
}
