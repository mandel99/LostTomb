#ifndef PHARAOH_RENDERING_TEMPLE_ANNEX_DRAW_H
#define PHARAOH_RENDERING_TEMPLE_ANNEX_DRAW_H

#include "pharaoh/building/type_queries.h"
#include "pharaoh/rendering/image_draw.h"

typedef struct PharaohTempleAnnexPart {
    int32_t on_road;
    int32_t size;
    int32_t dy;
    int32_t dx;
} PharaohTempleAnnexPart;

typedef struct PharaohTempleAnnexDrawContext {
    const PharaohBuildingTypeRecord *buildings;
    const PharaohTempleAnnexPart *parts;
    const int32_t *orientation_parts; /* eight rows of four indices */
    const int32_t *god_image_groups;
    const int32_t *image_group_first;
    PharaohImageDrawContext *draw;
    int32_t map_orientation;
    int32_t statue_image_first;
    uint16_t *tint;
    void *user_data;
    int32_t (*god_with_status)(void *user_data,int32_t status);
    void (*project_tile)(void *user_data,int32_t x,int32_t y,
        int32_t *pixel_x,int32_t *pixel_y);
} PharaohTempleAnnexDrawContext;

void pharaoh_temple_complex_draw_annex(
    PharaohTempleAnnexDrawContext *context,int32_t building_id); /*004C0160*/

#endif
