#ifndef PHARAOH_SCENARIO_SCENE_RECORDS_H
#define PHARAOH_SCENARIO_SCENE_RECORDS_H

#include "pharaoh/scenario/saved_game_initialize.h"
#include "pharaoh/map/scene_grid.h"
#include "pharaoh/building/type_queries.h"
#include "pharaoh/runtime/city_view.h"
#include "pharaoh/ui/city_message_text.h"

typedef struct PharaohSceneRecords {
    PharaohSavedGameInitializeContext *initialize;
    PharaohMapGridGeometry *grid;
    int32_t *row_x_start, *row_x_end, *row_start_offset;
    uint32_t *terrain;
    uint8_t *property_flags, *overlay_values;
    const int32_t *first_garden_image;
    uint32_t *derived_flag, *state_pair_first, *state_pair_second;
    int32_t *route_reset_values; /* 12996 DWORDs */
    int16_t *route_walker_ids; /* 1000 WORDs */
    PharaohBuildingTypeRecord *buildings;
    PharaohWalkerQueryRecord *walkers;
    const PharaohLegacyBuildingRecord *legacy_buildings; /* 200 records */
    const PharaohFerryPathRecord *ferry_paths; /* 50 records */
    int32_t *highest_building, *highest_building_ever, *legacy_highest_building;
    int32_t *anchor_x, *anchor_y, *active_transport_count;
    const int32_t *building_properties;
    int32_t *construction_size;
    PharaohBuildingConstructionAddToMap add_building_to_map;
    void *construction_user_data;
    PharaohCityMessageProblemState *message_problems;
    PharaohCityMessageGetTime get_tick; /* native timeGetTime */
    void *clock_user_data;
    PharaohCityViewContext *view;
    const int32_t *extra_camera_bounds_enabled, *event_54_suppress_flood;
} PharaohSceneRecords;

/* Contexts and buffers remain caller-owned. Grid, invasion and city bounds
 * share the same derived values; required child callbacks have no fallback. */
void pharaoh_saved_game_bind_record_steps(
    PharaohSavedGameInitializeOperations *ops, PharaohSceneRecords *c);

#endif
