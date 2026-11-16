#ifndef PHARAOH_RUNTIME_APPLICATION_STARTUP_H
#define PHARAOH_RUNTIME_APPLICATION_STARTUP_H

#include "pharaoh/runtime/settings_default_names.h"
#include "pharaoh/runtime/settings_load.h"
#include "pharaoh/runtime/text_loading.h"

typedef enum PharaohApplicationConfigurationResult {
    PHARAOH_APP_CONFIGURATION_READY = 0,
    PHARAOH_APP_CONFIGURATION_SETTINGS_FAILED = 1,
    PHARAOH_APP_CONFIGURATION_TEXT_FAILED = 2
} PharaohApplicationConfigurationResult;

typedef struct PharaohApplicationConfiguration {
    const PharaohSettingsLoadContext *settings;
    const PharaohTextLoadContext *texts;
    PharaohTextSelection *text_selection;
    const PharaohSettingsDefaultNameDestinations *default_names;
    const char *primary_text_filename;
    const char *media_text_filename;
} PharaohApplicationConfiguration;

/* Ordered configuration phase from native 0x00413ED0. The historical optical
 * media probe is intentionally outside this prerequisite phase. */
PharaohApplicationConfigurationResult pharaoh_application_load_configuration(
    const PharaohApplicationConfiguration *configuration);

#endif
