#include "pharaoh/runtime/application_startup.h"

#include <string.h>

PharaohApplicationConfigurationResult pharaoh_application_load_configuration(
    const PharaohApplicationConfiguration *c)
{
    if (pharaoh_settings_load(c->settings) == 0) {
        return PHARAOH_APP_CONFIGURATION_SETTINGS_FAILED;
    }
    if (pharaoh_load_text_catalogs(c->texts, c->primary_text_filename,
            c->media_text_filename) == 0) {
        return PHARAOH_APP_CONFIGURATION_TEXT_FAILED;
    }

    uint8_t detected_drive_letter = (uint8_t)'D';
    (void)pharaoh_application_probe_media_drive(c->media_drive_state,
        c->media_probe_operations, &detected_drive_letter);
    *c->detected_drive_letter = detected_drive_letter;

    pharaoh_settings_load_default_names(c->text_selection, c->default_names);
    return PHARAOH_APP_CONFIGURATION_READY;
}

void pharaoh_application_capture_current_directory(
    PharaohApplicationDirectoryState *state,
    const PharaohApplicationDirectoryOperations *operations)
{
    char current_directory[PHARAOH_APPLICATION_DIRECTORY_QUERY_BYTES];
    const uint32_t path_length = operations->get_current_directory(
        operations->user_data,
        PHARAOH_APPLICATION_DIRECTORY_QUERY_BYTES,
        current_directory);

    if (path_length == 0 ||
        path_length + 1 >= PHARAOH_APPLICATION_DIRECTORY_QUERY_BYTES + 1) {
        return;
    }

    const uint32_t path_bytes = path_length + 1;
    if (path_bytes > sizeof(state->path)) {
        state->runtime_errno = PHARAOH_APPLICATION_DIRECTORY_RANGE_ERROR;
        return;
    }

    memcpy(state->path, current_directory, path_bytes);
}
