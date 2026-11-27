#ifndef PHARAOH_UI_BUILDING_INFO_INDUSTRY_LIVE_H
#define PHARAOH_UI_BUILDING_INFO_INDUSTRY_LIVE_H

#include "pharaoh/ui/building_info_content_live.h"
#include "pharaoh/simulation/industry_queries.h"
#include "pharaoh/rendering/overlay_metrics.h"
#include "pharaoh/map/house_water.h"
#include "pharaoh/population/food_supply_query.h"

/* Actual WORD flags atEA4126, followed by the rest of each49D8 player bank.
 * Native consumers only test zero; signedness is not inferred from CMP. */
typedef struct PharaohIndustryMothballBank {
    uint16_t disabled[36];
    uint8_t reserved[0x49d8-72];
} PharaohIndustryMothballBank;

typedef struct PharaohInfoIndustryLiveContext {
    const PharaohInfoContentDrawContext *content;
    const PharaohFoodSupplyQueryContext *food;
    const PharaohMapHouseWaterContext *house_water;
    const PharaohIndustryMothballBank *mothball_banks;
    const uint32_t *current_player; /* actualEAAC28 */
    const int32_t *farm_progress_max; /* actualBCA34C */
    const int32_t *month,*flood_season_month; /*78D7C0/D386D0 */
    /* Actual56F5B8 schedule. Backing includes the four preceding DWORDs:
     * the Native non-farm/default crop -1 reads them without a guard. */
    const int32_t *harvest_months;
    const char *percent_suffix; /* actual5E03FC, percent sign and NUL */
} PharaohInfoIndustryLiveContext;

/* Full real drawing providers. All nested contexts borrow the same actual
 * 224-byte info state,264-byte building pool, city/map and text/image state.
 * text_width aliases text.drawing.cursor_x; actual fonts and framebuffer are
 * supplied by the existing StorageInfoDrawContext. No command emitters. */
void pharaoh_info_industry_draw_water_lift(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_water_supply(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_well(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_work_camp(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_farm_crop(const PharaohInfoIndustryLiveContext *,int32_t group);
void pharaoh_info_industry_draw_barley_farm(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_grain_farm(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_lettuce_farm(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_flax_farm(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_henna_farm(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_reed_gatherer(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_cattle_ranch(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_stone_quarry(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_gold_mine(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_copper_mine(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_gemstone_mine(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_clay_pit(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_brewery(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_weaver(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_weaponsmith(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_carpenters_guild(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_bricklayers_guild(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_stonemasons_guild(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_artisans_guild(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_hunting_lodge(const PharaohInfoIndustryLiveContext *);

/* Bind the already complete489240/508E00 industry_queries.c algorithms to
 * genuine current C queries and the complete raw-producer renderer below.
 * These adapters introduce no replacement Native query owners. */
PharaohIndustryQueryContext pharaoh_info_industry_query_context(const PharaohInfoIndustryLiveContext *);
void pharaoh_info_industry_draw_quarry(const PharaohInfoIndustryLiveContext *,int32_t resource);
void pharaoh_info_industry_draw_raw_producer(const PharaohInfoIndustryLiveContext *,
    int32_t help,int32_t resource,int32_t group,int32_t shows_stock);

#endif
