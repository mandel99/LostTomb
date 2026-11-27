#ifndef PHARAOH_BUILDING_CONSTRUCTION_BASIC_WARNINGS_H
#define PHARAOH_BUILDING_CONSTRUCTION_BASIC_WARNINGS_H
#include "pharaoh/runtime/city_warning.h"
#include "pharaoh/building/model_types.h"
#include "pharaoh/scenario/scenario_init_custom.h"
#include "pharaoh/map/tile_adjacency.h"
#include "pharaoh/population/food_supply_query.h"

typedef struct PharaohConstructionBasicWarnings {
    const uint32_t *editor_active;
    PharaohCityWarningContext *warnings;
    const PharaohBuildingModelRecord *models;
    const PharaohScenarioPlayerRuntime *players;
    const int32_t *current_player;
    const int32_t *granary_count,*booth_count,*bandstand_count,*pavilion_count,*fort_count;
    const PharaohMapTerrainView *map;
    const uint32_t *native_mode_784bbc;
    const PharaohFoodSupplyQueryContext *food_supply;
} PharaohConstructionBasicWarnings;
void pharaoh_construction_warning_workers(const PharaohConstructionBasicWarnings *,int32_t tool);
void pharaoh_construction_warning_granary(const PharaohConstructionBasicWarnings *,int32_t tool);
void pharaoh_construction_warning_booth(const PharaohConstructionBasicWarnings *,int32_t tool);
void pharaoh_construction_warning_bandstand(const PharaohConstructionBasicWarnings *,int32_t tool);
void pharaoh_construction_warning_pavilion(const PharaohConstructionBasicWarnings *,int32_t tool);
void pharaoh_construction_warning_fort(const PharaohConstructionBasicWarnings *,int32_t tool);
void pharaoh_construction_warning_recruiter(const PharaohConstructionBasicWarnings *,int32_t tool);
void pharaoh_construction_warning_well(const PharaohConstructionBasicWarnings *,int32_t tool,int32_t x,int32_t y);
/* Native004E3330, normal logical return is void. The status exposes native
 * signed-IDIV faults only; on fault the later percentage/post is not reached.
 * food_supply must share the same actual player selector and49D8 bank. */
PharaohPercentageScaleStatus pharaoh_construction_warning_food_supply(
    const PharaohConstructionBasicWarnings *,int32_t tool);
/* Native selectors stored at605A48. Only AL is consumed by482720. */
uint8_t pharaoh_construction_terrain_rule_00405b40(uint32_t terrain);
uint8_t pharaoh_construction_terrain_rule_00405b60(uint32_t terrain);
uint8_t pharaoh_construction_terrain_rule_00405b80(uint32_t terrain);
#endif
