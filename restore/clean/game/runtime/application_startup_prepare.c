#include "pharaoh/runtime/application_startup_prepare.h"

static const char startup_campaign_filename[] = "campaign.txt";

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

static void initialize_native_runtime_state(
    const PharaohApplicationStartupRuntimeContext *runtime)
{
    const PharaohApplicationStartupGeometry *const geometry =
        runtime->geometry;

    pharaoh_actor_walking_no_operation();
    runtime->operations->reset_post_sound_state(
        runtime->operations->user_data);
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
    initialize_native_runtime_state(context->runtime);
    report_startup_status(context->operations,
        PHARAOH_APPLICATION_STARTUP_REPORT_SOUND_READY, 1);
    return startup_result(PHARAOH_APPLICATION_STARTUP_PREPARED, 1);
}
