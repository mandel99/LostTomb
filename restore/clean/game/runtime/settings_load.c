#include "pharaoh/runtime/settings_load.h"

#include "pharaoh/runtime/file_loading.h"

#include <windows.h>

int32_t pharaoh_settings_load(const PharaohSettingsLoadContext *context)
{
    PharaohSettingsFile *settings = context->settings;
    const PharaohSettingsIniContext ini_context = {
        context->files,
        context->ini_operations,
        context->ini_user_data,
        settings
    };

    settings->desktop_color_depth = 16;
    settings->fullscreen = 1;
    settings->paused = 0;
    settings->unused_a = 0;
    settings->unused_b = 2;
    settings->ram = 15;
    settings->cpu = 66;
    settings->unused_c = 0;
    settings->cd_drive_letter = (uint8_t)'D';
    settings->sound_effects_enabled = 1;
    settings->sound_music_enabled = 1;
    settings->sound_speech_enabled = 1;
    settings->sound_city_enabled = 1;
    settings->warnings = 1;
    settings->tooltips = 2;
    settings->game_speed = 90;
    settings->scroll_speed = 70;
    settings->check_cd = 1;
    settings->difficulty = 2;
    settings->gods_enabled = 1;
    settings->sound_effects_volume = 100;
    settings->sound_music_volume = 80;
    settings->sound_speech_volume = 100;
    settings->sound_city_volume = 100;
    settings->display_mode = 1;

    (void)pharaoh_settings_read_ini(&ini_context);
    if (settings->ram < PHARAOH_SETTINGS_MINIMUM_RAM_FOR_DEFAULT_SPEED) {
        settings->game_speed = 100;
        settings->scroll_speed = 100;
        settings->display_mode = 1;
    }

    const int32_t screen_width = GetSystemMetrics(
        PHARAOH_SCREEN_METRIC_PRIMARY_WIDTH);
    if (screen_width < PHARAOH_SETTINGS_SCREEN_WIDTH_LOW) {
        settings->display_mode = 1;
    } else if (screen_width < PHARAOH_SETTINGS_SCREEN_WIDTH_MEDIUM) {
        settings->display_mode = 2;
    }
    settings->display_mode_start = settings->display_mode;

    PharaohCampaignStartContext campaign = context->campaign;
    campaign.settings = settings;
    pharaoh_settings_start_campaign(&campaign, 0);
    settings->last_advisor = 1;
    (void)pharaoh_load_raw_file(context->files, "Pharaoh.inf", settings);
    settings->check_cd = 1;
    (void)pharaoh_settings_read_ini(&ini_context);
    *context->startup_finalization_flag = 1;
    settings->paused = 0;

    if (settings->tooltips == 2) {
        context->menu_indexes->tooltips_primary = 4;
        context->menu_indexes->tooltips_secondary = 4;
    } else if (settings->tooltips == 1) {
        context->menu_indexes->tooltips_primary = 3;
        context->menu_indexes->tooltips_secondary = 3;
    }
    if (settings->warnings == 1) {
        context->menu_indexes->warnings = 6;
    }
    context->menu_indexes->monthly_autosave =
        (settings->autosave != 0) + 9;
    context->menu_indexes->pyramid_speedup =
        (settings->pyramid_speedup != 0) + 4;
    context->menu_indexes->classical_names =
        (settings->classical_names != 0) + 7;
    return 1;
}
