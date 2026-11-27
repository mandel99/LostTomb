#include "pharaoh/scenario/mission_scene.h"
#include "pharaoh/ui/building_info_live.h"
#include "pharaoh/ui/city_debug_grid.h"
#include "pharaoh/runtime/camera_scroll.h"
#include "pharaoh/building/construction_load.h"
#include "pharaoh/building/construction_support.h"
#include "pharaoh/core/crt_random.h"
#include "pharaoh/map/routing_search.h"
#include "pharaoh/monuments/base_progress.h"
#include "pharaoh/population/health_coverage.h"

#include <stdlib.h>
#include <string.h>

enum { SCENE_TILES=51984, SCENE_ROWS=228, CODEC_SCRATCH=1200000 };
typedef union SceneDword { int32_t signed_value; uint32_t value;
    struct { int16_t low,high; } words; } SceneDword;

struct PharaohMissionScene {
    PharaohSavedSceneMemory memory;
    PharaohMissionSceneInputs inputs;
    int32_t status;
    PharaohSavedSceneStorage saved;
    PharaohSavedSceneReader reader;
    uint8_t codec_scratch[CODEC_SCRATCH];
    PharaohCrtRandom random;
    PharaohCampaignMissionLoadContext load;
    PharaohCampaignMissionLoadOperations load_operations;
    PharaohCampaignMissionEntry entry;
    PharaohScenarioStartMissionContext start;
    PharaohScenarioStartBindings start_bindings;
    PharaohSavedGameInitializeContext initialize;
    PharaohSavedGameInitializeOperations initialize_operations;
    PharaohSceneInitializeBindings bindings;
    PharaohSavedGameGeometry geometry;
    PharaohMapGridGeometry grid;
    PharaohMapTerrainView map;
    PharaohMapSurfaceContext surface;
    PharaohSurfaceImageTables image_tables;
    PharaohMapImageSelection image_selection;
    PharaohPyramidTilePlacement pyramid_placement;
    PharaohMapBuildingUndo undo;
    PharaohBuildingTypeRecord undo_buildings[4000];
    int16_t undo_ids[525];
    uint32_t undo_count,undo_available;
    uint32_t current_tile,adjacent[8],channels_pending,placement_failed,snapshot_calls;
    int32_t scan_x,scan_y,area_x,area_y,preview_count;
    int32_t preview_offsets[1][2]; /*count remains0 for this mission-only owner*/
    int32_t row_x_start[SCENE_ROWS],row_x_end[SCENE_ROWS],row_start[SCENE_ROWS];
    int32_t map_orientation,current_climate,scenario_rank,scenario_mode,extra_camera_bounds,suppress_flood;
    SceneDword removal_undo;
    uint32_t editor_active,force_paved,event_popup,health_pending;
    uint8_t water_revalidation,ui_flag;
    int32_t overlay,previous_overlay,custom_value,warship_wharves;
    uint32_t pyramid_queue_count,pyramid_queue[4],pyramid_stage3;
    PharaohCityViewState view_state;
    PharaohCityProjectionLookup projection;
    PharaohCityViewContext view;
    PharaohSceneAssetBindings assets;
    PharaohSceneMonumentAssets monument_assets;
    PharaohSceneEnemyAssets enemy_assets;
    int32_t pyramid_style,extra_monument,mausoleum_size;
    PharaohCitySoundInitializeContext sound;
    int32_t sound_records[70][32];
    uint32_t sound_tick;
    PharaohSceneRecords records;
    uint32_t derived_flag,state_pair[2];
    int32_t route_reset[12996],construction_size,anchor_x,anchor_y,transport_count,highest_storage;
    PharaohCityMessageProblemState message_problem;
    PharaohSceneRouting routing;
    uint8_t routing_cells[4][SCENE_TILES],regions[SCENE_TILES],shades[SCENE_TILES];
    uint32_t buildings_without_id;
    PharaohRoutingSearchState route_search;
    int32_t route_distance[SCENE_TILES],route_queue[SCENE_TILES];
    PharaohRoadNetworkContext roads;
    PharaohRoadAccessContext road_access;
    PharaohRoadAccessMap access;
    PharaohSpecialEntryMap special_entry;
    PharaohAccessRepairMap repair;
    int32_t access_x,access_y,last_access_tile;
    PharaohOccupancyState occupancy;
    PharaohOccupancyBuilding occupancy_buildings[4000];
    PharaohSceneCounts counts_context;
    PharaohCityBuildingCounts counts;
    PharaohSceneUi ui;
    PharaohCityServiceCoverage coverage;
    PharaohSceneStorage storage;
    PharaohGranarySupplyContext granary;
    PharaohGranarySupplyCandidates granary_candidates;
    uint32_t granary_candidate_count,granary_totals[36];
    PharaohMonumentSlotContext monument_slots;
    PharaohSceneEconomy economy;
    PharaohEmpireSupplyContext supply;
    PharaohResourceAvailabilityContext resources;
    int32_t resource_count,food_count,resource_ids[36],food_ids[36];
    PharaohEmpireImagesContext empire_images;
    PharaohEmpireCityImageCatalog empire_catalog;
    PharaohExportKeepContext export_keep;
    int32_t export_amounts[36];
    PharaohMonumentResourceDemand demand;
    PharaohComplexProgressContext complex;
    PharaohPyramidSummaryContext pyramid;
    int32_t frontier_count,frontier[144];
    PharaohBaseMonumentProgress base_progress[4];
    PharaohMonumentBuildRequirements requirements;
    PharaohBuildMenuContext menu;
    int32_t menu_enabled[38*30],selected_category,cheat_allow_all;
    PharaohSidebarBuildButton sidebar[2][12];
    PharaohBuildingAccessContext kingdom;
    PharaohBuildingAccessOperations kingdom_operations;
    PharaohRotatedBuildingContext rotated;
    PharaohVenueTileContext venue;
    PharaohFloodWorld flood_world;
    PharaohFloodMemory flood_memory;
    PharaohFloodListsContext flood_lists;
    PharaohFloodBuildContext flood_build;
    PharaohFloodBuildOperations flood_operations;
    PharaohConstructionLoadContext construction;
    PharaohConstructionLoadOperations construction_operations;
    PharaohConstructionSupportContext construction_support;
    PharaohMonumentAnimationRegistry animations;
    PharaohTextSelection text;
    PharaohCityMessageScenarioReset messages;
    int32_t message_queue[20],message_shown,message_scroll,message_list_scroll;
    int32_t message_posted,message_fire,message_collapse,message_reserved[4],message_problem_count;
    PharaohMissionRecordApplyState record_apply;
    PharaohMissionRecordApplyOperations record_operations;
    PharaohMissionSpecialPositions special_positions;
    PharaohWalkerActionProfileState profile;
    PharaohWalkerActionProfileEntry profiles[110];
    uint8_t tutorial_legions,tutorial_a[26],tutorial_b[15];
    uint32_t tutorial_first,tutorial_five,tutorial_ready[7],cheats_used;
    char player_name[32];
};

_Static_assert(sizeof(PharaohBuildingTypeRecord)==264 &&
    sizeof(PharaohScenarioPlayerRuntime)==0x49d8 &&
    offsetof(PharaohScenarioPlayerRuntime,initial_rank)==0x444c,
    "mission scene aliases retain native record strides");

static void *allocate(void *user,uint32_t bytes)
{ (void)user;return malloc(bytes); }
static void release(void *user,void *allocation)
{ (void)user;free(allocation); }
static void *chunk(PharaohMissionScene *r,uint32_t index,uint32_t offset)
{ return (uint8_t *)r->saved.destinations[index]+offset; }

static void construction(void *data,int32_t owner,int32_t kind,int32_t id,
    int32_t x,int32_t y,int32_t size,int32_t creating,int32_t mode)
{
    PharaohMissionScene *r=data;
    pharaoh_building_construction_add_to_map(&r->construction,
        (uint32_t)owner,kind,id,x,y,size,(uint32_t)creating,(uint32_t)mode);
}
static const char *select_record_text(void *data,int32_t group,int32_t entry)
{
    PharaohMissionScene *r=data;
    pharaoh_text_select(&r->text,group,entry);
    return r->text.current;
}
static void finalize_records(void *data)
{ pharaoh_map_refresh_all_tiles(&((PharaohMissionScene *)data)->surface); }

/* Set only live pointers and metadata-qualified aliases. All allocations and
 * process-lifetime image table state survive this rebinding after the read. */
static void bind_live(PharaohMissionScene *r)
{
    PharaohSavedSceneViews *v=&r->saved.view;
    const PharaohMissionSceneProgram *p=r->inputs.program;
    uint8_t *metadata=v->metadata;
    const int32_t player=*v->current_player;
    r->scenario_mode=*(const int32_t *)(const void *)metadata;
    r->map.terrain=v->terrain;
    r->surface.image_ids=v->images;r->surface.property_flags=v->properties;
    r->surface.edge_flags=v->edges;r->surface.canal_water=v->canal_water;
    r->surface.height_values=v->height_values;r->surface.fertility=v->fertility;
    r->surface.variation_tag=v->variation_tag;r->surface.meadow_value=v->meadow_value;
    r->surface.random=v->random;r->surface.buildings=v->buildings;
    r->surface.building_ids=v->building_ids;
    r->surface.blocked_grid=v->walker_ids;
    r->surface.entry_x=r->saved.destinations[122];
    r->surface.entry_y=r->saved.destinations[123];
    r->surface.exit_x=r->saved.destinations[124];
    r->surface.exit_y=r->saved.destinations[125];
    r->surface.entry_tile=r->saved.destinations[132];
    r->surface.exit_tile=r->saved.destinations[133];
    r->surface.building_orientation=(const uint32_t *)(const void *)v->city_orientation;
    r->surface.scenario_entry_x=(const int16_t *)(const void *)(metadata+0x38c);
    r->surface.scenario_entry_y=(const int16_t *)(const void *)(metadata+0x38e);
    r->surface.scenario_exit_x=(const int16_t *)(const void *)(metadata+0x390);
    r->surface.scenario_exit_y=(const int16_t *)(const void *)(metadata+0x392);
    r->surface.sweep_start=r->saved.destinations[107];
    r->surface.sweep_countdown=r->saved.destinations[131];
    r->records.terrain=v->terrain;r->records.property_flags=v->properties;
    r->records.overlay_values=v->canal_water;
    r->records.legacy_buildings=(const PharaohLegacyBuildingRecord *)(const void *)v->storages;
    r->records.ferry_paths=r->saved.destinations[138];
    r->records.highest_building=r->saved.destinations[93];
    r->records.highest_building_ever=r->saved.destinations[39];
    r->roads.terrain=v->terrain;r->roads.players=v->players;
    r->roads.current_player=v->current_player;
    r->road_access.building_ids=v->building_ids;
    r->access.terrain=v->terrain;r->access.origin=r->geometry.grid_start;
    r->access.width=r->geometry.width;r->access.height=r->geometry.height;
    r->special_entry.buildings=v->buildings;
    r->route_search.terrain=v->terrain;
    r->repair.terrain=v->terrain;r->repair.edges=v->edges;r->repair.footprints=v->properties;
    r->occupancy.terrain=v->terrain;
    for(int32_t i=0;i<4000;++i) {
        r->occupancy_buildings[i].x=v->buildings[i].x;
        r->occupancy_buildings[i].y=v->buildings[i].y;
    }
    r->routing.tile_direction=r->saved.destinations[14];
    r->routing.footprints=v->properties;
    r->routing.flood_tiles=r->flood_world.tiles;
    r->routing.map_orientation=&r->map_orientation;
    r->routing.city_orientation=v->city_orientation;
    r->counts_context.highest_building=r->records.highest_building;
    r->ui.people_at_age=(const int16_t *)(const void *)((uint8_t *)&v->players[player]+0x25c4);
    r->ui.health_buildings=(const PharaohHealthCoverageBuilding *)(const void *)v->buildings;
    r->ui.god_status=(const int16_t *)(const void *)(metadata+0x0a);
    r->supply.cities=(const PharaohEmpireCityRecord *)(const void *)v->empire_cities;
    r->supply.route_limits=r->saved.destinations[114];
    r->supply.players=v->players;r->supply.current_player=v->current_player;
    r->supply.allowed_buildings=(const int16_t *)(const void *)(metadata+0x408);
    r->resources.events=(const PharaohScenarioEvent *)chunk(r,136,4);
    r->resources.event_count=r->saved.destinations[136];
    r->monument_assets.goal_definition_ids=(const int16_t *)(const void *)(metadata+0x508);
    r->monument_assets.animation_set=metadata+0x3e4;
    r->enemy_assets.enemy_type=(int16_t *)(void *)(metadata+0x24);
    r->menu.allowed_buildings=r->supply.allowed_buildings;
    r->menu.complexes_allowed=(const int16_t *)(const void *)(metadata+0x4d8);
    r->menu.god_status=r->ui.god_status;
    r->menu.goal_text_ids=r->monument_assets.goal_definition_ids;
    r->demand.players=v->players;r->demand.current_player=v->current_player;
    r->demand.planned_kinds=r->monument_assets.goal_definition_ids;
    r->complex.buildings=v->buildings;r->complex.walkers=v->walkers;
    r->pyramid.buildings=v->buildings;r->pyramid.walkers=v->walkers;
    r->pyramid.tile_building_ids=v->building_ids;
    for(int32_t i=0;i<4;++i) {
        r->base_progress[i].buildings=v->buildings;r->base_progress[i].walkers=v->walkers;
    }
    r->granary.buildings=v->buildings;
    r->granary.storages=(const PharaohBuildingStorageRecord *)(const void *)v->storages;
    r->granary.highest_building=r->records.highest_building;
    r->empire_images.objects=(PharaohEmpireObjectRecord *)(void *)v->empire_objects;
    r->scenario_rank=*(const int16_t *)(const void *)(metadata+0x28a);
    r->empire_images.scenario_rank=&r->scenario_rank;
    r->export_keep.highest_building=r->records.highest_building;
    r->export_keep.produced=(const uint32_t *)(const void *)(metadata+0x510);
    r->export_keep.consumed=(const uint32_t *)(const void *)(metadata+0x5a0);
    r->storage.storages=(PharaohBuildingStorageRecord *)(void *)v->storages;
    r->kingdom.highest_id=r->records.highest_building;
    r->construction.surface=&r->surface;
    r->construction.cities=(PharaohConstructionCity *)(void *)v->players;
    r->construction.current_player=(const uint32_t *)(const void *)v->current_player;
    r->construction.properties=p->building_properties;
    r->construction.formations=r->saved.destinations[24];
    r->construction.ferry_queues=r->saved.destinations[137];
    r->construction.saved_legions=(PharaohConstructionSavedLegion *)(void *)r->saved.mission_records.started.legions;
    r->construction_support.surface=&r->surface;
    r->construction_support.storages=(PharaohBuildingStorageRecord *)(void *)v->storages;
    r->construction_support.ferry_queues=r->saved.destinations[137];
    r->start.players=v->players;r->start.current_player=v->current_player;
    r->start.scenario_initial_rank=(const int16_t *)(const void *)(metadata+0x28a);
    r->start.scenario_animation_flag=metadata+0x3e4;
    r->start.game_is_replay=v->game_is_replay;
    r->start_bindings.scene=&r->saved;
}

static void read_state(void *data,int32_t file)
{
    PharaohMissionScene *r=data;
    pharaoh_saved_game_state_read(&r->reader.read,file);
    /* This factory currently owns the empty mission0 path. Reject another
     * scene before invoking a direct provider with unbound deeper children. */
    if(r->inputs.settings->mission_id!=0 || r->inputs.settings->is_custom_scenario!=0 ||
        *r->saved.view.current_player<0 || *r->saved.view.current_player>1) {
        r->status=PHARAOH_MISSION_SCENE_UNSUPPORTED;return;
    }
    for(int32_t i=1;i<4000;++i) if(r->saved.view.buildings[i].state==1 ||
        r->saved.view.buildings[i].state==3) {
        r->status=PHARAOH_MISSION_SCENE_UNSUPPORTED;return;
    }
    const int16_t *planned=(const int16_t *)(const void *)(r->saved.view.metadata+0x508);
    const PharaohScenarioPlayerRuntime *player=&r->saved.view.players[*r->saved.view.current_player];
    for(int32_t i=0;i<3;++i) if(planned[i]!=0 || player->monument_building_ids[i]!=0 || player->monument_definition_ids[i]!=0) {
        r->status=PHARAOH_MISSION_SCENE_UNSUPPORTED;return;
    }
    bind_live(r);
}
static void reset_floodplain(void *data)
{
    PharaohMissionScene *r=data;
    pharaoh_floodplain_settings_initialize(r->saved.view.floodplain_settings);
}
static void initialize_state(void *data)
{
    PharaohMissionScene *r=data;
    if(r->status<0) return;
    pharaoh_saved_game_bind_scene_storage(&r->bindings,&r->saved,&r->reader.read);
    /* Geometry-dependent copies are normalized only after the real reader. */
    const PharaohSavedGameGeometry *g=r->initialize.scenario_geometry;
    r->access.origin=g->grid_start;r->access.width=g->width;r->access.height=g->height;
    r->special_entry.origin=&r->geometry.grid_start;
    pharaoh_saved_game_bind_scene(&r->initialize_operations,&r->bindings);
    pharaoh_saved_game_initialize(&r->initialize);
    r->status=PHARAOH_MISSION_SCENE_INITIALIZED;
}

static void configure(PharaohMissionScene *r)
{
    const PharaohMissionSceneInputs *in=&r->inputs;
    const PharaohMissionSceneProgram *p=in->program;
    const PharaohMapSurfaceContext *tables=p->surface_tables;
    const PharaohSavedSceneViews *v=&r->saved.view;
    r->image_tables=pharaoh_surface_image_initial_tables;
    r->surface=(PharaohMapSurfaceContext){
        .map=&r->map,.geometry=&r->grid,.border_size=&r->geometry.grid_border,
        .orientation=&r->map_orientation,.editor_active=&r->editor_active,
        .force_paved_roads=&r->force_paved,
        .entry_x=r->saved.destinations[122],.entry_y=r->saved.destinations[123],
        .exit_x=r->saved.destinations[124],.exit_y=r->saved.destinations[125],
        .entry_tile=r->saved.destinations[132],.exit_tile=r->saved.destinations[133],
        .row_x_start=r->row_x_start,.row_x_end=r->row_x_end,.row_start_offset=r->row_start,
        .area_x=&r->area_x,.area_y=&r->area_y,.channels_refresh_pending=&r->channels_pending,
        .current_tile=&r->current_tile,.adjacent=r->adjacent,.scan_x=&r->scan_x,.scan_y=&r->scan_y,
        .climate=(const int16_t *)(const void *)&r->current_climate,
        .water_revalidation_pending=&r->water_revalidation,.blocked_grid=v->walker_ids,
        .flood_tiles=r->flood_world.tiles,.groups=in->groups,.footprints=tables->footprints,
        .placement_failed=&r->placement_failed,.selection=&r->image_selection,
        .rings=tables->rings,.distance_offsets=tables->distance_offsets,
        .pyramid_style=(const uint32_t *)(const void *)&r->pyramid_style,
        .pyramid_regular=pharaoh_pyramid_regular_special,.pyramid_style_five=pharaoh_pyramid_mastaba_special,
        .pyramid_queue_count=&r->pyramid_queue_count,.pyramid_queue=r->pyramid_queue,
        .pyramid_stage3_offset=&r->pyramid_stage3,.pyramid_complex_offsets=pharaoh_pyramid_complex_temple_offsets,
        .pyramid_placement=&r->pyramid_placement,.undo=&r->undo,.snapshot_calls=&r->snapshot_calls,
        .preview_count=&r->preview_count,.preview_offsets=r->preview_offsets};
    pharaoh_map_surface_bind_image_tables(&r->surface,&r->image_tables);
    r->undo=(PharaohMapBuildingUndo){v->buildings,r->undo_buildings,r->undo_ids,&r->undo_count,&r->undo_available};
    r->view=(PharaohCityViewContext){.state=&r->view_state,.projection=&r->projection,
        .map_image_grid=(const int32_t *)v->images,.map_bounds=&r->grid.bounds,.screen_width=in->screen_width};
    pharaoh_city_view_set_viewport_with_sidebar(&r->view);
    r->initialize=(PharaohSavedGameInitializeContext){
        .geometry=&r->geometry,.current_climate=&r->current_climate,
        .map_view_orientation=&r->map_orientation,.flood_revalidation_pending=&r->water_revalidation,
        .removal_undo=&r->removal_undo.signed_value,.city_overlay=&r->overlay,.previous_city_overlay=&r->previous_overlay,
        .briefing=in->briefing,.message_scroll_position=&r->message_scroll,.message_list_scroll=&r->message_list_scroll,
        .ui_flag=&r->ui_flag,.health_recompute_distances=&r->health_pending,.warship_wharf_count=&r->warship_wharves,
        .scenario_mode=&r->scenario_mode,.custom_value_78d828=&r->custom_value};
    r->monument_assets=(PharaohSceneMonumentAssets){.libraries=in->libraries,.mission_id=&in->settings->mission_id,
        .scenario_name="Nubt",.pyramid_style=&r->pyramid_style,.extra_script_monument=&r->extra_monument,.mausoleum_size=&r->mausoleum_size};
    r->enemy_assets=(PharaohSceneEnemyAssets){.libraries=in->libraries,.enemy_models=in->figure_models->enemy_models,.figure_models=in->figure_models->figure_models};
    r->sound=(PharaohCitySoundInitializeContext){r->sound_records,&r->sound_tick,in->clock_user_data,in->get_time};
    r->assets=(PharaohSceneAssetBindings){in->settings,in->building_models,in->figure_models,in->tax_model,&r->monument_assets,&r->enemy_assets,&r->sound};
    r->records=(PharaohSceneRecords){.grid=&r->grid,.row_x_start=r->row_x_start,.row_x_end=r->row_x_end,.row_start_offset=r->row_start,
        .first_garden_image=(const int32_t *)(const void *)&in->groups[328].first_image,.derived_flag=&r->derived_flag,
        .state_pair_first=&r->state_pair[0],.state_pair_second=&r->state_pair[1],.route_reset_values=r->route_reset,
        .legacy_highest_building=&r->highest_storage,.anchor_x=&r->anchor_x,.anchor_y=&r->anchor_y,.active_transport_count=&r->transport_count,
        .building_properties=(const int32_t *)(const void *)p->building_properties,.construction_size=&r->construction_size,
        .add_building_to_map=construction,.construction_user_data=r,.message_problems=&r->message_problem,
        .get_tick=in->get_time,.clock_user_data=in->clock_user_data,.view=&r->view,
        .extra_camera_bounds_enabled=&r->extra_camera_bounds,.event_54_suppress_flood=&r->suppress_flood};
    r->routing=(PharaohSceneRouting){.geometry=&r->geometry,.citizen=r->routing_cells[0],.noncitizen=r->routing_cells[1],
        .water=r->routing_cells[2],.walls=r->routing_cells[3],.cleared_building_image=&in->groups[0].first_image,
        .flood_image_base=&in->groups[528].first_image,.buildings_without_id_cleared=&r->buildings_without_id};
    r->roads=(PharaohRoadNetworkContext){.geometry=&r->geometry,.citizen_routing=(const int8_t *)r->routing_cells[0],.regions=r->regions,.shades=r->shades};
    r->road_access=(PharaohRoadAccessContext){.ring_offsets=p->road_tables!=NULL?p->road_tables->ring_offsets:NULL,.ring_xy=p->road_tables!=NULL?p->road_tables->ring_xy:NULL};
    r->access=(PharaohRoadAccessMap){.routing_distance=r->route_distance,.region_color=r->regions,
        .perimeter_offsets=p->access_tables!=NULL?p->access_tables->perimeter_offsets:NULL,.coordinate_offsets=p->access_tables!=NULL?p->access_tables->coordinate_offsets:NULL,
        .result_x=&r->access_x,.result_y=&r->access_y,.last_tile=&r->last_access_tile};
    r->special_entry=(PharaohSpecialEntryMap){.mastaba_entries=p->entrance_tables!=NULL?p->entrance_tables->mastaba_entries:NULL,
        .pyramid_entries=p->entrance_tables!=NULL?p->entrance_tables->pyramid_entries:NULL,.layout_directions=p->entrance_tables!=NULL?p->entrance_tables->layout_directions:NULL,
        .pyramid_style=&r->pyramid_style,.origin=&r->geometry.grid_start};
    r->route_search=(PharaohRoutingSearchState){.distance=r->route_distance,.queue_items=r->route_queue,.land_citizen=(const int8_t *)r->routing_cells[0]};
    r->repair.routing=&r->route_search;
    r->occupancy=(PharaohOccupancyState){.offsets=(const PharaohOccupancyOffset *)(const void *)tables->footprints,
        .objects=(const uint8_t *const *)(const void *)r->flood_world.tiles,.buildings=r->occupancy_buildings};
    r->counts_context=(PharaohSceneCounts){.roads=&r->roads,.access=&r->road_access,.counts=&r->counts,.occupancy=&r->occupancy};
    r->ui=(PharaohSceneUi){.build_menu=&r->menu,.counts=&r->counts,.coverage=&r->coverage};
    r->supply.allowed_buildings=(const int16_t *)(const void *)(v->metadata+0x408);
    r->resources=(PharaohResourceAvailabilityContext){.supply=&r->supply,.resource_count=&r->resource_count,.resources=r->resource_ids,.food_count=&r->food_count,.foods=r->food_ids};
    r->empire_catalog=*p->empire_images;
    /*004425C0 reloads these live SG3 group first-image values.*/
    r->empire_catalog.default_city_group=(int32_t)in->groups[274].first_image;
    r->empire_catalog.rank_ten_city_group=(int32_t)in->groups[436].first_image;
    r->empire_catalog.type_two_city_group=(int32_t)in->groups[433].first_image;
    r->empire_catalog.type_four_city_group=(int32_t)in->groups[438].first_image;
    r->empire_images=(PharaohEmpireImagesContext){.catalog=&r->empire_catalog,
        .default_image_base=(const int32_t *)(const void *)&in->groups[459].first_image,
        .default_image_limit=(const int32_t *)(const void *)&in->groups[460].first_image,.images=in->images,.image_count=in->image_count};
    r->economy=(PharaohSceneEconomy){&r->resources,&r->empire_images};
    r->complex=p->complex_tables!=NULL?*p->complex_tables:(PharaohComplexProgressContext){0};
    r->pyramid=p->pyramid_tables!=NULL?*p->pyramid_tables:(PharaohPyramidSummaryContext){0};
    r->pyramid.style=&r->pyramid_style;r->pyramid.frontier_count=&r->frontier_count;r->pyramid.frontier_parts=r->frontier;
    r->demand=(PharaohMonumentResourceDemand){.complex=&r->complex,.pyramid=&r->pyramid};
    r->requirements=(PharaohMonumentBuildRequirements){&r->demand,pharaoh_monument_definitions,
        &r->base_progress[0],&r->base_progress[1],&r->base_progress[2],&r->base_progress[3]};
    r->menu=(PharaohBuildMenuContext){.supply=&r->supply,.monuments=&r->requirements,.settings=in->settings,
        .tutorial_rules=p->menu_tables->tutorial_rules,.tutorial_flags=r->tutorial_a,
        .campaign_mode=&in->settings->is_custom_scenario,.cheat_allow_all=&r->cheat_allow_all,.editor_active=(const int32_t *)(const void *)&r->editor_active,
        .final_mission_id=p->menu_tables->final_mission_id,.types=p->menu_tables->types,.enabled=r->menu_enabled,.selected_category=&r->selected_category,
        .sidebar_buttons={r->sidebar[0],r->sidebar[1]}};
    memcpy(r->sidebar[0],p->menu_tables->sidebar_buttons[0],sizeof(r->sidebar[0]));
    memcpy(r->sidebar[1],p->menu_tables->sidebar_buttons[1],sizeof(r->sidebar[1]));
    r->granary=(PharaohGranarySupplyContext){.resource_totals=r->granary_totals,.candidates=&r->granary_candidates,.candidate_count=&r->granary_candidate_count};
    r->storage=(PharaohSceneStorage){.granary_supply=&r->granary,.monuments=&r->monument_slots,.asset_monuments=&r->monument_assets};
    r->export_keep=(PharaohExportKeepContext){.monuments=&r->demand,.categories=p->export_categories,.population_limits=p->export_limits,.export_keep=r->export_amounts};
    r->kingdom=(PharaohBuildingAccessContext){.access=&r->access,.pyramid=&r->special_entry,.repair=&r->repair,
        .event_popup=&r->event_popup,.removal_undo=&r->removal_undo.value,.operations=&r->kingdom_operations};
    r->rotated=(PharaohRotatedBuildingContext){.venue=&r->venue};
    r->flood_memory=(PharaohFloodMemory){r->memory.user_data,r->memory.allocate,r->memory.release};
    r->flood_lists=(PharaohFloodListsContext){.world=&r->flood_world,.memory=&r->flood_memory};
    r->flood_build=(PharaohFloodBuildContext){.map=&r->map,.surface=&r->surface};
    r->bindings=(PharaohSceneInitializeBindings){.initialize=&r->initialize,.assets=&r->assets,.records=&r->records,
        .routing=&r->routing,.counts=&r->counts_context,.ui=&r->ui,.storage=&r->storage,.economy=&r->economy,
        .surface=&r->surface,.kingdom=&r->kingdom,.rotated=&r->rotated,.export_keep=&r->export_keep,
        .floods=&r->flood_build,.flood_lists=&r->flood_lists,.flood_operations=&r->flood_operations};
    r->text.catalog=in->text;
    r->messages=(PharaohCityMessageScenarioReset){.queue=r->message_queue,.messages_used=r->saved.destinations[79],
        .shown_index=&r->message_shown,.scroll_position=&r->message_scroll,.list_scroll=&r->message_list_scroll,
        .posted_once=&r->message_posted,.fire_last_time=&r->message_fire,.collapse_last_time=&r->message_collapse,
        .reserved1=&r->message_reserved[0],.reserved2=&r->message_reserved[1],.count_problem=&r->message_problem_count,
        .queue_reserved=&r->message_reserved[2],.problem=&r->message_problem,.get_time=in->get_time,.clock_user_data=in->clock_user_data};
    r->record_apply.special_positions=&r->special_positions;
    r->record_operations=(PharaohMissionRecordApplyOperations){.user_data=r,.select_text=select_record_text,.finalize_records=finalize_records};
    r->profile=(PharaohWalkerActionProfileState){.entries=r->profiles,.query_counter=in->query_counter,.get_tick_count=in->get_tick,.clock_user_data=in->clock_user_data};
    r->start_bindings=(PharaohScenarioStartBindings){&r->saved,&r->menu,&r->messages,&r->record_apply,&r->record_operations,&r->profile,in->query_frequency};
    r->start=(PharaohScenarioStartMissionContext){.path_prefix="",.settings=in->settings,
        .tutorial_legions_message_posted=&r->tutorial_legions,.current_window_id=in->window_id,.window_redraw_requested=in->redraw_requested,
        .player_name_destination=r->player_name,.difficulty_percentages=p->difficulty_percentages,
        .starting_favor_by_difficulty=p->starting_favor,.salary_by_rank=p->salary_by_rank,.campaign_index=p->campaign_index,
        .tutorial_first_mission=&r->tutorial_first,.tutorial_first_five=&r->tutorial_five,
        .tutorial_ready_flags={&r->tutorial_ready[0],&r->tutorial_ready[1],&r->tutorial_ready[2],&r->tutorial_ready[3],&r->tutorial_ready[4],&r->tutorial_ready[5],&r->tutorial_ready[6]},
        .tutorial_rules=p->menu_tables->tutorial_rules,.tutorial_rule_count=26,
        .tutorial_flags_a=r->tutorial_a,.tutorial_flags_a_count=26,.tutorial_flags_b=r->tutorial_b,.tutorial_flags_b_count=15};
    r->load_operations=(PharaohCampaignMissionLoadOperations){r,read_state,reset_floodplain,initialize_state};
    r->load=(PharaohCampaignMissionLoadContext){in->package_files,in->settings,&r->cheats_used,&r->load_operations};
    r->entry=(PharaohCampaignMissionEntry){&r->load,&r->start,in->briefing,"Nubt"};
    bind_live(r);
    pharaoh_scenario_bind_start_steps(&r->start,&r->start_bindings);
    pharaoh_campaign_mission_bind_entry(&r->entry,&(PharaohBriefingOperations){0});
}

PharaohMissionScene *pharaoh_mission_scene_create(const PharaohMissionSceneInputs *in)
{
    if(in==NULL || in->program==NULL || in->settings==NULL || in->briefing==NULL ||
       in->window_id==NULL || in->redraw_requested==NULL || in->text==NULL || in->package_files==NULL ||
       in->building_models==NULL || in->figure_models==NULL || in->tax_model==NULL || in->libraries==NULL ||
       in->groups==NULL || in->images==NULL || in->image_count==NULL || in->get_time==NULL ||
       in->get_tick==NULL || in->query_counter==NULL || in->query_frequency==NULL) return NULL;
    const PharaohMissionSceneProgram *p=in->program;
    if(p->surface_tables==NULL || p->menu_tables==NULL || p->empire_images==NULL ||
       p->building_properties==NULL || p->export_categories==NULL || p->export_limits==NULL ||
       p->campaign_index==NULL || p->difficulty_percentages==NULL || p->starting_favor==NULL || p->salary_by_rank==NULL) return NULL;
    const PharaohSavedSceneMemory memory=in->memory!=NULL?*in->memory:(PharaohSavedSceneMemory){NULL,allocate,release};
    if(memory.allocate==NULL || memory.release==NULL) return NULL;
    PharaohMissionScene *r=memory.allocate(memory.user_data,(uint32_t)sizeof(*r));
    if(r==NULL) return NULL;
    memset(r,0,sizeof(*r));r->memory=memory;r->inputs=*in;
    pharaoh_crt_random_seed(&r->random,in->random_seed);
    if(!pharaoh_saved_scene_storage_open(&r->saved,in->settings,&r->memory)) {
        memory.release(memory.user_data,r);return NULL;
    }
    /* Native51/52 destinations ARE these mutable service counters. */
    r->saved.destinations[51]=r->counts.services_total;
    r->saved.destinations[52]=r->counts.services_active;
    PharaohSavedSceneReaderInputs reader=in->reader;
    reader.compressed.scratch_input=r->codec_scratch;
    reader.random=&r->random;reader.pyramid_style=&r->pyramid_style;
    reader.default_resource_prices=in->program->default_resource_prices;
    reader.pyramid_parts=(const int32_t *)pharaoh_pyramid_part_counts;
    reader.mastaba_parts=(const int32_t *)pharaoh_mastaba_part_counts;
    pharaoh_saved_scene_reader_bind(&r->reader,&r->saved,&reader);
    configure(r);r->status=PHARAOH_MISSION_SCENE_READY;
    return r;
}
void pharaoh_mission_scene_destroy(PharaohMissionScene *r)
{
    if(r==NULL) return;
    pharaoh_floods_reset(&r->flood_lists);
    pharaoh_saved_scene_reader_close(&r->reader);
    pharaoh_saved_scene_storage_close(&r->saved);
    const PharaohSavedSceneMemory memory=r->memory;
    memory.release(memory.user_data,r);
}
int32_t pharaoh_mission_scene_load_nubt(PharaohMissionScene *r)
{
    if(r==NULL) return PHARAOH_MISSION_SCENE_IO_FAILED;
    if(r->inputs.settings->mission_id!=0 || r->inputs.settings->is_custom_scenario!=0 ||
        r->status!=PHARAOH_MISSION_SCENE_READY) return PHARAOH_MISSION_SCENE_UNSUPPORTED;
    pharaoh_scenario_start_mission(&r->start,"Nubt",0);
    if(r->status==PHARAOH_MISSION_SCENE_INITIALIZED) r->status=PHARAOH_MISSION_SCENE_LOADED;
    else if(r->status==PHARAOH_MISSION_SCENE_READY) r->status=PHARAOH_MISSION_SCENE_IO_FAILED;
    return r->status;
}
int32_t pharaoh_mission_scene_status(const PharaohMissionScene *r)
{return r!=NULL?r->status:PHARAOH_MISSION_SCENE_IO_FAILED;}
PharaohCampaignMissionEntry *pharaoh_mission_scene_entry(PharaohMissionScene *r)
{return &r->entry;}

/* Borrow the actual Native world owners. Platform UI, cursor/font/layout and
 * complete operations remain with the caller; no shadow world or callbacks. */
void pharaoh_mission_scene_bind_building_info_world(PharaohMissionScene *r,
    PharaohBuildingInfoLiveContext *ui,PharaohBuildingInfoLiveInitContext *init)
{
    static const int32_t neighbours[9]={0,-228,228,1,-1,-229,-227,227,229};
    PharaohSavedSceneViews *v=&r->saved.view;
    ui->buildings=v->buildings;ui->walkers=v->walkers;
    ui->formations=r->saved.destinations[24];
    ui->storage=(PharaohBuildingStorageRecord *)(void *)v->storages;
    ui->food_count=&r->food_count;ui->foods=r->food_ids;
    ui->resource_count=&r->resource_count;ui->resources=r->resource_ids;
    ui->current_window=r->inputs.window_id;ui->redraw_requested=r->inputs.redraw_requested;
    init->ui=ui;init->models=r->inputs.building_models->building_models;
    init->editor_active=(const int32_t *)(const void *)&r->editor_active;
    init->grid_start=&r->geometry.grid_start;
    init->building_ids=v->building_ids;init->walker_ids=v->walker_ids;
    init->rubble_types=r->saved.destinations[168];init->canals=v->canal_water;
    init->bitfields=v->properties;init->sprites=r->saved.destinations[18];
    init->terrain=v->terrain;
    init->entry_flag=r->saved.destinations[132];init->exit_flag=r->saved.destinations[133];
    init->neighbours=neighbours;init->sentry_request=r->saved.destinations[116];
}

void pharaoh_mission_scene_bind_debug_grid_values(PharaohMissionScene *r,
    PharaohCityDebugGridValues *g)
{
    const PharaohSavedSceneViews *const v=&r->saved.view;
    *g=(PharaohCityDebugGridValues){
        .building_ids=v->building_ids,.walker_ids=v->walker_ids,
        .building_damage=r->saved.destinations[18],.terrain=v->terrain,
        .images=v->images,.service_visits=v->service_visits,
        .routing_distance=(const uint32_t *)(const void *)r->route_distance,
        .bitfields=v->properties,.edges=v->edges,.canals=v->canal_water,
        .random=v->random,.sprites=r->saved.destinations[14],
        .elevation=r->saved.destinations[17],.desirability=r->saved.destinations[16],
        .old_types=r->saved.destinations[168],.soldier_strength=NULL,
        .land_citizen=(const int8_t *)(const void *)r->routing_cells[0],
        .land_noncitizen=(const int8_t *)(const void *)r->routing_cells[1],
        .water=(const int8_t *)(const void *)r->routing_cells[2],
        .walls=(const int8_t *)(const void *)r->routing_cells[3],
        .road_network=r->regions,.road_segment=r->shades};
}

const PharaohSavedSceneStorage *pharaoh_mission_scene_storage(const PharaohMissionScene *r)
{return &r->saved;}
const PharaohSavedGameGeometry *pharaoh_mission_scene_geometry(const PharaohMissionScene *r)
{return &r->geometry;}
const PharaohCityViewContext *pharaoh_mission_scene_view(const PharaohMissionScene *r)
{return &r->view;}
const int32_t *pharaoh_mission_scene_pyramid_style(const PharaohMissionScene *r)
{return &r->pyramid_style;}
PharaohOccupancyState *pharaoh_mission_scene_occupancy(PharaohMissionScene *r)
{return &r->occupancy;}
PharaohMapSurfaceContext *pharaoh_mission_scene_surface(PharaohMissionScene *r)
{return &r->surface;}
int32_t pharaoh_mission_scene_flood_state(const PharaohMissionScene *r)
{return r->flood_world.state;}
void pharaoh_mission_scene_determine_available_foods(PharaohMissionScene *r)
{pharaoh_city_resource_determine_available(&r->resources);}
const int32_t *pharaoh_mission_scene_food_count(const PharaohMissionScene *r)
{return &r->food_count;}
const int32_t *pharaoh_mission_scene_food_ids(const PharaohMissionScene *r)
{return r->food_ids;}
const PharaohCityBuildingCounts *pharaoh_mission_scene_counts(const PharaohMissionScene *r)
{return &r->counts;}
const PharaohBuildMenuContext *pharaoh_mission_scene_build_menu(const PharaohMissionScene *r)
{return &r->menu;}

void pharaoh_mission_scene_bind_briefing_hint(PharaohMissionScene *r,
    PharaohBriefingHintContext *hint,PharaohTextSelection *selection)
{
    *hint=(PharaohBriefingHintContext){
        .scenario_mode=&r->inputs.settings->is_custom_scenario,
        .current_player=r->saved.view.current_player,
        .mission_id=&r->inputs.settings->mission_id,
        .tutorial_flags=r->tutorial_a,
        .active_buildings=(const int32_t *)(const void *)r->counts.active,
        .players=r->saved.view.players,
        .monument_goal=(const int32_t *)(const void *)(r->saved.view.metadata+0x354),
        .selection=selection};
}

const PharaohCityMessageScenarioReset *pharaoh_mission_scene_messages(const PharaohMissionScene *r)
{return &r->messages;}

void pharaoh_mission_scene_bind_camera(PharaohMissionScene *r,PharaohCameraScrollContext *camera)
{
    /* Pinned5E2A60 DWORD1, sole observed xref is the scroll consumer. */
    static const int32_t vertical_step=1;
    camera->large_vertical_step=&vertical_step;
    camera->view=&r->view;
    camera->decay_records=(int32_t (*)[32])(void *)&r->sound_records[1][1];
}

int32_t *pharaoh_mission_scene_current_overlay(PharaohMissionScene *scene)
{ return &scene->overlay; }

int32_t *pharaoh_mission_scene_previous_overlay(PharaohMissionScene *scene)
{ return &scene->previous_overlay; }
