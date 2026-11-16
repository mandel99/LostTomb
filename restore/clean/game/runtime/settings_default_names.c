#include "pharaoh/runtime/settings_default_names.h"

#include <string.h>

enum {
    PHARAOH_DEFAULT_NAME_TEXT_GROUP = 9,
    PHARAOH_DEFAULT_NAME_FIRST_ENTRY = 5,
    PHARAOH_DEFAULT_NAME_SHARED_ENTRY = 6,
    PHARAOH_DEFAULT_NAME_LAST_ENTRY = 7
};

static void copy_selected_text(char *destination, const char *text)
{
    const size_t bytes_including_terminator = strlen(text) + 1;
    memcpy(destination, text, bytes_including_terminator);
}

void pharaoh_settings_load_default_names(
    PharaohTextSelection *selection,
    const PharaohSettingsDefaultNameDestinations *destinations)
{
    pharaoh_text_select(selection, PHARAOH_DEFAULT_NAME_TEXT_GROUP,
        PHARAOH_DEFAULT_NAME_FIRST_ENTRY);
    copy_selected_text(destinations->group_9_entry_5, selection->current);

    pharaoh_text_select(selection, PHARAOH_DEFAULT_NAME_TEXT_GROUP,
        PHARAOH_DEFAULT_NAME_SHARED_ENTRY);
    copy_selected_text(destinations->group_9_entry_6_first_copy,
        selection->current);
    copy_selected_text(destinations->group_9_entry_6_second_copy,
        selection->current);

    pharaoh_text_select(selection, PHARAOH_DEFAULT_NAME_TEXT_GROUP,
        PHARAOH_DEFAULT_NAME_LAST_ENTRY);
    copy_selected_text(destinations->group_9_entry_7, selection->current);
}
