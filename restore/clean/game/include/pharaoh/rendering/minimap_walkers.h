#ifndef PHARAOH_RENDERING_MINIMAP_WALKERS_H
#define PHARAOH_RENDERING_MINIMAP_WALKERS_H
#include "pharaoh/rendering/minimap.h"
#include "pharaoh/walker/record_queries.h"

/* Strided signed-WORD view begins at nativeBCBA4A, not at formation base.
 * This is a field view; it does not assert an unqualified formation layout. */
typedef struct PharaohMinimapFormationStatus {
    int16_t value;
    uint8_t unobserved[142];
} PharaohMinimapFormationStatus;
typedef struct PharaohMinimapWalkers {
    const PharaohWalkerQueryRecord *walkers;
    const PharaohMinimapFormationStatus *formation_status;
    const int32_t *overlay;
    void *user_data;
    int32_t (*walker_matches_overlay)(void *,int32_t walker); /*004387D0*/
} PharaohMinimapWalkers;

int32_t pharaoh_minimap_walker_land_kind(const PharaohMinimapWalkers *,int32_t walker);
int32_t pharaoh_minimap_walker_other_kind(const PharaohMinimapWalkers *,int32_t walker);
void pharaoh_minimap_bind_walkers(PharaohMinimap *,PharaohMinimapWalkers *);
void pharaoh_minimap_bind_pixel_pair(PharaohMinimap *,PharaohGraphicsPrimitives *);
#endif
