#ifndef PHARAOH_MONUMENTS_MAUSOLEUM_IMAGES_H
#define PHARAOH_MONUMENTS_MAUSOLEUM_IMAGES_H

#include "pharaoh/monuments/mausoleum_projection.h"
#include "pharaoh/map/rotated_buildings.h"
#include "pharaoh/walker/tile_chain.h"

typedef struct PharaohMausoleumImageContext {
    PharaohMapSurfaceContext *surface;
    const PharaohMausoleumProjectionContext *projection;
    PharaohWalkerQueryRecord *walkers;
    int16_t *slot_walkers;
    const int32_t *current_walker_id;
    void *user_data;
    uint32_t (*query_image)(void *user_data,int32_t building,uint32_t *flags);
    void (*unlink_tile)(void *user_data,int32_t walker);
    void (*link_tile)(void *user_data,int32_t walker);
    const PharaohSpriteLibraryLoadContext *libraries;
    const int32_t *mausoleum_size;
    const uint8_t *animation_set;
    void *query_user_data; /* NULL retains user_data. */
    void *movement_user_data; /* NULL retains user_data. */
} PharaohMausoleumImageContext;

uint32_t pharaoh_mausoleum_query_image(PharaohMausoleumImageContext *context,
    int32_t building,uint32_t *flags); /* 004DF360 */
/* Bind the actual C query/SG3 loader without changing movement userdata. */
void pharaoh_mausoleum_bind_image_query(PharaohMausoleumImageContext *context);
void pharaoh_mausoleum_bind_tile_chains(PharaohMausoleumImageContext *context,
    PharaohWalkerTileChainContext *chains);

void pharaoh_mausoleum_update_map(PharaohMausoleumImageContext *context,
    int32_t building); /* 004DFBB0 */
void pharaoh_mausoleum_reposition_part(PharaohMausoleumImageContext *context,
    int32_t building); /* 004DFD90 */
void pharaoh_rotated_bind_mausoleum_image(PharaohRotatedBuildingOperations *ops,
    PharaohMausoleumImageContext *context);

#endif
