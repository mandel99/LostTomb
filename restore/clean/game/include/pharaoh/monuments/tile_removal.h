#ifndef PHARAOH_MONUMENTS_TILE_REMOVAL_H
#define PHARAOH_MONUMENTS_TILE_REMOVAL_H

#include "pharaoh/monuments/stages.h"

#include <stdint.h>

typedef struct PharaohMonumentPartDesc {
    int32_t size;
    int32_t kind;
} PharaohMonumentPartDesc;

typedef void (*PharaohRemoveBuildingTileFunction)(
    void *user_data, int32_t building_id, int32_t x, int32_t y);
typedef void (*PharaohSetTerrainFunction)(
    void *user_data, int32_t x, int32_t y, uint32_t terrain);

typedef struct PharaohMonumentTileContext {
    PharaohMonumentBuilding *buildings;
    int32_t grid_width;
    int32_t tile_size_x;
    int32_t tile_size_y;
    int32_t open_ground_kind;
    int32_t lighthouse_tower_kind;
    uint32_t monument_rock_terrain;
    uint32_t rock_terrain;
    uint32_t groundwater_terrain;
    uint32_t destroyed_bitfield_keep;
    uint8_t destroyed_edge_keep;
    uint8_t edge_draw_tile;
    uint16_t *building_ids;
    uint32_t *terrain;
    uint32_t *bitfields;
    uint8_t *canals;
    uint8_t *building_damage;
    uint8_t *edge;
    uint16_t *sprite;
    const PharaohMonumentPartDesc *small_tomb_descs;
    const PharaohMonumentPartDesc *medium_tomb_descs;
    const PharaohMonumentPartDesc *large_tomb_descs;
    const PharaohMonumentPartDesc *grand_tomb_descs;
    int32_t small_last_part;
    int32_t medium_last_part;
    int32_t large_last_part;
    int32_t grand_last_part;
    PharaohRemoveBuildingTileFunction remove_tile;
    PharaohSetTerrainFunction set_terrain;
    void *user_data;
} PharaohMonumentTileContext;

void pharaoh_abu_simbel_remove_tiles(PharaohMonumentTileContext *context,
    const PharaohMonumentPartDesc *parts, int32_t building_id);
void pharaoh_library_remove_tiles(PharaohMonumentTileContext *context,
    const PharaohMonumentPartDesc *parts, int32_t building_id);
void pharaoh_caesareum_remove_tiles(PharaohMonumentTileContext *context,
    const PharaohMonumentPartDesc *parts, int32_t building_id);
void pharaoh_lighthouse_remove_tiles(PharaohMonumentTileContext *context,
    const PharaohMonumentPartDesc *parts, int32_t building_id);
void pharaoh_royal_tomb_clear_tiles(PharaohMonumentTileContext *context,
    int32_t building_id, const PharaohMonumentPartDesc *descs,
    int32_t last_part);
void pharaoh_small_royal_tomb_clear_tiles(
    PharaohMonumentTileContext *context, int32_t building_id);
void pharaoh_medium_royal_tomb_clear_tiles(
    PharaohMonumentTileContext *context, int32_t building_id);
void pharaoh_large_royal_tomb_clear_tiles(
    PharaohMonumentTileContext *context, int32_t building_id);
void pharaoh_grand_royal_tomb_clear_tiles(
    PharaohMonumentTileContext *context, int32_t building_id);

#endif
