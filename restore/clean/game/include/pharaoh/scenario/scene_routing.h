#ifndef PHARAOH_SCENARIO_SCENE_ROUTING_H
#define PHARAOH_SCENARIO_SCENE_ROUTING_H

#include "pharaoh/scenario/saved_game_initialize.h"
#include "pharaoh/building/type_queries.h"
#include "pharaoh/simulation/flood_simulation.h"

typedef struct PharaohSceneRouting {
    const PharaohSavedGameGeometry *geometry;
    uint32_t *terrain;
    const int16_t *building_ids;
    const PharaohBuildingTypeRecord *buildings;
    const uint8_t *tile_direction, *random;
    uint32_t *images;
    uint8_t *edges, *footprints;
    uint8_t *citizen, *noncitizen, *water, *walls;
    PharaohFloodTile *const *flood_tiles;
    const uint32_t *cleared_building_image, *flood_image_base;
    const int32_t *map_orientation, *city_orientation;
    uint32_t *buildings_without_id_cleared;
    void *ferry_user_data;
    void (*update_ferry_tiles)(void *user_data, int32_t building,
        uint8_t enabled, int32_t marker); /* 0043F060 */
    void (*detach_ferry)(void *user_data, int32_t building); /* 004A36B0 */
} PharaohSceneRouting;

/* The native geometry has width+grid_border=228. Arrays include the native
 * border cells, and all referenced building/flood records must be live. */
void pharaoh_map_routing_update_water_rect(
    const uint32_t *terrain, const uint8_t *tile_direction,
    uint8_t *water_routing, int32_t map_origin, int32_t map_width,
    int32_t map_height, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void pharaoh_map_routing_update_land_noncitizen_rect(
    const uint32_t *terrain, const int16_t *building_ids,
    const PharaohBuildingTypeRecord *buildings, uint8_t *routing,
    int32_t map_origin, int32_t map_width, int32_t map_height,
    int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void pharaoh_map_routing_update_citizen_rect(PharaohSceneRouting *c,
    int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t update_links);
void pharaoh_map_routing_initialize_citizen(PharaohSceneRouting *c);
void pharaoh_map_routing_initialize_noncitizen(PharaohSceneRouting *c);
void pharaoh_map_routing_initialize_water(PharaohSceneRouting *c);
void pharaoh_map_routing_initialize_walls(PharaohSceneRouting *c);
void pharaoh_saved_game_bind_route_steps(
    PharaohSavedGameInitializeOperations *operations, PharaohSceneRouting *c);

#endif
