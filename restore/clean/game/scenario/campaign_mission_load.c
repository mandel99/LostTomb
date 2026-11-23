#include "pharaoh/scenario/campaign_mission_load.h"

#include "pharaoh/runtime/file_directory_reset.h"

static int32_t load_package(void *user_data, int32_t package, int32_t skip)
{
    return pharaoh_campaign_mission_load(user_data, package, skip);
}

static void load_selected(void *user_data)
{
    PharaohCampaignMissionEntry *const entry = user_data;
    pharaoh_scenario_start_mission(entry->start, entry->scenario_name, 0);
    /* These fields are snapshots of the native shared window state. Reload
     * after the child, before briefing tests its early return to city0x01. */
    entry->briefing->current_window_id = *entry->start->current_window_id;
    entry->briefing->redraw_requested = *entry->start->window_redraw_requested;
    entry->briefing->difficulty = entry->start->settings->difficulty;
}

void pharaoh_campaign_mission_bind_entry(PharaohCampaignMissionEntry *entry,
    PharaohBriefingOperations *operations)
{
    entry->start->settings = entry->load->settings;
    entry->start->load_campaign_mission = load_package;
    entry->start->campaign_load_user_data = entry->load;
    operations->load_selected_mission = load_selected;
    operations->load_user_data = entry;
}

static void read_saved_state(void *user_data, int32_t file)
{
    PharaohCampaignMissionStateContext *const state = user_data;
    pharaoh_saved_game_state_read(state->read, file);
}

static void reset_floodplain(void *user_data)
{
    PharaohCampaignMissionStateContext *const state = user_data;
    pharaoh_floodplain_settings_initialize(state->floodplain_settings);
}

static void initialize_saved_state(void *user_data)
{
    PharaohCampaignMissionStateContext *const state = user_data;
    pharaoh_saved_game_initialize(state->initialize);
}

void pharaoh_campaign_mission_bind_state(
    PharaohCampaignMissionLoadContext *load,
    PharaohCampaignMissionLoadOperations *operations,
    PharaohCampaignMissionStateContext *state)
{
    load->operations = operations;
    state->read->settings = load->settings;
    state->read->floodplain_settings = state->floodplain_settings;
    state->initialize->file_version = &state->read->chunk_io->file_version;
    state->initialize->players = state->read->players;
    state->initialize->current_player = state->read->current_player;
    operations->user_data = state;
    operations->read_saved_game_state = read_saved_state;
    operations->reset_floodplain_settings = reset_floodplain;
    operations->initialize_saved_game = initialize_saved_state;
}

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
