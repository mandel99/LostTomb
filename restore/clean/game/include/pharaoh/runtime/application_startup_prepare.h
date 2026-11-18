#ifndef PHARAOH_RUNTIME_APPLICATION_STARTUP_PREPARE_H
#define PHARAOH_RUNTIME_APPLICATION_STARTUP_PREPARE_H

#include "pharaoh/runtime/application_startup.h"
#include "pharaoh/runtime/application_window_startup.h"
#include "pharaoh/runtime/game_state_initialization.h"
#include "pharaoh/runtime/platform_measure_speed.h"
#include "pharaoh/runtime/building_model_loading.h"
#include "pharaoh/runtime/event_reason_phrases.h"
#include "pharaoh/runtime/figure_model_loading.h"
#include "pharaoh/runtime/sound_initialization.h"
#include "pharaoh/runtime/startup_asset_loading.h"
#include "pharaoh/runtime/tax_sentiment_model.h"
#include "pharaoh/runtime/trade_recommendations.h"
#include "pharaoh/runtime/video_player.h"
#include "pharaoh/scenario/campaign_file.h"
#include "pharaoh/monuments/mausoleum_projection.h"
#include "pharaoh/monuments/startup_geometry.h"
#include "pharaoh/ui/sidebar_layout.h"

typedef enum PharaohApplicationStartupPreparationStatus {
    PHARAOH_APPLICATION_STARTUP_PREPARED,
    PHARAOH_APPLICATION_STARTUP_SETTINGS_FAILED,
    PHARAOH_APPLICATION_STARTUP_TEXT_FAILED,
    PHARAOH_APPLICATION_STARTUP_WINDOW_FAILED,
    PHARAOH_APPLICATION_STARTUP_ASSETS_FAILED,
    PHARAOH_APPLICATION_STARTUP_BUILDING_MODELS_FAILED,
    PHARAOH_APPLICATION_STARTUP_FIGURE_MODELS_FAILED,
    PHARAOH_APPLICATION_STARTUP_TAX_MODEL_FAILED,
    PHARAOH_APPLICATION_STARTUP_CAMPAIGN_FAILED,
    PHARAOH_APPLICATION_STARTUP_TRADE_MODEL_FAILED,
    PHARAOH_APPLICATION_STARTUP_EVENT_PHRASES_FAILED
} PharaohApplicationStartupPreparationStatus;

typedef enum PharaohApplicationStartupReport {
    PHARAOH_APPLICATION_STARTUP_REPORT_CONFIGURATION_READY,
    PHARAOH_APPLICATION_STARTUP_REPORT_WINDOW_READY,
    PHARAOH_APPLICATION_STARTUP_REPORT_ASSETS_READY,
    PHARAOH_APPLICATION_STARTUP_REPORT_SOUND_READY,
    PHARAOH_APPLICATION_STARTUP_REPORT_MESSAGE_LOOP_READY,
    PHARAOH_APPLICATION_STARTUP_REPORT_SETTINGS_FAILED,
    PHARAOH_APPLICATION_STARTUP_REPORT_TEXT_FAILED,
    PHARAOH_APPLICATION_STARTUP_REPORT_WINDOW_FAILED,
    PHARAOH_APPLICATION_STARTUP_REPORT_ASSETS_FAILED,
    PHARAOH_APPLICATION_STARTUP_REPORT_BUILDING_MODELS_FAILED,
    PHARAOH_APPLICATION_STARTUP_REPORT_FIGURE_MODELS_FAILED,
    PHARAOH_APPLICATION_STARTUP_REPORT_TAX_MODEL_FAILED,
    PHARAOH_APPLICATION_STARTUP_REPORT_CAMPAIGN_FAILED,
    PHARAOH_APPLICATION_STARTUP_REPORT_TRADE_MODEL_FAILED,
    PHARAOH_APPLICATION_STARTUP_REPORT_EVENT_PHRASES_FAILED
} PharaohApplicationStartupReport;

typedef struct PharaohApplicationStartupPreparationResult {
    PharaohApplicationStartupPreparationStatus status;
    int32_t native_result;
} PharaohApplicationStartupPreparationResult;

typedef struct PharaohApplicationStartupPreparationOperations {
    void *user_data;
    void (*report_status)(void *user_data,
        PharaohApplicationStartupReport status, int32_t native_result);
    void (*co_uninitialize)(void *user_data);
} PharaohApplicationStartupPreparationOperations;

typedef struct PharaohApplicationStartupGeometry {
    PharaohProjectionRecord *abu_simbel_parts;
    PharaohProjectionRecord *library_parts;
    PharaohProjectionRecord *caesareum_parts;
    PharaohProjectionRecord *lighthouse_parts;
    PharaohProjectionRecord *small_tomb_parts;
    PharaohProjectionRecord *medium_tomb_parts;
    PharaohProjectionRecord *large_tomb_parts;
    PharaohProjectionRecord *grand_tomb_parts;
    PharaohProjectionRecord *sun_temple_parts;
    const PharaohProjectionSettings *projection_settings;
    PharaohMausoleumProjectionContext *mausoleum;
    uint32_t logical_screen_width;
    PharaohSidebarClipPositions *sidebar_positions;
} PharaohApplicationStartupGeometry;

typedef struct PharaohApplicationStartupPostSoundResetFields {
    int32_t *event_54_suppress_flood;
    int32_t *editor_map_preview;
    /* The following slots preserve native store order and width. Their
       semantic names/overlays remain unresolved. */
    int32_t *unresolved_dword_1;
    int32_t *unresolved_dword_2;
    uint8_t *unresolved_byte_1;
    uint8_t *construction_placing_royal_tomb;
    int32_t *cheat_allow_all;
    int32_t *unresolved_dword_3;
    int32_t *unresolved_dword_4;
    int32_t *unresolved_dword_5;
    int32_t *unresolved_dword_6;
} PharaohApplicationStartupPostSoundResetFields;

typedef struct PharaohApplicationStartupRuntimeContext {
    const PharaohApplicationStartupPostSoundResetFields *post_sound_reset;
    const PharaohGameStateInitContext *game_state;
    const PharaohApplicationStartupGeometry *geometry;
    int32_t *current_window_id;
    uint8_t *redraw_requested;
    /* Native byte state is set to 1 after speed measurement and before the
       "launching game loop" status. Its initial 0 store belongs to WinMain. */
    uint8_t *native_message_loop_state;
    const PharaohGraphicsWindowRect *startup_status_rectangle;
    PharaohPlatformMeasurementState *platform_measurement;
    const PharaohPlatformMeasurementOperations *platform_measurement_operations;
} PharaohApplicationStartupRuntimeContext;

typedef struct PharaohApplicationStartupIntroContext {
    int32_t enabled;
    PharaohVideoRuntimeState *video_state;
    const PharaohVideoOpenOperations *video_operations;
} PharaohApplicationStartupIntroContext;

typedef struct PharaohApplicationStartupPreparationContext {
    void *instance;
    const PharaohApplicationConfiguration *configuration;
    PharaohApplicationWindowStartupState *window_state;
    PharaohGraphicsStartupState *graphics_state;
    const PharaohApplicationWindowStartupRequest *window_request;
    const PharaohApplicationWindowStartupOperations *window_operations;
    const PharaohStartupAssetLoadContext *asset_loading;
    PharaohBuildingModelLoadContext *building_models;
    PharaohFigureModelLoadContext *figure_models;
    PharaohTaxSentimentModelLoadContext *tax_model;
    PharaohCampaign *campaign;
    const PharaohCampaignFileOperations *campaign_operations;
    PharaohTradeModelLoadContext *trade_model;
    const PharaohEventReasonPhraseContext *event_phrases;
    const PharaohSoundInitializationContext *sound;
    const PharaohApplicationStartupRuntimeContext *runtime;
    const PharaohApplicationStartupIntroContext *intro;
    const PharaohApplicationStartupPreparationOperations *operations;
    /* WinMain's EH3 filter is disabled until window initialization succeeds. */
    volatile int32_t *native_exception_filter_active;
} PharaohApplicationStartupPreparationContext;

/* Composes the observed 0x00413ED0 startup through optional intro playback.
   The outer message/frame loop, teardown, and host wiring remain outside. */
PharaohApplicationStartupPreparationResult
pharaoh_application_startup_prepare_load(
    const PharaohApplicationStartupPreparationContext *context);

#endif
