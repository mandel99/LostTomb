#include "pharaoh/scenario/campaign_mission_load.h"

#include "pharaoh/runtime/file_directory_reset.h"

enum {
    CAMPAIGN_PACKAGE_COUNT = 5,
    CAMPAIGN_PACKAGE_PATH_BYTES = 20,
    CAMPAIGN_MISSION_NONE = 0x7f,
    CAMPAIGN_OFFSET_BYTES = (int32_t)sizeof(int32_t),
    CAMPAIGN_READ_ORIGIN = 0,
    CAMPAIGN_OPEN_FLAGS = PHARAOH_FILE_OPEN_FLAGS_NATIVE_8000
};

static const char campaign_package_paths[CAMPAIGN_PACKAGE_COUNT]
    [CAMPAIGN_PACKAGE_PATH_BYTES] = {
        "mission1.pak",
        "mission2.pak",
        "mission3.pak",
        "mission4.pak",
        "mission5.pak"
    };

int32_t pharaoh_campaign_mission_load(
    const PharaohCampaignMissionLoadContext *context, int32_t package_index,
    int32_t skip_initialize)
{
    int32_t mission_offset;
    int32_t file_handle;
    int32_t difficulty;
    const char *package_path;
    const PharaohFileReadPartContext *const reader = context->file_read_part;

    if (package_index < 0 || package_index >= CAMPAIGN_PACKAGE_COUNT) {
        return 0;
    }
    difficulty = context->settings->difficulty;
    if (context->settings->mission_id == CAMPAIGN_MISSION_NONE) {
        return 0;
    }

    package_path = campaign_package_paths[package_index];
    pharaoh_file_directory_reset(reader->directory_reset);
    if (pharaoh_file_read_part(reader, package_path, &mission_offset,
            CAMPAIGN_OFFSET_BYTES,
            (int32_t)context->settings->mission_id * CAMPAIGN_OFFSET_BYTES) ==
        0) {
        return 0;
    }
    if (mission_offset < 1) {
        return 0;
    }

    file_handle = reader->files->operations->open_file(
        reader->files->user_data, package_path, CAMPAIGN_OPEN_FLAGS);
    if (file_handle == -1) {
        return 0;
    }
    (void)reader->operations->seek_file(reader->user_data, file_handle,
        mission_offset, CAMPAIGN_READ_ORIGIN);
    context->operations->read_saved_game_state(
        context->operations->user_data, file_handle);
    (void)reader->files->operations->close_file(reader->files->user_data,
        file_handle);

    context->settings->difficulty = difficulty;
    *context->player_cheats_used = 0;
    if (skip_initialize == 0) {
        context->operations->reset_floodplain_settings(
            context->operations->user_data);
        context->operations->initialize_saved_game(
            context->operations->user_data);
    }
    return 1;
}
