#ifndef PHARAOH_RUNTIME_APPLICATION_STARTUP_H
#define PHARAOH_RUNTIME_APPLICATION_STARTUP_H

#include <stdint.h>

#include "pharaoh/runtime/application_media_probe.h"
#include "pharaoh/runtime/settings_default_names.h"
#include "pharaoh/runtime/settings_load.h"
#include "pharaoh/runtime/text_loading.h"

enum {
    PHARAOH_APPLICATION_DIRECTORY_BYTES = 100,
    PHARAOH_APPLICATION_DIRECTORY_QUERY_BYTES = 0x104,
    PHARAOH_APPLICATION_DIRECTORY_RANGE_ERROR = 0x22
};

typedef enum PharaohApplicationConfigurationResult {
    PHARAOH_APP_CONFIGURATION_READY = 0,
    PHARAOH_APP_CONFIGURATION_SETTINGS_FAILED = 1,
    PHARAOH_APP_CONFIGURATION_TEXT_FAILED = 2
} PharaohApplicationConfigurationResult;

typedef struct PharaohApplicationConfiguration {
    const PharaohSettingsLoadContext *settings;
    const PharaohTextLoadContext *texts;
    const PharaohStartupMediaDriveState *media_drive_state;
    const PharaohStartupMediaProbeOperations *media_probe_operations;
    uint8_t *detected_drive_letter;
    PharaohTextSelection *text_selection;
    const PharaohSettingsDefaultNameDestinations *default_names;
    const char *primary_text_filename;
    const char *media_text_filename;
} PharaohApplicationConfiguration;

typedef struct PharaohApplicationDirectoryState {
    char path[PHARAOH_APPLICATION_DIRECTORY_BYTES];
    uint32_t runtime_errno;
} PharaohApplicationDirectoryState;

typedef struct PharaohApplicationDirectoryOperations {
    void *user_data;
    uint32_t (*get_current_directory)(void *user_data,
                                      uint32_t buffer_capacity,
                                      char *buffer);
} PharaohApplicationDirectoryOperations;

/* Ordered settings/text startup phase observed in native 0x00413ED0. The
 * optional media probe's result is ignored; it only selects a drive letter. */
PharaohApplicationConfigurationResult pharaoh_application_load_configuration(
    const PharaohApplicationConfiguration *configuration);

/* Original entry 0x005623FC. Copies GetCurrentDirectoryA's result into the
   native 100-byte directory buffer when it fits; an oversized path sets the
   runtime range error and leaves the previous buffer bytes intact. */
void pharaoh_application_capture_current_directory(
    PharaohApplicationDirectoryState *state,
    const PharaohApplicationDirectoryOperations *operations);

#endif
