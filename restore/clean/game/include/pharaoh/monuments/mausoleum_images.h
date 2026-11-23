#ifndef PHARAOH_MONUMENTS_MAUSOLEUM_IMAGES_H
#define PHARAOH_MONUMENTS_MAUSOLEUM_IMAGES_H

#include "pharaoh/monuments/mausoleum_projection.h"
#include "pharaoh/map/rotated_buildings.h"

typedef struct PharaohMausoleumImageContext {
    PharaohMapSurfaceContext *surface;
    const PharaohMausoleumProjectionContext *projection;
    PharaohWalkerQueryRecord *walkers;
    int16_t *slot_walkers;
    const int32_t *current_walker_id;
    void *user_data;
    uint32_t (*query_image)(void *user_data,int32_t building,uint32_t *flags);
    void (*reset_route)(void *user_data,int32_t walker);
    void (*restart_movement)(void *user_data,int32_t walker);
} PharaohMausoleumImageContext;

void pharaoh_mausoleum_update_map(PharaohMausoleumImageContext *context,
    int32_t building); /* 004DFBB0 */
void pharaoh_mausoleum_reposition_part(PharaohMausoleumImageContext *context,
    int32_t building); /* 004DFD90 */
void pharaoh_rotated_bind_mausoleum_image(PharaohRotatedBuildingOperations *ops,
    PharaohMausoleumImageContext *context);

#endif
