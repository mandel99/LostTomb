#ifndef PHARAOH_MONUMENTS_PYRAMID_DEMOLITION_H
#define PHARAOH_MONUMENTS_PYRAMID_DEMOLITION_H

#include "pharaoh/map/surface_tiles.h"

typedef struct PharaohPyramidDemolitionContext {
    PharaohMapSurfaceContext *surface;
    uint32_t *placed_count; /* BC971C, shared construction counter. */
} PharaohPyramidDemolitionContext;

/* Original read-only DWORD tables5700D4/5700E8. Regular size0..4 and
 * mastaba size0..2 are the native producer domains; no invented clamp. */
extern const int32_t pharaoh_pyramid_auxiliary_offsets_regular[5];
extern const int32_t pharaoh_pyramid_auxiliary_offsets_mastaba[3];

/* Whole004F4930. Uses the actual mode5 segment updater and live placement
 * EAAE84 output terminal_tile(EAAE9C), followed by all genuine refreshes. */
void pharaoh_pyramid_clear_complex_temple(
    const PharaohPyramidDemolitionContext *,int32_t building);
/* Whole004F5E70. Native consumes only the reserve argument's BYTE. The real
 * building undo and live IDs/record pool must share the surface context. */
void pharaoh_pyramid_update_auxiliary_reservation(
    const PharaohPyramidDemolitionContext *,int32_t building,uint8_t reserve);

#endif
