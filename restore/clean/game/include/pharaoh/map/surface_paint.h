#ifndef PHARAOH_MAP_SURFACE_PAINT_H
#define PHARAOH_MAP_SURFACE_PAINT_H

#include "pharaoh/building/construction_undo.h"
#include "pharaoh/map/terrain_queries.h"

typedef struct PharaohSurfacePaintContext {
    PharaohMapSurfaceContext *surface;
    const PharaohConstructionUndoContext *backup;
    uint8_t *cleared; /* Live AE8E7C BYTE grid. */
    const PharaohConstructionEntryPoints *entry_points;
    uint32_t *placed_count; /* Live BC971C DWORD. */
} PharaohSurfacePaintContext;

/* Complete native004754A0/004759E0. The first native physical argument is
 * unused and is omitted from these logical C APIs. Both returnvoid. Borrow
 * the actual shared map, backup and image/footprint tables; no replacement
 * preview surfaces or child callbacks. */
void pharaoh_construction_paint_paved_roads(const PharaohSurfacePaintContext *,
    int32_t left, int32_t top, int32_t right, int32_t bottom);
void pharaoh_construction_paint_gardens(const PharaohSurfacePaintContext *,
    int32_t left, int32_t top, int32_t right, int32_t bottom);

#endif
