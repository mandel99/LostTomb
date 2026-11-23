#ifndef PHARAOH_BUILDING_ALLOCATION_H
#define PHARAOH_BUILDING_ALLOCATION_H
#include "pharaoh/building/house_health.h"
#include "pharaoh/building/occupancy_query.h"
#include "pharaoh/map/building_tiles.h"
#include "pharaoh/runtime/city_warning.h"

/* Normalized fields +0/+4 of native20-byte definition rows, not a native overlay. */
typedef struct PharaohBuildingAllocationDefinition { uint8_t size,native_04; }
    PharaohBuildingAllocationDefinition;
extern const PharaohBuildingAllocationDefinition pharaoh_building_allocation_definitions[237];
typedef struct PharaohBuildingAllocationUndo {
    const int32_t *ready,*mode,*count;
    const int16_t *ids; /*525 entries*/
} PharaohBuildingAllocationUndo;
typedef struct PharaohBuildingAllocationContext {
    PharaohMapSurfaceContext *surface;
    const PharaohScenarioPlayerRuntime *players;
    const PharaohHouseModel *house_models;
    const PharaohBuildingAllocationDefinition *definitions;
    const int32_t (*water_patterns)[24];
    const uint32_t *current_player,*rotation;
    uint32_t *next_serial,*highest_id,*ferry_state,*first_ferry;
    PharaohBuildingAllocationUndo undo;
    PharaohBuildingRepairQueueEntry *repair_queue;
    PharaohBuildingRepairDebugOutput output_debug;
    void *debug_user;
    PharaohHouseHealthContext *health;
    PharaohCityWarningContext *warnings;
    PharaohOccupancyState *occupancy;
} PharaohBuildingAllocationContext;

int32_t pharaoh_building_allocate(PharaohBuildingAllocationContext *c,
    uint32_t owner,uint32_t kind,int32_t x,int32_t y,
    int32_t contiguous,int32_t selected); /*0046A400*/
void pharaoh_house_change_to_vacant_lots(PharaohBuildingAllocationContext *c,
    PharaohMapBuildingTilesContext *tiles,int32_t building); /*0046CF40*/
#endif
