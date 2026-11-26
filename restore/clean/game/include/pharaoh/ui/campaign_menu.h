#ifndef PHARAOH_UI_CAMPAIGN_MENU_H
#define PHARAOH_UI_CAMPAIGN_MENU_H

#include "pharaoh/runtime/timed_window_transition.h"
#include "pharaoh/runtime/text_loading.h"
#include "pharaoh/ui/briefing.h"
#include "pharaoh/ui/bitmap_font.h"
#include "pharaoh/ui/label_text.h"
#include "pharaoh/ui/main_menu.h"
#include "pharaoh/ui/token_expansion.h"
#include "pharaoh/rendering/image_draw.h"
#include "pharaoh/scenario/campaign_start.h"

#include <stdint.h>

typedef struct PharaohCampaignMenuState {
    uint8_t *current_window_id;
    uint8_t *redraw_requested;
    uint32_t *selected_period;
    uint32_t *latest_period;
    uint32_t *campaign_mode;
    uint32_t *map_changed;
    uint32_t *returns_to_front_menu;
    uint8_t *selection_flag;
    uint32_t *map_mode;
    uint32_t *campaign_started;
    int8_t *mission_id;
    uint8_t *selection_kind;
    uint32_t *scenario_mode;
} PharaohCampaignMenuState;

typedef struct PharaohCampaignMenuRenderRecord {
    PharaohMenuRect rect;
    int16_t unknown_08_19[6];
    int32_t period_id;
    int16_t unknown_24_27[2];
} PharaohCampaignMenuRenderRecord;

typedef struct PharaohCampaignMenuDrawContext {
    PharaohCampaignMenuState *menu_state;
    PharaohImageDrawContext *image_draw;
    PharaohTextSelection *text_selection;
    const char **current_text_00e20768;
    const PharaohTokenExpansionEntry *token_entries;
    const PharaohTokenExpansionOperations *token_operations;
    const uint8_t *glyph_map;
    PharaohBitmapTextDrawState *text_draw_state;
    PharaohRichTextState *wrapped_word_metrics;
    uint8_t *line_buffer_00e3439c;
    int32_t *maximum_width_00e2076c;
    const PharaohBitmapFont *font_00f56fb4;
    const PharaohBitmapFont *font_00f56fbc;
    const PharaohBitmapFont *font_00f56f94;
    const PharaohBitmapFont *font_00f56f8c;
    const PharaohBitmapFont *font_00f56fa4;
    int32_t *center_offset_00e20770;
    uint32_t *reset_flag_00d3b504;
    const PharaohMenuRenderer *menu_renderer;
    const PharaohLabelRenderer *label_renderer;
    const PharaohCampaignMenuRenderRecord *geometry_0057be78;
    uint8_t *menu_redraw_flag_00e38dc9;
    uint8_t *window_redraw_request;
    uint32_t *text_format_mode_00784f0c;
    int32_t *screen_origin_x_00e38da8;
    int32_t *screen_origin_y_00e38da4;
    int32_t *screen_width_00e38db0;
    int32_t *screen_height_00e38dac;
    int32_t *selected_row_007056c8;
    int32_t *title_image_base_00f55c94;
    int32_t *period_image_base_00f57314;
    int32_t *font_id_00f56fb4;
    int32_t *font_id_00f56fbc;
    int32_t *font_id_00f56f94;
    int32_t *font_id_00f56f8c;
    int32_t *font_id_00f56fa4;
    int32_t *layout_x_00d3b56c;
    int32_t *layout_y_00d3b568;
    int32_t *layout_width_00d3b574;
    int32_t *layout_height_00d3b570;
} PharaohCampaignMenuDrawContext;

_Static_assert(sizeof(PharaohCampaignMenuRenderRecord) == 0x1c,
    "native campaign menu geometry stride");

typedef struct PharaohCampaignPeriodVideo {
    const char *path;
    int32_t x, y;
} PharaohCampaignPeriodVideo;

/* Inlined video selection from callback 0x0041BB70. */
int32_t pharaoh_campaign_menu_period_video(uint32_t period, int32_t cpu,
    int32_t screen_width, int32_t screen_height, PharaohCampaignPeriodVideo *video);

typedef struct PharaohCampaignMenuOperations {
    void *user_data;
    void (*show_individual_missions)(void *user_data);
    void (*play_period_intro)(void *user_data, uint32_t period);
    void (*start_campaign)(void *user_data, int8_t period);
    void (*begin_briefing)(void *user_data);
} PharaohCampaignMenuOperations;

typedef struct PharaohCampaignBriefingContext {
    const PharaohCampaignIndex *index;
    const PharaohSettingsFile *settings;
    uint32_t campaign_mode;
    PharaohTimedWindowTransitionStartContext transition;
    PharaohBriefingState *briefing;
} PharaohCampaignBriefingContext;

/* Complete typed C translations of native 0x0041BA00, 0x0041BAD0 and
   0x0041BB30. */
int32_t pharaoh_campaign_menu_period_valid(int32_t period);
uint32_t pharaoh_campaign_menu_period_lock_mask(uint32_t period);
int32_t pharaoh_campaign_menu_image_button_count(
    uint32_t campaign_mode, uint32_t selected_period,
    uint32_t latest_period);
/* State helper used by the reconstructed period-button callbacks. */
void pharaoh_campaign_menu_select_period(
    PharaohCampaignMenuState *state, uint32_t period);

/* Typed logical callbacks 0x0041C310 and 0x0041C350. */
int32_t pharaoh_campaign_menu_open_individual_missions(
    PharaohCampaignMenuState *state,
    const PharaohCampaignMenuOperations *operations);
int32_t pharaoh_campaign_menu_open_campaigns(
    PharaohCampaignMenuState *state);

/* Complete typed translation of Pharaoh.exe 0x0041BE10. */
void pharaoh_campaign_menu_draw(const PharaohCampaignMenuDrawContext *context);

/* Complete typed translation of Pharaoh.exe 0x0041C2D0. */
void pharaoh_campaign_menu_draw_background(PharaohImageDrawContext *drawing,
    int32_t image, int32_t origin_x, int32_t origin_y,
    int32_t *loading_progress, uint8_t *menu_redraw);

/* Typed logical callback 0x0041BB70 with explicit child operations. */
int32_t pharaoh_campaign_menu_start_selected(
    PharaohCampaignMenuState *state,
    const PharaohCampaignMenuOperations *operations);

/* Complete typed C translations of native 0x00418140 and 0x004181E0. */
int32_t pharaoh_campaign_briefing_message_id(
    const PharaohCampaignIndex *index, const PharaohSettingsFile *settings,
    uint32_t campaign_mode);
void pharaoh_campaign_begin_briefing(
    const PharaohCampaignBriefingContext *context);

#endif
