#ifndef PHARAOH_RUNTIME_SETTINGS_DEFAULT_NAMES_H
#define PHARAOH_RUNTIME_SETTINGS_DEFAULT_NAMES_H

#include "pharaoh/runtime/text_loading.h"

typedef struct PharaohSettingsDefaultNameDestinations {
    char *group_9_entry_5;
    char *group_9_entry_6_first_copy;
    char *group_9_entry_6_second_copy;
    char *group_9_entry_7;
} PharaohSettingsDefaultNameDestinations;

void pharaoh_settings_load_default_names(
    PharaohTextSelection *selection,
    const PharaohSettingsDefaultNameDestinations *destinations);

#endif
