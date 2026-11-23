#ifndef PHARAOH_RENDERING_TERRAIN_DIAMOND_H
#define PHARAOH_RENDERING_TERRAIN_DIAMOND_H
#include "pharaoh/rendering/primitives.h"

typedef struct PharaohTerrainRasterTarget {
    uint8_t *surface_pixels;
    int32_t pitch_bytes, x, y;
    uint16_t tint;
} PharaohTerrainRasterTarget;

/* Six native PUSHAD/POPAD leaves; their consumers discard EAX. Source is one
 * packed1800-byte diamond. Clip3 selects the lower half,4 the upper half. */
void pharaoh_terrain_diamond_draw(const PharaohTerrainRasterTarget *,const uint8_t *,int32_t); /*00427A20 */
void pharaoh_terrain_diamond_mask(const PharaohTerrainRasterTarget *,const uint8_t *,int32_t); /*00428C00 */
void pharaoh_terrain_diamond_draw_left(const PharaohTerrainRasterTarget *,const uint8_t *,int32_t); /*0042A160 */
void pharaoh_terrain_diamond_mask_left(const PharaohTerrainRasterTarget *,const uint8_t *,int32_t); /*0042AA00 */
void pharaoh_terrain_diamond_draw_right(const PharaohTerrainRasterTarget *,const uint8_t *,int32_t); /*0042B440 */
void pharaoh_terrain_diamond_mask_right(const PharaohTerrainRasterTarget *,const uint8_t *,int32_t); /*0042BCF0 */

typedef struct PharaohTerrainFootprintContext {
    PharaohSpriteClipState *clip;
    const uint8_t *pixels; /* resolved pixel base plus native image offset */
    uint8_t *surface_pixels;
    int32_t pitch_bytes;
    uint16_t tint;
} PharaohTerrainFootprintContext;

int32_t pharaoh_terrain_footprint_draw2(PharaohTerrainFootprintContext *); /*0042C740 */
int32_t pharaoh_terrain_footprint_draw3(PharaohTerrainFootprintContext *); /*0042C9A0 */
int32_t pharaoh_terrain_footprint_draw4(PharaohTerrainFootprintContext *); /*0042CEE0 */
int32_t pharaoh_terrain_footprint_draw5(PharaohTerrainFootprintContext *); /*0042D810 */
int32_t pharaoh_terrain_footprint_draw6(PharaohTerrainFootprintContext *); /*0042E680 */
#endif
