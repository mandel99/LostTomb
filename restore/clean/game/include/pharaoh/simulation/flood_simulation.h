#ifndef PHARAOH_SIMULATION_FLOOD_SIMULATION_H
#define PHARAOH_SIMULATION_FLOOD_SIMULATION_H

#include <stdint.h>

#include "pharaoh/runtime/random_generator.h"
#include "pharaoh/simulation/flood_settings.h"

typedef struct PharaohBuildingTypeRecord PharaohBuildingTypeRecord;

typedef struct PharaohFloodList {
    int32_t count;
    union {
        uint8_t reserved_04[0x0c];
        struct { uint32_t native_04; uint8_t reserved_08[8]; };
    };
    int32_t head;
    int32_t tail;
    int32_t cursor;
} PharaohFloodList;

typedef struct PharaohFloodTile {
    PharaohFloodList *list;
    void *sorted_link;
    int32_t grid_offset;
    int32_t x;
    int32_t y;
    int32_t depth;
    uint8_t is_floodplain;
    uint8_t padding[3];
} PharaohFloodTile;

/* Complete typed translation of native initializer 0x004BD310. */
void pharaoh_flood_simulation_initialize_scale(double *simulation_scale);
void pharaoh_flood_list_construct(PharaohFloodList *list);
void pharaoh_floodplain_settings_roll_quality(
    PharaohFloodplainSettings *settings,
    const PharaohRandomGeneratorContext *random);
int32_t pharaoh_floodplain_current_quality(
    const PharaohFloodplainSettings *settings,
    const PharaohBuildingTypeRecord *buildings,
    int32_t osiris_complex_building_id,
    int16_t osiris_complex_count); /* 0x004BD590 */
int32_t pharaoh_flood_previous_month(int32_t current_month);
PharaohFloodTile *pharaoh_flood_tile_initialize(
    PharaohFloodTile *tile, int32_t grid_offset, int32_t x, int32_t y,
    int32_t depth, PharaohFloodList *list, uint8_t is_floodplain);

#endif
