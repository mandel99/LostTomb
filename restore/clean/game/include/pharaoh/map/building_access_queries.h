#ifndef PHARAOH_MAP_BUILDING_ACCESS_QUERIES_H
#define PHARAOH_MAP_BUILDING_ACCESS_QUERIES_H
#include "pharaoh/building/type_queries.h"
#include "pharaoh/map/road_network_types.h"
#include "pharaoh/map/grid_geometry.h"

typedef struct PharaohRoadAccessMap {
    const uint32_t *terrain;
    const int32_t *routing_distance;
    const uint8_t *region_color;
    const PharaohRoadNetworkRank *preferred_regions;
    const int32_t (*perimeter_offsets)[24];
    const int32_t (*coordinate_offsets)[24][2];
    int32_t origin;
    int32_t *result_x,*result_y;
    int32_t width,height;
    int32_t *last_tile;
} PharaohRoadAccessMap;

typedef struct PharaohMastabaEntranceRow {
    int32_t entry;
    uint8_t reserved_04[124];
} PharaohMastabaEntranceRow;
typedef struct PharaohPyramidEntranceRow {
    int32_t entry;
    uint8_t reserved_04[956];
} PharaohPyramidEntranceRow;
typedef struct PharaohPyramidEntranceDirection {
    int32_t direction;
    int32_t native_04;
} PharaohPyramidEntranceDirection;
typedef struct PharaohSpecialEntryMap {
    const PharaohBuildingTypeRecord *buildings;
    const PharaohMastabaEntranceRow *mastaba_entries;
    const PharaohPyramidEntranceRow *pyramid_entries;
    const PharaohPyramidEntranceDirection *layout_directions;
    const int32_t *pyramid_style,*origin;
} PharaohSpecialEntryMap;

uint32_t pharaoh_building_find_access(const PharaohRoadAccessMap *map,
    int32_t x,int32_t y,uint32_t size);
uint32_t pharaoh_building_find_wide_access(const PharaohRoadAccessMap *map,
    int32_t x,int32_t y,uint32_t size);
uint32_t pharaoh_building_has_access_neighbor(const PharaohRoadAccessMap *map,
    int32_t x,int32_t y,uint32_t size);
uint32_t pharaoh_map_closest_road_within_radius(const PharaohRoadAccessMap *map,
    int32_t x,int32_t y,uint32_t size,int32_t radius,uint32_t skip);
uint32_t pharaoh_map_road_exists_within_distance(const PharaohRoadAccessMap *map,
    int32_t x,int32_t y,uint32_t size,int32_t distance);
uint8_t pharaoh_building_special_entry(const PharaohSpecialEntryMap *map,
    int32_t building_id,uint32_t *tile,int32_t *x,int32_t *y);
int32_t pharaoh_map_sphinx_access_tile(int32_t map_grid_start,
    int32_t x,int32_t y,int32_t *result_x,int32_t *result_y);
#endif
