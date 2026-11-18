#ifndef PHARAOH_MAP_RING_TYPES_H
#define PHARAOH_MAP_RING_TYPES_H

#include <stddef.h>
#include <stdint.h>

enum {
    PHARAOH_MAP_RING_DISTANCE_COUNT = 12,
    PHARAOH_MAP_RING_BUILDING_SIZE_COUNT = 5,
    PHARAOH_MAP_RING_BUILDING_DISTANCE_COUNT = 6,
    PHARAOH_MAP_RING_DISTANCE_GRID_OFFSET_COUNT = 624,
    PHARAOH_MAP_RING_GRID_WIDTH = 228
};

typedef struct PharaohRingTile {
    int8_t x;
    int8_t y;
} PharaohRingTile;

typedef struct PharaohRingOffsetTables {
    PharaohRingTile distance_12_tiles[96];
    PharaohRingTile distance_11_tiles[88];
    PharaohRingTile distance_10_tiles[80];
    PharaohRingTile distance_9_tiles[72];
    PharaohRingTile distance_8_tiles[64];
    PharaohRingTile distance_7_tiles[56];
    PharaohRingTile distance_6_tiles[48];
    PharaohRingTile distance_5_tiles[40];
    PharaohRingTile distance_4_tiles[32];
    PharaohRingTile distance_3_tiles[24];
    PharaohRingTile distance_2_tiles[16];
    PharaohRingTile distance_1_tiles[8];
    int16_t size_6_offsets[288];
    int16_t size_5_offsets[264];
    int16_t size_4_offsets[240];
    int16_t size_3_offsets[216];
    int16_t size_2_offsets[192];
} PharaohRingOffsetTables;

typedef struct PharaohMapRingInitContext {
    PharaohRingOffsetTables *ring_offsets;
    int16_t *distance_grid_offsets;
} PharaohMapRingInitContext;

_Static_assert(sizeof(PharaohRingTile) == 2,
    "native ring tile coordinates are signed bytes");
_Static_assert(offsetof(PharaohRingOffsetTables, size_6_offsets) == 0x4e0 &&
    offsetof(PharaohRingOffsetTables, size_2_offsets) == 0xcc0 &&
    sizeof(PharaohRingOffsetTables) == 0xe40,
    "ring coordinate and building offset tables preserve native layout");

#endif
