#ifndef PHARAOH_UI_DEBUG_INFO_H
#define PHARAOH_UI_DEBUG_INFO_H

#include "pharaoh/ui/ui_text_layout.h"
#include "pharaoh/ui/panel_blocks.h"
#include "pharaoh/runtime/city_view.h"
#include "pharaoh/walker/record_queries.h"
#include "pharaoh/scenario/scenario_init_custom.h"
#include "pharaoh/building/type_queries.h"
#include "pharaoh/building/model_types.h"
#include "pharaoh/ui/debug_building_types.h"

typedef struct PharaohDebugInfoText {
    const PharaohUiTextLayoutContext *ui;
    const PharaohBitmapTextCenteredNativeContext *centered;
    const PharaohBitmapFont *small_font;
    const int32_t *small_image, *normal_image, *origin_x, *origin_y;
    const PharaohPanelBlocksContext *panel;
} PharaohDebugInfoText;

typedef struct PharaohDebugEvolutionContext {
    const PharaohDebugInfoText *text;
    const int32_t *enabled, *tick, *day, *month, *year;
    const int32_t *building_bound, *buildings_ever;
    const int32_t *connection_count, *total_connections, *error;
    const int32_t *routings, *barbarians;
} PharaohDebugEvolutionContext;

typedef struct PharaohDebugWalkerContext {
    const PharaohDebugInfoText *text;
    const int32_t *enabled, *count, *ever;
    int32_t *selected_grid, *watched;
    const int16_t *walker_heads, *route_slots;
    const PharaohWalkerQueryRecord *walkers; /* two thousand native records */
    const PharaohCityViewContext *city;
    PharaohCityPixelHit *hit;
    const int32_t *mouse_x, *mouse_y;
} PharaohDebugWalkerContext;

typedef union PharaohDebugCityPlayer {
    PharaohScenarioPlayerRuntime scenario;
    PharaohHousePopulationPlayerMetrics houses;
    struct {
        uint8_t reserved_00[2], active;
        uint8_t reserved_03[0x24-3];
        int32_t population;
        uint8_t reserved_28[0x34-0x28];
        int32_t capacity, spare_capacity;
        uint8_t reserved_3c[0x26dc-0x3c];
        int32_t births, deaths;
        uint8_t reserved_26e4[0x4460-0x26e4];
        int16_t docks[10];
        uint8_t reserved_4474[0x4594-0x4474];
        int32_t natives_restless;
        uint8_t reserved_4598[4];
        int32_t mission;
        uint8_t reserved_45a0[0x46a4-0x45a0];
        int32_t wine_sources;
        uint8_t reserved_46a8[0x49d8-0x46a8];
    };
} PharaohDebugCityPlayer;

typedef struct PharaohDebugCityContext {
    const PharaohDebugInfoText *text;
    const int32_t *enabled, *current_player, *highest_building;
    PharaohDebugCityPlayer *players; /* native indices0..8 */
    const PharaohBuildingTypeRecord *buildings;
    const int32_t *dock_count, *orientation, *map_style;
    const int32_t *was_in_int, *was_in_set, *alert_count, *last_alert;
    const int32_t *removed_tiles, *test1, *test2, *test3, *test4;
    const int32_t *mouse_x, *mouse_y, *mouse_movement;
    const int32_t *sidebar_x, *building_in_progress;
} PharaohDebugCityContext;

typedef struct PharaohDebugBuildingContext {
    const PharaohDebugInfoText *text;
    const int32_t *enabled, *mouse_x, *mouse_y, *grid_overlay;
    const PharaohCityViewContext *city;
    PharaohCityPixelHit *hit;
    int32_t *selected_grid;
    const int16_t *building_ids, *undo_ids;
    const PharaohDebugBuilding *buildings, *undo_buildings;
    const PharaohHouseModel *models;
    const int32_t *undo_ready, *undo_available, *undo_count, *direction_deltas;
    const int8_t *water_routing;
    const uint8_t *redraw;
} PharaohDebugBuildingContext;

/* Native00534DF0/00536120. All font/text/glyph/panel slots must be shared
 * native state. No snapshot counters or substitute drawing methods. */
void pharaoh_debug_evolution_draw(const PharaohDebugEvolutionContext *context);
void pharaoh_debug_walker_draw(const PharaohDebugWalkerContext *context);
void pharaoh_debug_city_draw(const PharaohDebugCityContext *context);
void pharaoh_debug_building_draw(const PharaohDebugBuildingContext *context);

#endif
