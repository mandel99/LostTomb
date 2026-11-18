#include "pharaoh/runtime/map_ring.h"

typedef struct PharaohRingCoordinateList {
    const PharaohRingTile *tiles;
    int16_t count;
} PharaohRingCoordinateList;

#include "map_ring_coordinates.inc"

static const int16_t pharaoh_distance_ring_counts[
    PHARAOH_MAP_RING_DISTANCE_COUNT] = {
        8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96
    };

static PharaohRingTile *distance_tiles_at(PharaohRingOffsetTables *tables,
                                           int32_t distance_index)
{
    PharaohRingTile *const distance_tiles[
        PHARAOH_MAP_RING_DISTANCE_COUNT] = {
            tables->distance_1_tiles,
            tables->distance_2_tiles,
            tables->distance_3_tiles,
            tables->distance_4_tiles,
            tables->distance_5_tiles,
            tables->distance_6_tiles,
            tables->distance_7_tiles,
            tables->distance_8_tiles,
            tables->distance_9_tiles,
            tables->distance_10_tiles,
            tables->distance_11_tiles,
            tables->distance_12_tiles
        };
    return distance_tiles[distance_index];
}

void pharaoh_map_ring_generate_tiles(PharaohRingOffsetTables *tables)
{
    for (int32_t distance_index = 0;
         distance_index < PHARAOH_MAP_RING_DISTANCE_COUNT;
         ++distance_index) {
        const int32_t distance = distance_index + 1;
        PharaohRingTile *tile = distance_tiles_at(tables, distance_index);
        for (int32_t y = -distance; y <= distance; ++y) {
            for (int32_t x = -distance; x <= distance; ++x) {
                if (x == -distance || x == distance ||
                    y == -distance || y == distance) {
                    tile->x = (int8_t)x;
                    tile->y = (int8_t)y;
                    ++tile;
                }
            }
        }
    }
}

void pharaoh_map_ring_initialize(const PharaohMapRingInitContext *context)
{
    PharaohRingOffsetTables *const tables = context->ring_offsets;
    pharaoh_map_ring_generate_tiles(tables);

    int16_t *distance_offset = context->distance_grid_offsets;
    for (int32_t distance_index = 0;
         distance_index < PHARAOH_MAP_RING_DISTANCE_COUNT;
         ++distance_index) {
        const int16_t count = pharaoh_distance_ring_counts[distance_index];
        const PharaohRingTile *const tiles =
            distance_tiles_at(tables, distance_index);
        for (int32_t tile_index = 0; tile_index < count; ++tile_index) {
            *distance_offset++ = (int16_t)(tiles[tile_index].x +
                tiles[tile_index].y * PHARAOH_MAP_RING_GRID_WIDTH);
        }
    }

    int16_t *size_offsets[PHARAOH_MAP_RING_BUILDING_SIZE_COUNT] = {
        tables->size_2_offsets,
        tables->size_3_offsets,
        tables->size_4_offsets,
        tables->size_5_offsets,
        tables->size_6_offsets
    };
    for (int32_t size_index = 0;
         size_index < PHARAOH_MAP_RING_BUILDING_SIZE_COUNT; ++size_index) {
        int16_t *offset = size_offsets[size_index];
        for (int32_t distance_index = 0;
             distance_index < PHARAOH_MAP_RING_BUILDING_DISTANCE_COUNT;
             ++distance_index) {
            const PharaohRingCoordinateList *const list =
                &pharaoh_building_ring_coordinates[size_index][distance_index];
            for (int32_t tile_index = 0; tile_index < list->count; ++tile_index) {
                *offset++ = (int16_t)(list->tiles[tile_index].x +
                    list->tiles[tile_index].y * PHARAOH_MAP_RING_GRID_WIDTH);
            }
        }
    }
}
