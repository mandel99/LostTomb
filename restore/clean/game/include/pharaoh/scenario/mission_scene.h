#ifndef PHARAOH_SCENARIO_MISSION_SCENE_H
#define PHARAOH_SCENARIO_MISSION_SCENE_H

#include "pharaoh/scenario/campaign_mission_load.h"
#include "pharaoh/scenario/saved_scene_reader.h"
#include "pharaoh/scenario/scene_initialize_bindings.h"
#include "pharaoh/scenario/scenario_start_bindings.h"
#include "pharaoh/map/surface_image_tables.h"
#include "pharaoh/map/surface_tiles.h"
#include "pharaoh/runtime/text_loading.h"
#include "pharaoh/map/road_access.h"
#include "pharaoh/ui/briefing_hint.h"

/* Borrowed program/asset catalogues, not live scene storage. Only the table
 * fields named below are used; transient pointers in these templates are
 * ignored and replaced by owned records in the factory. */
typedef struct PharaohMissionSceneProgram {
    const PharaohMapSurfaceContext *surface_tables; /*footprints/rings/distance_offsets*/
    const PharaohRoadAccessContext *road_tables; /*ring_offsets/ring_xy*/
    const PharaohRoadAccessMap *access_tables; /*perimeter/coordinate offsets*/
    const PharaohSpecialEntryMap *entrance_tables; /*optional: inactive mission0 never queries entrances*/
    const PharaohComplexProgressContext *complex_tables; /*optional: active monument scenes unsupported*/
    const PharaohPyramidSummaryContext *pyramid_tables; /*optional: active monument scenes unsupported*/
    const PharaohBuildMenuContext *menu_tables; /*types/tutorial_rules/sidebar initial rows/final mission*/
    const PharaohEmpireCityImageCatalog *empire_images;
    const PharaohBuildingImageProperty *building_properties; /*237 five-DWORD rows*/
    const int32_t (*default_resource_prices)[2]; /*native005D2E60, five pairs*/
    const uint32_t *export_categories; /*six values*/
    const uint32_t (*export_limits)[6]; /*four population rows*/
    const PharaohCampaignIndex *campaign_index;
    const int32_t *difficulty_percentages,*starting_favor,*salary_by_rank;
    const int32_t *farm_progress_max,*farm_harvest_months; /*BCA34C/56F5B8*/
} PharaohMissionSceneProgram;

typedef struct PharaohMissionSceneInputs {
    const PharaohMissionSceneProgram *program;
    PharaohSettingsFile *settings;
    PharaohBriefingState *briefing;
    uint8_t *window_id,*redraw_requested;
    const PharaohTextCatalog *text;
    const PharaohFileReadPartContext *package_files;
    PharaohSavedSceneReaderInputs reader;
    const PharaohSavedSceneMemory *memory; /*NULL selects the CRT heap*/
    PharaohBuildingModelLoadContext *building_models;
    PharaohFigureModelLoadContext *figure_models;
    PharaohTaxSentimentModelLoadContext *tax_model;
    const PharaohSpriteLibraryLoadContext *libraries;
    const PharaohSpriteGroupRecord *groups;
    const PharaohSpriteImageRecord *images;
    const int32_t *image_count;
    void *clock_user_data;
    PharaohCityMessageGetTime get_time;
    PharaohProfileGetTickCount get_tick;
    PharaohProfileQueryCounter query_counter,query_frequency;
    uint32_t random_seed;
    int32_t screen_width;
} PharaohMissionSceneInputs;

typedef struct PharaohMissionScene PharaohMissionScene;
enum PharaohMissionSceneStatus {
    PHARAOH_MISSION_SCENE_READY=1,
    PHARAOH_MISSION_SCENE_INITIALIZED=2,
    PHARAOH_MISSION_SCENE_LOADED=3,
    PHARAOH_MISSION_SCENE_IO_FAILED=-1,
    PHARAOH_MISSION_SCENE_UNSUPPORTED=-2
};

/* Mission0/Nubt only. This owner supplies the actual reader, flood reset,
 * all45 initialization providers and the real scenario-start final steps.
 * Active-building/custom/monument-carry scenes are explicitly unsupported;
 * missing construction/kingdom/ferry child providers are never no-op stubs.
 * Models/libraries/program tables and platform IO/clocks remain borrowed. */
PharaohMissionScene *pharaoh_mission_scene_create(const PharaohMissionSceneInputs *inputs);
void pharaoh_mission_scene_destroy(PharaohMissionScene *scene);
int32_t pharaoh_mission_scene_load_nubt(PharaohMissionScene *scene);
int32_t pharaoh_mission_scene_status(const PharaohMissionScene *scene);

/* The entry can be passed directly to pharaoh_campaign_mission_bind_entry.
 * All returned views are owned by scene and remain valid until destroy. */
PharaohCampaignMissionEntry *pharaoh_mission_scene_entry(PharaohMissionScene *scene);
const PharaohSavedSceneStorage *pharaoh_mission_scene_storage(const PharaohMissionScene *scene);
const PharaohSavedGameGeometry *pharaoh_mission_scene_geometry(const PharaohMissionScene *scene);
const PharaohCityViewContext *pharaoh_mission_scene_view(const PharaohMissionScene *scene);
int32_t *pharaoh_mission_scene_previous_overlay(PharaohMissionScene *scene);
int32_t *pharaoh_mission_scene_current_overlay(PharaohMissionScene *scene);
const int32_t *pharaoh_mission_scene_pyramid_style(const PharaohMissionScene *scene);
PharaohOccupancyState *pharaoh_mission_scene_occupancy(PharaohMissionScene *scene);
PharaohMapSurfaceContext *pharaoh_mission_scene_surface(PharaohMissionScene *scene);
int32_t pharaoh_mission_scene_flood_state(const PharaohMissionScene *scene);
void pharaoh_mission_scene_determine_available_foods(PharaohMissionScene *scene);
const int32_t *pharaoh_mission_scene_food_count(const PharaohMissionScene *scene);
const int32_t *pharaoh_mission_scene_food_ids(const PharaohMissionScene *scene);
const PharaohCityBuildingCounts *pharaoh_mission_scene_counts(const PharaohMissionScene *scene);
const PharaohBuildMenuContext *pharaoh_mission_scene_build_menu(const PharaohMissionScene *scene);
void pharaoh_mission_scene_bind_briefing_hint(PharaohMissionScene *scene,
    PharaohBriefingHintContext *hint,PharaohTextSelection *selection);
const PharaohCityMessageScenarioReset *pharaoh_mission_scene_messages(const PharaohMissionScene *scene);
/* Actual initialized702900 record bank; camera uses its702984 view. */
struct PharaohCameraScrollContext;
void pharaoh_mission_scene_bind_camera(PharaohMissionScene *,struct PharaohCameraScrollContext *);
/* Data ownership only: caller owns the actual224-byte UI state and supplies
 * platform input/drawing/operation providers before invoking any dispatcher. */
struct PharaohBuildingInfoLiveContext;
struct PharaohBuildingInfoLiveInitContext;
void pharaoh_mission_scene_bind_building_info_world(PharaohMissionScene *,
    struct PharaohBuildingInfoLiveContext *,struct PharaohBuildingInfoLiveInitContext *);

struct PharaohCityDebugGridValues;
void pharaoh_mission_scene_bind_debug_grid_values(PharaohMissionScene *,
    struct PharaohCityDebugGridValues *);

struct PharaohBuildingInfoPhraseWeightBank;
struct PharaohBuildingInfoPhraseWeightBank *pharaoh_mission_scene_info_phrase_bank(PharaohMissionScene *);

struct PharaohInfoIndustryLiveContext;
struct PharaohFoodSupplyQueryContext;
struct PharaohMapHouseWaterContext;
void pharaoh_mission_scene_bind_info_industry_world(PharaohMissionScene *,
    struct PharaohInfoIndustryLiveContext *,struct PharaohFoodSupplyQueryContext *,
    struct PharaohMapHouseWaterContext *);

#endif
