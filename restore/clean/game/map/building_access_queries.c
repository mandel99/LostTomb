#include "pharaoh/map/building_access_queries.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohMastabaEntranceRow)==128 &&
    sizeof(PharaohPyramidEntranceRow)==960 &&
    sizeof(PharaohPyramidEntranceDirection)==8,
    "native pyramid entrance table strides");

/* Reused typed terrain queries and prior mixed/BuildingAccessSearch C.
 * Native481060 and481210 retain rank11 and the100000 fallback cutoff. */
static int32_t road_region_rank(
    const PharaohRoadNetworkRank *preferred_regions, uint8_t color)
{
    int32_t rank;
    for (rank = 0; rank < 10; ++rank) {
        if (preferred_regions[rank].id == color) return rank;
    }
    return 11;
}

/* Complete native00481060. */
uint32_t pharaoh_building_find_access(const PharaohRoadAccessMap *map,
    int32_t x, int32_t y, uint32_t size)
{
    const int32_t base = (int32_t)((uint32_t)map->origin + (uint32_t)y * PHARAOH_MAP_GRID_WIDTH + (uint32_t)x);
    int32_t best_rank = 12;
    int32_t best_index = 0;
    int32_t index;
    for (index = 0; index < 24; ++index) {
        const int32_t delta = map->perimeter_offsets[size][index];
        int32_t tile;
        int32_t rank;
        if (delta == 0) break;
        tile = (int32_t)((uint32_t)base + (uint32_t)delta);
        if ((map->terrain[tile] & 0x40u) == 0 ||
            (map->terrain[tile] & 4u) != 0 || map->routing_distance[tile] <= 0)
            continue;
        rank = road_region_rank(map->preferred_regions, map->region_color[tile]);
        if (rank < best_rank) { best_rank = rank; best_index = index; }
    }
    if (best_rank < 12) {
        *map->result_x = x + map->coordinate_offsets[size][best_index][0];
        *map->result_y = y + map->coordinate_offsets[size][best_index][1];
        return (uint32_t)(base + map->perimeter_offsets[size][best_index] + 1);
    }
    {
        int32_t best_distance = 100000;
        int32_t best_tile = -1;
        int32_t out_x = x;
        int32_t out_y = y;
        for (index = 0; index < 24; ++index) {
            const int32_t delta = map->perimeter_offsets[size][index];
            int32_t tile;
            int32_t distance;
            if (delta == 0) break;
            tile = (int32_t)((uint32_t)base + (uint32_t)delta);
            distance = map->routing_distance[tile];
            if (distance > 0 && distance < best_distance) {
                best_distance = distance;
                best_tile = tile;
                out_x = x + map->coordinate_offsets[size][index][0];
                out_y = y + map->coordinate_offsets[size][index][1];
            }
        }
        if (best_tile < 0) return 0;
        *map->result_x = out_x;
        *map->result_y = out_y;
        return (uint32_t)(best_tile + 1);
    }
}

/* Complete native00481210. Fallback deliberately adds the winning shift twice. */
uint32_t pharaoh_building_find_wide_access(const PharaohRoadAccessMap *map,
    int32_t x, int32_t y, uint32_t size)
{
    const int32_t base = (int32_t)((uint32_t)map->origin + (uint32_t)y * PHARAOH_MAP_GRID_WIDTH + (uint32_t)x);
    int32_t best_rank = 12;
    int32_t best_index = 0;
    int32_t best_shift = 0;
    int32_t shift;
    for (shift = 0; shift <= 6; shift += 3) {
        int32_t index;
        for (index = 0; index < 24; ++index) {
            const int32_t delta = map->perimeter_offsets[size][index];
            int32_t tile;
            int32_t rank;
            if (delta == 0) break;
            tile = (int32_t)((uint32_t)base + (uint32_t)shift + (uint32_t)delta);
            if ((map->terrain[tile] & 0x40u) == 0 || map->routing_distance[tile] <= 0)
                continue;
            rank = road_region_rank(map->preferred_regions, map->region_color[tile]);
            if (rank < best_rank) {
                best_rank = rank; best_index = index; best_shift = shift;
            }
        }
    }
    if (best_rank < 12) {
        *map->result_x = x + best_shift +
            map->coordinate_offsets[size][best_index][0];
        *map->result_y = y + map->coordinate_offsets[size][best_index][1];
        return (uint32_t)(base + best_shift +
            map->perimeter_offsets[size][best_index] + 1);
    }
    {
        int32_t best_distance = 100000;
        int32_t best_tile = -1;
        int32_t winning_shift = 0;
        int32_t out_x = x;
        int32_t out_y = y;
        for (shift = 0; shift <= 6; shift += 3) {
            int32_t index;
            for (index = 0; index < 24; ++index) {
                const int32_t delta = map->perimeter_offsets[size][index];
                int32_t tile;
                int32_t distance;
                if (delta == 0) break;
                tile = (int32_t)((uint32_t)base + (uint32_t)shift + (uint32_t)delta);
                distance = map->routing_distance[tile];
                if (distance > 0 && distance < best_distance) {
                    best_distance = distance;
                    best_tile = tile;
                    winning_shift = shift;
                    out_x = x + shift + map->coordinate_offsets[size][index][0];
                    out_y = y + map->coordinate_offsets[size][index][1];
                }
            }
        }
        if (best_tile < 0) return 0;
        *map->result_y = out_y;
        *map->result_x = out_x + winning_shift;
        return (uint32_t)(best_tile + winning_shift + 1);
    }
}

/* Native00487D00 returns a zero-based tile and stores Y before X. */
int32_t pharaoh_map_sphinx_access_tile(int32_t map_grid_start,
    int32_t x, int32_t y, int32_t *result_x, int32_t *result_y)
{
    const uint32_t access_x = (uint32_t)x + 9u;
    const uint32_t access_y = (uint32_t)y + 6u;
    *result_y = (int32_t)access_y;
    *result_x = (int32_t)access_x;
    return (int32_t)((uint32_t)map_grid_start +
        access_y * PHARAOH_MAP_GRID_WIDTH + access_x);
}

/* Native004F30B0: the sole direct caller458034 tests AL; full EAX has
 * incidental pointer/base high bits. No indirect-entry coverage is claimed. */
uint8_t pharaoh_building_special_entry(const PharaohSpecialEntryMap *map,
    int32_t building_id, uint32_t *tile, int32_t *x, int32_t *y)
{
    uint32_t root=(uint32_t)building_id-
        (uint32_t)(int32_t)map->buildings[building_id].subtype;
    uint32_t entry;
    if (*map->pyramid_style==5) {
        entry=root+(uint32_t)map->mastaba_entries[
            map->buildings[root].pyramid_size_variant].entry;
        *x=map->buildings[entry].x;
        *y=(int32_t)((uint32_t)(int32_t)map->buildings[entry].y+2u);
    } else {
        entry=root+(uint32_t)map->pyramid_entries[
            map->buildings[root].pyramid_size_variant].entry;
        const uint32_t index=
            (uint32_t)map->buildings[entry].pyramid_size_variant*144u+
            (uint32_t)(int32_t)map->buildings[entry].subtype;
        const int32_t direction=map->layout_directions[index].direction;
        *x=map->buildings[entry].x;
        *y=map->buildings[entry].y;
        if (direction==4) *x=(int32_t)((uint32_t)*x-1u);
        else if (direction==5) *y=(int32_t)((uint32_t)*y+2u);
        else if (direction==6) *x=(int32_t)((uint32_t)*x+2u);
        else if (direction==7) *y=(int32_t)((uint32_t)*y-1u);
    }
    *tile=(uint32_t)*y*PHARAOH_MAP_GRID_WIDTH+(uint32_t)*x+
        (uint32_t)*map->origin;
    return 1;
}

/* Native00481420 tests the low terrain BYTE and a nonzero DWORD distance. */
uint32_t pharaoh_building_has_access_neighbor(
    const PharaohRoadAccessMap *map,int32_t x,int32_t y,uint32_t size)
{
    const uint32_t base=(uint32_t)map->origin+
        (uint32_t)y*PHARAOH_MAP_GRID_WIDTH+(uint32_t)x;
    for (uint32_t i=0;i<24;++i) {
        const uint32_t delta=(uint32_t)map->perimeter_offsets[size][i];
        if (delta==0) return 0;
        const uint32_t tile=base+delta;
        if ((map->terrain[tile]&4u)!=0 && map->routing_distance[tile]!=0)
            return 1;
    }
    return 0;
}

/* Complete native0047FEB0; reused CommandMigration C. Only the first five
 * ranked networks are consulted; an unmatched road still qualifies at11. */
uint32_t pharaoh_map_closest_road_within_radius(const PharaohRoadAccessMap *map,
    int32_t x,int32_t y,uint32_t size,int32_t radius,uint32_t skip)
{
    uint32_t left=(uint32_t)x-(uint32_t)radius;
    uint32_t top=(uint32_t)y-(uint32_t)radius;
    uint32_t right=(uint32_t)x+size+(uint32_t)radius-1u;
    uint32_t bottom=(uint32_t)y+size+(uint32_t)radius-1u;
    if ((int32_t)left<0) left=0;
    if ((int32_t)top<0) top=0;
    if ((int32_t)right>=map->width) right=(uint32_t)map->width-1u;
    if ((int32_t)bottom>=map->height) bottom=(uint32_t)map->height-1u;
    uint32_t best=12,best_x=left,best_y=top;
    uint32_t tile=(uint32_t)map->origin+top*228u+left,best_tile=tile;
    *map->result_y=(int32_t)top;
    *map->last_tile=(int32_t)tile;
    if ((int32_t)top>(int32_t)bottom) return 0;
    do {
        uint32_t column=left;
        *map->result_x=(int32_t)column;
        while ((int32_t)column<=(int32_t)right) {
            if ((map->terrain[tile]&0x44u)==0x40u && skip==0) {
                uint32_t rank=11;
                for (uint32_t i=0;i<5;++i)
                    if (map->preferred_regions[i].id==map->region_color[tile]) {
                        rank=i;break;
                    }
                if (rank<best) {
                    best=rank;best_tile=tile;best_x=column;best_y=top;
                }
            }
            ++column;++tile;
            *map->result_x=(int32_t)column;
            *map->last_tile=(int32_t)tile;
        }
        ++top;tile+=left-right+227u;
        *map->result_y=(int32_t)top;
        *map->last_tile=(int32_t)tile;
    } while ((int32_t)top<=(int32_t)bottom);
    if (best>=12) return 0;
    *map->last_tile=(int32_t)best_tile;
    *map->result_x=(int32_t)best_x;*map->result_y=(int32_t)best_y;
    return 1;
}

/* Complete native0047FE60; reused CleanGeneratedLeaves65 C. */
uint32_t pharaoh_map_road_exists_within_distance(const PharaohRoadAccessMap *map,
    int32_t x,int32_t y,uint32_t size,int32_t distance)
{
    for (uint32_t radius=1;(int32_t)radius<=distance;++radius)
        if (pharaoh_map_closest_road_within_radius(map,x,y,size,
                (int32_t)radius,0)!=0) return 1;
    return 0;
}
