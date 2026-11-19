#ifndef PHARAOH_PLATFORM_WINDOWS_STARTUP_HOST_H
#define PHARAOH_PLATFORM_WINDOWS_STARTUP_HOST_H

#include "pharaoh/runtime/application_startup.h"
#include "pharaoh/runtime/application_window_startup.h"
#include "pharaoh/runtime/application_settings_shutdown.h"
#include "pharaoh/runtime/graphics_startup.h"
#include "pharaoh/runtime/startup_asset_loading.h"
#include "pharaoh/runtime/building_model_loading.h"
#include "pharaoh/runtime/figure_model_loading.h"
#include "pharaoh/runtime/tax_sentiment_model.h"
#include "pharaoh/runtime/trade_recommendations.h"
#include "pharaoh/runtime/event_reason_phrases.h"
#include "pharaoh/assets/sprite_libraries.h"
#include "pharaoh/platform/windows_bink.h"
#include "pharaoh/runtime/status_log_format.h"
#include "pharaoh/scenario/campaign_start.h"
#include "pharaoh/scenario/campaign_file.h"
#include "pharaoh/scenario/campaign_profile_load.h"
#include "pharaoh/ui/high_scores.h"
#include "pharaoh/ui/main_menu_open.h"
#include "pharaoh/ui/message_box.h"
#include "pharaoh/ui/player_selection.h"
#include "pharaoh/ui/custom_map_selection.h"
#include "pharaoh/platform/windows_startup_files.h"
#include "pharaoh/platform/windows_sound.h"

enum { PHARAOH_WINDOWS_DEFAULT_NAME_CAPACITY = 4096 };
enum {
    PHARAOH_WINDOWS_STARTUP_IMAGE_CAPACITY = 30000,
    PHARAOH_WINDOWS_STARTUP_TAG_CAPACITY = 0x394
};

typedef struct PharaohWindowsStartupHost {
    PharaohFileReadOperations file_operations;
    PharaohFileSizeOperations file_size_operations;
    PharaohTextFileOperations text_operations;
    PharaohSettingsIniOperations ini_operations;
    PharaohStartupMediaProbeOperations media_operations;
    PharaohStatusLogAppendOperations log_append_operations;
    PharaohStatusLogFormatOperations log_format_operations;
    PharaohStatusLogAppendState log_append_state;
    PharaohStatusLogFormatState log_format_state;
    PharaohWindowsStartupFileStatus file_status;
    PharaohFileReadContext files;
    PharaohFileSizeContext file_size;
    PharaohSettingsLoadContext settings_context;
    PharaohTextLoadContext text_context;
    PharaohApplicationConfiguration configuration;
    PharaohGraphicsStartupState graphics;
    PharaohGraphicsStartupOperations graphics_operations;
    PharaohApplicationDirectoryOperations directory_operations;
    PharaohApplicationWindowOperations window_operations;
    PharaohApplicationWindowStartupOperations window_startup_operations;
    PharaohApplicationWindowStartupState window_startup;
    PharaohGameDataLoadState game_data_state;
    PharaohGameDataLoadContext game_data;
    PharaohSpritePixelMemoryOperations sprite_memory_operations;
    PharaohSpritePixelLoadContext sprite_pixels;
    PharaohSpriteLibraryReadContext sprite_reader;
    PharaohSpriteLibraryLoadContext sprite_libraries;
    PharaohStartupAssetState asset_state;
    PharaohStartupAssetOperations startup_asset_operations;
    PharaohStartupAssetLoadContext startup_asset_context;
    PharaohBuildingModelOperations building_model_operations;
    PharaohBuildingModelLoadContext building_model_context;
    PharaohFigureModelOperations figure_model_operations;
    PharaohFigureModelLoadContext figure_model_context;
    PharaohTaxSentimentModelOperations tax_model_operations;
    PharaohTaxSentimentModelLoadContext tax_model_context;
    PharaohTradeModelOperations trade_model_operations;
    PharaohTradeModelLoadContext trade_model_context;
    PharaohCampaignFileOperations campaign_file_operations;
    PharaohEventReasonPhraseOperations event_reason_operations;
    PharaohEventReasonPhraseContext event_reason_context;
    PharaohEventReasonPhraseTable event_reason_table;
    PharaohBuildingModelTable building_models;
    PharaohHouseModelTable house_models;
    PharaohFigureModelTable figure_models;
    PharaohEnemyModelTable enemy_models;
    PharaohTaxSentimentModel tax_model;
    PharaohCampaign campaign;
    uint16_t campaign_profile_suffix[PHARAOH_CAMPAIGN_PROFILE_SUFFIX_WORDS];
    char startup_model_mismatch_text[160];
    PharaohSpriteLibraryResetState *loaded_libraries;
    PharaohSpriteImageRecord *sprite_images;
    PharaohSpriteTagRecord *sprite_tags;
    uint8_t map_info[PHARAOH_MAP_INFO_SETTINGS_BYTES];
    PharaohWindowsBink bink;
    PharaohWindowsSound sound;
    PharaohVideoPlayerBackend bink_player;
    PharaohVideoLoopOperations video_loop;
    PharaohVideoOpenOperations video_open;
    PharaohVideoBlitContext video_blit;
    PharaohGraphicsWindowRect video_screen_bounds;
    PharaohVideoRuntimeState video_runtime;
    uint32_t video_transition_active;
    uint8_t shutdown_requested;
    uint8_t startup_video_started;
    uint8_t current_window_id;
    uint8_t video_write_to_primary;
    PharaohCampaignIndex campaign_index;
    uint32_t city_state_words[PHARAOH_CAMPAIGN_CITY_STATE_STRIDE_WORDS];
    PharaohSettingsFile settings;
    PharaohSettingsMenuIndexes menu_indexes;
    PharaohTextCatalog text_catalog;
    PharaohTextSelection text_selection;
    PharaohStartupMediaDriveState media_drive;
    PharaohSettingsDefaultNameDestinations default_names;
    char default_name_1[PHARAOH_WINDOWS_DEFAULT_NAME_CAPACITY];
    char default_name_2[PHARAOH_WINDOWS_DEFAULT_NAME_CAPACITY];
    char default_name_3[PHARAOH_WINDOWS_DEFAULT_NAME_CAPACITY];
    char default_name_4[PHARAOH_WINDOWS_DEFAULT_NAME_CAPACITY];
    uint8_t campaign_status;
    uint8_t startup_finalization_flag;
    uint8_t detected_drive_letter;
} PharaohWindowsStartupHost;

/* Binds real Win32/CRT adapters to the native settings and text startup. */
PharaohApplicationConfigurationResult pharaoh_windows_startup_host_load(
    PharaohWindowsStartupHost *host);

/* Performs the native window and DirectDraw session setup after configuration.
   The clean preview intentionally uses windowed presentation at the saved
   display resolution. */
int32_t pharaoh_windows_startup_host_open_window(
    PharaohWindowsStartupHost *host, void *instance,
    PharaohApplicationWindowProcedure window_procedure);

/* Loads the startup DAT and SG3 assets with the typed game-data readers.
   Presentation of the loading screen remains a separate raster owner. */
int32_t pharaoh_windows_startup_host_load_assets(
    PharaohWindowsStartupHost *host);

/* Reads the native pre-intro model, campaign, trade and reason data while the
   final loading background is still presented. */
int32_t pharaoh_windows_startup_host_load_pre_intro_data(
    PharaohWindowsStartupHost *host);

/* Plays the native startup Bink after graphics and assets are initialized. */
int32_t pharaoh_windows_startup_host_play_intro(
    PharaohWindowsStartupHost *host);

/* Refreshes the primary-surface origin after native window-position messages. */
int32_t pharaoh_windows_startup_host_refresh_video_bounds(
    PharaohWindowsStartupHost *host);

/* Re-presents the last startup loading frame after a Win32 paint message. */
void pharaoh_windows_startup_host_present_loading(
    PharaohWindowsStartupHost *host);

void pharaoh_windows_startup_host_present_frame(
    PharaohWindowsStartupHost *host);

/* Debug-only F12 capture target: writes the fully composed 16-bit backbuffer
   as a BMP without changing the presentation path. */
int32_t pharaoh_windows_startup_host_capture_frame(
    PharaohWindowsStartupHost *host, const char *path);

/* Ends loading before UpdateWindow or a video loop can process paint messages. */
void pharaoh_windows_startup_host_finish_loading(
    PharaohWindowsStartupHost *host);

/* Presents the SG3 logo/front-menu background at the logical UI origin.
   Foreground menu widgets remain with their own renderer. */
void pharaoh_windows_startup_host_draw_boot_background(
    PharaohWindowsStartupHost *host, uint8_t window_id);

void pharaoh_windows_startup_host_draw_boot_foreground(
    PharaohWindowsStartupHost *host, int32_t focused_button);
void pharaoh_windows_startup_host_draw_player_selection(
    PharaohWindowsStartupHost *host,
    const PharaohPlayerSelectionState *selection, int32_t focused_button);
void pharaoh_windows_startup_host_draw_player_name_input(
    PharaohWindowsStartupHost *host,
    const PharaohPlayerSelectionState *selection,
    int32_t list_button_focused);
void pharaoh_windows_startup_host_draw_player_name_choices(
    PharaohWindowsStartupHost *host,
    const PharaohPlayerSelectionState *selection, int32_t focused_button);
void pharaoh_windows_startup_host_draw_family_message(
    PharaohWindowsStartupHost *host, int32_t text_entry,
    int32_t button_count, int32_t focused_button);
void pharaoh_windows_startup_host_draw_main_menu(
    PharaohWindowsStartupHost *host, const PharaohMainMenuWindowState *state,
    int32_t focused_button);
void pharaoh_windows_startup_host_draw_custom_map_list(
    PharaohWindowsStartupHost *host,
    const PharaohCustomMapSelectionState *selection, int32_t campaign_mode);
void pharaoh_windows_startup_host_draw_saved_game_list(
    PharaohWindowsStartupHost *host, const PharaohFileList *files,
    int32_t first_visible, int32_t selected_row);
int32_t pharaoh_windows_startup_host_draw_message_box(
    PharaohWindowsStartupHost *host, PharaohMessageBoxState *state,
    const char *title, const char *body, uint32_t type);
void pharaoh_windows_startup_host_bind_high_scores(
    PharaohWindowsStartupHost *host, PharaohHighScoresOperations *operations);

#endif
