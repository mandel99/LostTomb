#ifndef PHARAOH_UI_BUILDING_INFO_MONUMENTS_LIVE_H
#define PHARAOH_UI_BUILDING_INFO_MONUMENTS_LIVE_H

#include "pharaoh/ui/building_info_services_live.h"
#include "pharaoh/ui/building_info_industry_live.h"
#include "pharaoh/military/shipwright_demand.h"
#include "pharaoh/map/road_access.h"
#include "pharaoh/monuments/progress_reports.h"
#include "pharaoh/monuments/base_progress.h"
#include "pharaoh/monuments/definitions.h"
#include "pharaoh/monuments/stages.h"
#include "pharaoh/building/kingdom_access.h"
#include "pharaoh/building/sphinx_queries.h"
#include "pharaoh/ui/token_expansion.h"

typedef struct PharaohInfoMonumentPlayerRecord {
    uint8_t reserved_0000[0x2a88];
    int32_t stockpiled[36];             /* EA42F8, DWORD zero tests */
    uint8_t reserved_2b18[0x433a-0x2b18];
    int16_t sphinx_stage;               /* EA5BAA, signed WORD */
    uint8_t reserved_433c[0x473c-0x433c];
    int32_t monument_buildings[3];      /* EA5FAC, DWORD IDs */
    uint8_t reserved_4748[0x48f0-0x4748];
    int32_t available[36];              /* EA6160, signed DWORD */
    uint8_t reserved_4980[0x49d8-0x4980];
} PharaohInfoMonumentPlayerRecord;
typedef struct PharaohInfoMonumentTextResources {
    int32_t second,first; /* actual EADB30/EADB34 */
} PharaohInfoMonumentTextResources;
typedef struct PharaohInfoScriptedMonumentReport {
    int32_t stage,stonemasons,stonemasonsWaiting,laborers,carpenters;
    int32_t material,percent,materialTotal;
} PharaohInfoScriptedMonumentReport;
typedef struct PharaohInfoLibraryProgress {
    int32_t stage,reserved[6],stonemasons,stonemasonsWaiting,laborers,carpenters;
} PharaohInfoLibraryProgress;
typedef struct PharaohInfoLibraryStock { int32_t material,percent,materialTotal; } PharaohInfoLibraryStock;
typedef struct PharaohInfoLibraryReport {
    PharaohInfoLibraryProgress progress;
    PharaohInfoLibraryStock stock;
} PharaohInfoLibraryReport;
typedef struct PharaohInfoAbuSimbelReport { int32_t stage,reserved[2],percent; } PharaohInfoAbuSimbelReport;
typedef struct PharaohInfoMausoleumReport {
    int32_t status,partsAtStage0,missingSubtiles,partsBelowStage2,stonemasons;
    int32_t stonemasonsWaiting,laborers,material,percent,materialTotal,carpenters,blocksLeft;
} PharaohInfoMausoleumReport;

typedef struct PharaohInfoMonumentsLiveContext {
    const PharaohStorageInfoDrawContext *drawing;
    const PharaohInfoServicesLiveContext *services;
    const PharaohInfoIndustryLiveContext *industry;
    PharaohRoadAccessContext *road_access;
    const int32_t *map_sweep_countdown; /* actual78D7B0 */
    const PharaohInfoMonumentPlayerRecord *players; /* actual EA1870/49D8 */
    const uint32_t *current_player; /* actual EAAC28, shared with services/industry */
    PharaohPyramidSummaryContext *pyramid;
    PharaohComplexProgressContext *complex;
    PharaohBaseMonumentProgress *library_progress;
    PharaohBuildingAccessContext *kingdom_access;
    const PharaohMonumentDefinition *definitions; /* actual5D19C8,16B */
    const int16_t *goal_definition_ids; /* actual784DE0 */
    const int32_t *building_count_total,*building_count_active; /*705AF0/70573C */
    const int32_t *flood_state; /* actualD386D4, values1/2/3 mean running */
    /* Actual EE0C50 plus the BYTE offset read from BCA2D8. One land-citizen
     * routing byte per4B cell; indices here are x+228*y without that origin. */
    const int8_t *land_citizen;
    const int16_t *tile_building_ids;
    const PharaohSphinxAssignment *sphinx_assignments; /* actual16B stage/type/pointer/reserved rows */
    const PharaohSphinxWorkSpot *sphinx_percent_spots; /* same actual16B rows, first8B view */
    const PharaohMonumentWorkAssignment *sun_temple_spots,*library_spots,*abu_simbel_spots;
    const PharaohMonumentWorkAssignment *caesareum_spots,*lighthouse_spots;
    const PharaohMonumentWorkAssignment *small_obelisk_spots,*large_obelisk_spots;
    /* Immutable projection of each real16B obelisk row's type/assignment,
     * retaining pointers into the same actual mutable assignment records. */
    const PharaohObeliskWorkSpot *small_obelisk_counts,*large_obelisk_counts;
    PharaohInfoMonumentTextResources *text_resources; /* actualEADB30 */
    const PharaohPyramidProgressReport *pyramid_report; /* actualEADC48,112B */
    const int32_t *sun_temple_report; /* EADC10,14DWORD */
    const PharaohInfoLibraryReport *library_report; /* EADBD8,56B */
    const PharaohInfoScriptedMonumentReport *caesareum_report,*lighthouse_report; /* EADBA8/EADB88 */
    const PharaohInfoAbuSimbelReport *abu_simbel_report; /* EADBC8,16B */
    const PharaohInfoMausoleumReport *mausoleum_report; /* EADB58,48B */
    const int32_t *tomb_report; /* EADB38,8DWORD view */
    const int32_t *victory_stored,*victory_required; /* genuine545FB0/4EA930 resource arrays */
    const PharaohTokenExpansionEntry *token_entries;
    const PharaohTokenExpansionOperations *token_operations;
    int32_t *draw_y_accum; /* actualE20774; aliases shared text drawing.cursor_y */
} PharaohInfoMonumentsLiveContext;

void pharaoh_info_monuments_draw_bullfight_school(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_fort(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_mud_gatehouse(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_brick_gatehouse(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_clay_gatehouse(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_roadblock(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_mud_tower(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_brick_tower(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_clay_tower(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_military_academy(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_recruiter(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_fig_farm(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_pomegranate_farm(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_chickpea_farm(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_wood_cutter(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_papyrus_maker(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_lamp_maker(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_paint_maker(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_chariot_maker(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_brickworks(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_jeweler(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_potter(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_shipwright(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_dock(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_fishing_wharf(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_transport_wharf(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_warship_wharf(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_ferry(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_pyramid(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_sphinx(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_obelisk(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_sun_temple(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_great_library(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_abu_simbel(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_caesareum(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_lighthouse(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_royal_tomb(const PharaohInfoMonumentsLiveContext *);
void pharaoh_info_monuments_draw_mausoleum(const PharaohInfoMonumentsLiveContext *);

/* Complete five necessary leaves. All observed Native consumers test AL;
 * unused upper EAX/address bits are not exposed as invented C arguments. */
uint8_t pharaoh_info_monuments_guild_serves(const PharaohInfoMonumentsLiveContext *,int32_t guild,int32_t definition);
uint8_t pharaoh_info_monuments_needs_resource(const PharaohInfoMonumentsLiveContext *,int32_t building,int32_t resource);
uint8_t pharaoh_info_monuments_pyramid_material_blocked(const PharaohInfoMonumentsLiveContext *,int32_t definition,const PharaohPyramidProgressReport *,int32_t building);
uint8_t pharaoh_info_monuments_has_ramp_work(const PharaohInfoMonumentsLiveContext *,int32_t building);
uint8_t pharaoh_info_monuments_obelisk_has_free_spot(const PharaohInfoMonumentsLiveContext *,int32_t walker_type,int32_t building);

#endif
