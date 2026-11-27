#ifndef PHARAOH_UI_BUILDING_INFO_CONTENT_LIVE_H
#define PHARAOH_UI_BUILDING_INFO_CONTENT_LIVE_H

#include "pharaoh/ui/building_info_draw_live.h"
#include "pharaoh/building/occupancy_query.h"
#include "pharaoh/commerce/resource_supply.h"
#include "pharaoh/rendering/city_render.h"
#include "pharaoh/rendering/sprite_raw_blit.h"
#include "pharaoh/ui/building_info_init_support.h"
#include "pharaoh/ui/walker_portrait.h"
#include "pharaoh/commerce/trader_records.h"
#include "pharaoh/map/building_access_queries.h"
#include "pharaoh/population/house_economy.h"

typedef struct PharaohInfoPreviewContext {
    PharaohBuildingInfoLiveContext *ui;
    PharaohCityViewContext *view;
    PharaohCityRenderContext *render;
    const int32_t *grid_origin,*panel_position_mode;
    int32_t *projected_x,*projected_y; /* actual D3B4E4/E0 outputs */
    const PharaohSpriteImageRecord *images; /* Actual10D69F8, stride64 */
    const int32_t *image_count,*menu_bar_images; /* 126517C/F567E4 */
    const int32_t *capture_x,*capture_y; /* actual70E3E4/E0 */
    int32_t *draw_x,*draw_y;
    uint16_t *draw_width,*draw_height; /* D3B5BE/BC, WORD stores */
    uint8_t *surface_pixels;
    const int32_t *surface_pitch;
    uint8_t *preview_base;
    const uint32_t *preview_offsets; /* actual5F76E8, BYTE offsets */
} PharaohInfoPreviewContext;

/* Whole430E60,4FA330,4FA3C0. Render/view/viewport/clip/pass contexts share
 * the scene's real state. Each captured picture has48 rows of96 raw bytes. */
void pharaoh_info_render_selected_scene(PharaohCityRenderContext *,
    int32_t x,int32_t y,int32_t grid_origin,int32_t actor);
void pharaoh_info_prepare_walker_preview(const PharaohInfoPreviewContext *,
    int32_t grid,int32_t actor);
void pharaoh_info_prepare_walker_pictures(const PharaohInfoPreviewContext *);

typedef struct PharaohInfoContentDrawContext {
    const PharaohStorageInfoDrawContext *drawing;
    PharaohOccupancyState *occupancy; /* actual BCA350 child state */
    const PharaohInfoPreviewContext *previews;
} PharaohInfoContentDrawContext;

void pharaoh_info_draw_no_info(const PharaohInfoContentDrawContext *);
void pharaoh_info_draw_house_population(const PharaohInfoContentDrawContext *,int32_t y);
void pharaoh_info_draw_house_taxes(const PharaohInfoContentDrawContext *,int32_t y);
void pharaoh_info_draw_house_crime(const PharaohInfoContentDrawContext *,int32_t y);
void pharaoh_info_draw_farm_workers(const PharaohInfoContentDrawContext *,int32_t y);
void pharaoh_info_draw_mud_wall(const PharaohInfoContentDrawContext *);
void pharaoh_info_draw_brick_wall(const PharaohInfoContentDrawContext *);
void pharaoh_info_draw_strong_wall(const PharaohInfoContentDrawContext *);
void pharaoh_info_draw_statue(const PharaohInfoContentDrawContext *);
void pharaoh_info_draw_triumphal_arch(const PharaohInfoContentDrawContext *);
void pharaoh_info_draw_burning_ruin(const PharaohInfoContentDrawContext *);

typedef struct PharaohInfoWorstDesirabilityContext {
    PharaohBuildingInfoLiveContext *ui;
    const PharaohBuildingModelRecord *models; /* Actual705EA4 */
    const int16_t *building_ids;
    const int32_t *map_width,*map_height,*grid_origin;
    int32_t *worst_desirability_building; /* alias window.worst_desirability_building, EADCD4 */
    int32_t *worst_desirability_score; /* alias window.worst_desirability_score, EADCD8 */
    int32_t *distance_x,*distance_y; /* D3B4FC/F8 */
} PharaohInfoWorstDesirabilityContext;
void pharaoh_info_house_find_worst_desirability(const PharaohInfoWorstDesirabilityContext *);

typedef struct PharaohInfoEmpireCulture {
    int32_t reserved,luxury_name;
} PharaohInfoEmpireCulture;
typedef struct PharaohInfoHouseTradeContext {
    const PharaohEmpireSupplyContext *supply;
    const PharaohInfoEmpireCulture *cultures; /* actual5D2A38, stride8 */
    const int32_t *luxury_industry_count; /* actual78D728 */
    const int16_t *luxury_import_status; /* EA40BC + selected player*49D8 */
} PharaohInfoHouseTradeContext;
/* Whole50E3C0. Caller-owned16DWORD counters are read/updated, not cleared.
 * Original culture names must map into these16 slots after subtracting38. */
int32_t pharaoh_info_house_trade_goods(const PharaohInfoHouseTradeContext *,
    int32_t counts[16],int32_t imports);

typedef struct PharaohInfoWalkerContentContext {
    const PharaohInfoContentDrawContext *content;
    const PharaohInfoWalkerPortraitCodeContext *portrait_codes;
    const PharaohClimatePreyRecord *portrait_scripts; /* actual5E4C20,48-byte script rows */
    const PharaohWalkerPortraitImages *portrait_images;
    const PharaohBitmapFont *font_name;
    const int32_t *font_name_id; /* actual F56FBC */
    const PharaohEmpireSupplyContext *supply;
    const PharaohTraderRecord *traders; /* actual77AF00, stride88 */
    const uint8_t *classical_names; /* actualE39088 */
    PharaohImageButtonDrawState *image_buttons;
} PharaohInfoWalkerContentContext;

/* Whole4FAB20. All saved fields are read from the actual388-byte walker,
 * 264-byte building,144-byte formation,106-byte city and88-byte trader pools.
 * UI width shares the real text cursor; button drawing uses real sprite data. */
void pharaoh_info_draw_walker(const PharaohInfoWalkerContentContext *,int32_t walker);
void pharaoh_info_draw_walker_list(const PharaohInfoWalkerContentContext *);

typedef struct PharaohInfoTerrainContentContext {
    const PharaohInfoWalkerContentContext *walkers;
    const PharaohInfoWalkerSelectionVoiceContext *selection_voice;
} PharaohInfoTerrainContentContext;
void pharaoh_info_draw_terrain(const PharaohInfoTerrainContentContext *);

typedef struct PharaohInfoHouseContentContext {
    const PharaohInfoWalkerContentContext *walkers;
    const PharaohHouseEconomyRecord *houses; /* alias actual7999D8 pool */
    const PharaohHouseModel *models; /* actual70CDCC */
    const PharaohRoadAccessMap *roads;
    const PharaohInfoHouseTradeContext *trade;
    const uint8_t *list_or,*comma,*close_parenthesis; /* 5F7E74/5E0400/5F7E78 */
} PharaohInfoHouseContentContext;
void pharaoh_info_draw_vacant_lot(const PharaohInfoHouseContentContext *);
void pharaoh_info_draw_house(const PharaohInfoHouseContentContext *);

#endif
