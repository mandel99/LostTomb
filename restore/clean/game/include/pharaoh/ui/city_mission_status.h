#ifndef PHARAOH_UI_CITY_MISSION_STATUS_H
#define PHARAOH_UI_CITY_MISSION_STATUS_H

#include "pharaoh/ui/ui_text_layout.h"
#include "pharaoh/ui/panel_frame.h"
#include "pharaoh/rendering/primitives.h"
#include "pharaoh/runtime/city_view.h"
#include "pharaoh/scenario/scenario_init_custom.h"

typedef struct PharaohCityMissionStatusContext {
    const PharaohUiTextLayoutContext *text;
    const PharaohBitmapFont *small_font, *normal_font;
    const int32_t *small_font_id, *normal_font_id;
    PharaohGraphicsPrimitives *graphics;
    const PharaohPanelFrameNativeContext *panel;
    const PharaohCityViewState *view;
    const PharaohScenarioPlayerRuntime *players;
    const int32_t *current_player;
    const int32_t *origin_x, *origin_y;
    const uint8_t *tomb_placement_006de514, *paused_00e38e60;
    const uint16_t *campaign_gate_00784b60;
    const int32_t *pixel_mode_00e92dfc;
    const int32_t *time_limit_00784c48, *survival_limit_00784c50;
    const int32_t *base_year_0078d7b8, *year_0078d7bc, *month_0078d7c0;
    const int32_t *allow_all_00605a0c, *debug_stats_00f53f70;
    const int32_t *event_kind_00605a00, *event_level_00605a04;
    const int32_t *event_id_00605a08;
} PharaohCityMissionStatusContext;

/* Complete typed C native 0x0051F790. Player DWORD views at +0x10,
   +0x44D0 and +0x46A8 are width observations, not new canonical field types. */
void pharaoh_city_draw_mission_status(const PharaohCityMissionStatusContext *);

#endif
