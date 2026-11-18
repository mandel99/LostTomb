#include "pharaoh/runtime/application_startup_prepare.h"
#include "pharaoh/runtime/format_unbounded.h"

static const char startup_campaign_filename[] = "campaign.txt";
static const char startup_low_intro_filename[] = "Binks\\low\\intro.bik";
static const char startup_high_intro_filename[] = "Binks\\high\\intro_big.bik";
static const char startup_rectangle_format[] = "x=%d,y=%d,w=%d,h=%d";

enum {
    STARTUP_INTRO_LOW_HEIGHT_LIMIT = 0xe9,
    STARTUP_INTRO_CENTER_WIDTH = 0x1e0,
    STARTUP_INTRO_CENTER_HEIGHT = 0x11e
};

static PharaohApplicationStartupPreparationResult startup_result(
    PharaohApplicationStartupPreparationStatus status, int32_t native_result)
{
    PharaohApplicationStartupPreparationResult result = { status,
        native_result };
    return result;
}

static void report_startup_status(
    const PharaohApplicationStartupPreparationOperations *operations,
    PharaohApplicationStartupReport status, int32_t native_result)
{
    operations->report_status(operations->user_data, status, native_result);
}

static void reset_post_sound_state(
    const PharaohApplicationStartupPostSoundResetFields *fields)
{
    *fields->event_54_suppress_flood = 0;
    *fields->editor_map_preview = 0;
    *fields->unresolved_dword_1 = 2;
    *fields->unresolved_dword_2 = 0;
    *fields->unresolved_byte_1 = 0;
    *fields->construction_placing_royal_tomb = 0;
    *fields->cheat_allow_all = 0;
    *fields->unresolved_dword_3 = 0;
    *fields->unresolved_dword_4 = 1;
    *fields->unresolved_dword_5 = 0;
    *fields->unresolved_dword_6 = -1;
}

static void initialize_native_runtime_state(
    const PharaohApplicationStartupRuntimeContext *runtime)
{
    const PharaohApplicationStartupGeometry *const geometry =
        runtime->geometry;

    reset_post_sound_state(runtime->post_sound_reset);
    pharaoh_game_state_initialize(runtime->game_state);

    pharaoh_abu_simbel_part_geometry_initialize(
        geometry->abu_simbel_parts, geometry->projection_settings);
    pharaoh_library_part_geometry_initialize(geometry->library_parts,
        geometry->projection_settings);
    pharaoh_caesareum_part_geometry_initialize(geometry->caesareum_parts,
        geometry->projection_settings);
    pharaoh_lighthouse_part_geometry_initialize(
        geometry->lighthouse_parts, geometry->projection_settings);
    pharaoh_small_tomb_part_geometry_initialize(geometry->small_tomb_parts,
        geometry->projection_settings);
    pharaoh_medium_tomb_part_geometry_initialize(geometry->medium_tomb_parts,
        geometry->projection_settings);
    pharaoh_large_tomb_part_geometry_initialize(geometry->large_tomb_parts,
        geometry->projection_settings);
    pharaoh_grand_tomb_part_geometry_initialize(geometry->grand_tomb_parts,
        geometry->projection_settings);
    pharaoh_sun_temple_part_geometry_initialize(geometry->sun_temple_parts,
        geometry->projection_settings);
    pharaoh_mausoleum_projection_initialize(geometry->mausoleum);
    pharaoh_sidebar_init_positions(geometry->logical_screen_width,
        geometry->sidebar_positions);

    *runtime->current_window_id = 0x44;
    *runtime->redraw_requested = 1;
    pharaoh_platform_measure_speed(runtime->platform_measurement,
        runtime->platform_measurement_operations);
}

static void play_startup_intro(
    const PharaohApplicationStartupIntroContext *intro,
    PharaohVideoPlayer *player)
{
    if (intro->enabled != 0) {
        const uint32_t screen_width = intro->video_state->screen_width;
        const uint32_t screen_height = intro->video_state->screen_height;
        const char *const filename = screen_height <
            STARTUP_INTRO_LOW_HEIGHT_LIMIT ? startup_low_intro_filename :
            startup_high_intro_filename;
        const int32_t x = (int32_t)(screen_width -
            STARTUP_INTRO_CENTER_WIDTH) / 2;
        const int32_t y = (int32_t)(screen_height -
            STARTUP_INTRO_CENTER_HEIGHT) / 2;

        (void)pharaoh_video_player_play(player, intro->video_state,
            filename, x, y, intro->video_operations);
    }
}

static void format_startup_rectangle(
    const PharaohGraphicsWindowRect *rectangle)
{
    char description[100];
    const int32_t width = (int32_t)((uint32_t)rectangle->right -
        (uint32_t)rectangle->left);
    const int32_t height = (int32_t)((uint32_t)rectangle->bottom -
        (uint32_t)rectangle->top);

    (void)pharaoh_format_unbounded(description, startup_rectangle_format,
        rectangle->left, rectangle->top, width, height);
    (void)description;
}

PharaohApplicationStartupPreparationResult
pharaoh_application_startup_prepare_load(
    const PharaohApplicationStartupPreparationContext *context)
{
    const PharaohApplicationConfigurationResult configuration_result =
        pharaoh_application_load_configuration(context->configuration);

    if (configuration_result == PHARAOH_APP_CONFIGURATION_SETTINGS_FAILED) {
        report_startup_status(context->operations,
            PHARAOH_APPLICATION_STARTUP_REPORT_SETTINGS_FAILED,
            configuration_result);
        return startup_result(PHARAOH_APPLICATION_STARTUP_SETTINGS_FAILED,
            configuration_result);
    }
    if (configuration_result == PHARAOH_APP_CONFIGURATION_TEXT_FAILED) {
        report_startup_status(context->operations,
            PHARAOH_APPLICATION_STARTUP_REPORT_TEXT_FAILED,
            configuration_result);
        return startup_result(PHARAOH_APPLICATION_STARTUP_TEXT_FAILED,
            configuration_result);
    }
    report_startup_status(context->operations,
        PHARAOH_APPLICATION_STARTUP_REPORT_CONFIGURATION_READY,
        configuration_result);

    PharaohApplicationWindowStartupRequest window_request =
        *context->window_request;
    window_request.instance = context->instance;
    window_request.settings_display_mode =
        context->configuration->settings->settings->display_mode;
    window_request.window_title_selection =
        context->configuration->text_selection;

    if (pharaoh_application_window_initialize(context->window_state,
            context->graphics_state, &window_request,
            context->window_operations) == 0) {
        report_startup_status(context->operations,
            PHARAOH_APPLICATION_STARTUP_REPORT_WINDOW_FAILED, 0);
        context->operations->co_uninitialize(context->operations->user_data);
        return startup_result(PHARAOH_APPLICATION_STARTUP_WINDOW_FAILED, 0);
    }
    if (context->native_exception_filter_active != 0) {
        *context->native_exception_filter_active = 1;
    }
    report_startup_status(context->operations,
        PHARAOH_APPLICATION_STARTUP_REPORT_WINDOW_READY, 1);

    PharaohStartupAssetLoadContext asset_loading = *context->asset_loading;
    asset_loading.screen_width =
        (uint32_t)context->graphics_state->screen_width;
    asset_loading.screen_height =
        (uint32_t)context->graphics_state->screen_height;
    const int32_t asset_result = pharaoh_startup_asset_setup(&asset_loading);
    if (asset_result == 1 || asset_result == 2 || asset_result == 3 ||
        (uint32_t)asset_result == UINT32_C(0xfffffffe)) {
        report_startup_status(context->operations,
            PHARAOH_APPLICATION_STARTUP_REPORT_ASSETS_FAILED, asset_result);
        return startup_result(PHARAOH_APPLICATION_STARTUP_ASSETS_FAILED,
            asset_result);
    }
    report_startup_status(context->operations,
        PHARAOH_APPLICATION_STARTUP_REPORT_ASSETS_READY, asset_result);

    int32_t native_result =
        pharaoh_building_model_load(context->building_models);
    if (native_result == 0) {
        report_startup_status(context->operations,
            PHARAOH_APPLICATION_STARTUP_REPORT_BUILDING_MODELS_FAILED,
            native_result);
        return startup_result(
            PHARAOH_APPLICATION_STARTUP_BUILDING_MODELS_FAILED,
            native_result);
    }

    native_result = pharaoh_figure_model_load(context->figure_models);
    if (native_result == 0) {
        report_startup_status(context->operations,
            PHARAOH_APPLICATION_STARTUP_REPORT_FIGURE_MODELS_FAILED,
            native_result);
        return startup_result(
            PHARAOH_APPLICATION_STARTUP_FIGURE_MODELS_FAILED, native_result);
    }

    native_result = pharaoh_tax_sentiment_model_load(context->tax_model);
    if (native_result == 0) {
        report_startup_status(context->operations,
            PHARAOH_APPLICATION_STARTUP_REPORT_TAX_MODEL_FAILED,
            native_result);
        return startup_result(PHARAOH_APPLICATION_STARTUP_TAX_MODEL_FAILED,
            native_result);
    }

    native_result = pharaoh_campaign_load(context->campaign,
        startup_campaign_filename, context->campaign_operations);
    native_result = (int32_t)((uint32_t)native_result & UINT32_C(0xff));
    if (native_result == 0) {
        report_startup_status(context->operations,
            PHARAOH_APPLICATION_STARTUP_REPORT_CAMPAIGN_FAILED,
            native_result);
        return startup_result(PHARAOH_APPLICATION_STARTUP_CAMPAIGN_FAILED,
            native_result);
    }

    native_result = pharaoh_trade_recommendations_load(context->trade_model);
    if (native_result == 0) {
        report_startup_status(context->operations,
            PHARAOH_APPLICATION_STARTUP_REPORT_TRADE_MODEL_FAILED,
            native_result);
        return startup_result(
            PHARAOH_APPLICATION_STARTUP_TRADE_MODEL_FAILED, native_result);
    }

    native_result = pharaoh_event_reason_phrases_load(context->event_phrases);
    if (((uint32_t)native_result & UINT32_C(0xff)) == 0) {
        report_startup_status(context->operations,
            PHARAOH_APPLICATION_STARTUP_REPORT_EVENT_PHRASES_FAILED,
            native_result & 0xff);
        return startup_result(
            PHARAOH_APPLICATION_STARTUP_EVENT_PHRASES_FAILED,
            native_result & 0xff);
    }

    pharaoh_sound_initialize(context->sound);
    pharaoh_actor_walking_no_operation();
    report_startup_status(context->operations,
        PHARAOH_APPLICATION_STARTUP_REPORT_SOUND_READY, 1);
    initialize_native_runtime_state(context->runtime);
    *context->runtime->native_message_loop_state = 1;
    report_startup_status(context->operations,
        PHARAOH_APPLICATION_STARTUP_REPORT_MESSAGE_LOOP_READY, 1);
    format_startup_rectangle(context->runtime->startup_status_rectangle);
    play_startup_intro(context->intro, context->window_state->video_player);
    return startup_result(PHARAOH_APPLICATION_STARTUP_PREPARED, 1);
}
