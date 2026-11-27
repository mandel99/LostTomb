#ifndef PHARAOH_MAP_NATIVE_LEAF_QUERIES_H
#define PHARAOH_MAP_NATIVE_LEAF_QUERIES_H

#include "pharaoh/building/type_queries.h"

typedef struct PharaohNativeMapLeafContext {
    volatile PharaohBuildingTypeRecord *buildings;
    const volatile int16_t *building_grid;
    const volatile int32_t *width,*height;
    const volatile uint32_t *origin;
    const volatile int32_t *row_tiles,*row_left,*row_right;
    const volatile int32_t *corner_rotation; /* E94E04 */
    const volatile uint32_t *bridge_turn;    /* BC96C8 */
    const volatile uint32_t *terrain;
    volatile uint8_t *bridge_sprites,*markers;
    const volatile uint8_t *bitfields;
    volatile uint32_t *result_x,*result_y,*cursor;
} PharaohNativeMapLeafContext;

typedef struct PharaohNativeDockPlayer {
    int16_t dock_ids[10]; /* EA5CD0 + player*49D8 */
    uint8_t reserved_014[0x49d8-20];
} PharaohNativeDockPlayer;

typedef struct PharaohWaterTargetContext {
    const volatile PharaohWalkerQueryRecord *walkers;
    volatile PharaohBuildingTypeRecord *buildings;
    const volatile int16_t *walker_heads;
    const volatile uint32_t *terrain;
    const volatile int32_t *width,*height;
    const volatile uint32_t *origin,*current_player;
    const volatile int32_t *dock_count; /* actual DWORD705C1C */
    const volatile PharaohNativeDockPlayer *players;
    const volatile int16_t *point_x,*point_y; /* 784B74/84 */
    const int32_t (*perimeter_offsets)[24];
    const int32_t (*coordinate_offsets)[24][2];
    volatile uint32_t *result_x,*result_y,*difference_x,*difference_y;
} PharaohWaterTargetContext;

void pharaoh_map_update_orientation_corners(const PharaohNativeMapLeafContext *c);
void pharaoh_map_rotate_bridge_sprites(const PharaohNativeMapLeafContext *c);
void pharaoh_map_mark_well_access(const PharaohNativeMapLeafContext *c,int32_t building_id,int32_t radius);
void pharaoh_map_native_land_mark(const PharaohNativeMapLeafContext *c,int32_t x,int32_t y,int32_t size,int32_t radius);
void pharaoh_map_native_land_unmark(const PharaohNativeMapLeafContext *c,int32_t x,int32_t y,int32_t size,int32_t radius);

int32_t pharaoh_map_find_permitted_neighbor(const PharaohWaterTargetContext *c,int32_t x,int32_t y,int32_t row);
int32_t pharaoh_dock_claim_position(const PharaohWaterTargetContext *c,int32_t id);
int32_t pharaoh_dock_find_waiting_position(const PharaohWaterTargetContext *c,int32_t id);
int32_t pharaoh_water_find_available_building(const PharaohWaterTargetContext *c,int32_t id);
int32_t pharaoh_water_find_nearest_waypoint(const PharaohWaterTargetContext *c,int32_t id);
int32_t pharaoh_water_space_blocked(const PharaohWaterTargetContext *c,int32_t id);
int32_t pharaoh_water_find_unoccupied_cross(const PharaohWaterTargetContext *c,int32_t id);
uint32_t pharaoh_map_no_fighting_enemy(const PharaohWaterTargetContext *c,int32_t tile);
uint32_t pharaoh_map_no_fighting_friendly(const PharaohWaterTargetContext *c,int32_t tile);
int32_t pharaoh_map_distance_with_penalty(const PharaohWaterTargetContext *c,
    int32_t x1,int32_t y1,int32_t x2,int32_t y2,int32_t region1,int32_t region2);

#endif
