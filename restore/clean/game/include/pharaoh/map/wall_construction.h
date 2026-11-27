#ifndef PHARAOH_MAP_WALL_CONSTRUCTION_H
#define PHARAOH_MAP_WALL_CONSTRUCTION_H

#include "pharaoh/building/construction_undo.h"
#include "pharaoh/map/terrain_queries.h"
#include "pharaoh/scenario/scene_routing.h"

typedef struct PharaohWallTilePlacementContext {
    PharaohMapSurfaceContext *surface;
    uint32_t *placed_count, *upgrade_credit; /* BC971C/BC9718. */
    const int32_t *base_price, *first_upgrade_price; /* 705F94/7078E4. */
} PharaohWallTilePlacementContext;

typedef struct PharaohWallConstructionContext {
    const PharaohWallTilePlacementContext *tiles;
    const PharaohConstructionUndoContext *backup;
    uint8_t *cleared; /* AE8E7C. */
    const PharaohConstructionEntryPoints *entry_points;
    PharaohSceneRouting *routing;
    uint8_t *map_changed; /* E38DCA. */
} PharaohWallConstructionContext;

/* Whole native0047AD20, logicalvoid. Genuine mutable image-context tables and
 * cursor/selection/adjacency scratch must share the live surface backing. */
void pharaoh_map_set_construction_wall_tile(
    const PharaohWallTilePlacementContext *,int32_t x,int32_t y,
    uint32_t place,uint32_t mask);
/* Whole native00475220. mode0 commits; other values still perform placement
 * and its real local image refresh but skip the final routing rebuild. */
void pharaoh_construction_place_wall_strip(
    const PharaohWallConstructionContext *,int32_t mode,
    int32_t x1,int32_t y1,int32_t x2,int32_t y2,uint32_t mask);

#endif
