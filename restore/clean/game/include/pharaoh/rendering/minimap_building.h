#ifndef PHARAOH_RENDERING_MINIMAP_BUILDING_H
#define PHARAOH_RENDERING_MINIMAP_BUILDING_H
#include "pharaoh/rendering/minimap.h"
#include "pharaoh/building/type_queries.h"

typedef struct PharaohMinimapBuilding {
    PharaohMinimap *map;
    const PharaohBuildingTypeRecord *buildings;
    const uint8_t *footprints;
    const int32_t *type_image_ids; /*237 DWORDs, native5CE008*/
    const int32_t *house_image,*overlay_image,*garden_image,*special_image,*building_image;
    uint16_t *tint;
    void *user_data;
    void *metric_user_data; /* NULL retains user_data */
    /* Actual overlay metric owners indexed by overlay. Required for the
     * recognized non-resource modes; modes27/28 and9-terrain receive tile. */
    int32_t (*height[51])(void *,int32_t building_or_tile);
    int32_t (*terrain_damage)(void *,int32_t tile); /*00433D30*/
    int32_t (*resource_height)(void *,int32_t building,int32_t resource,int32_t slot); /*00433AE0*/
    void (*draw_sprite)(void *,int32_t image,int32_t x,int32_t y);
} PharaohMinimapBuilding;

int32_t pharaoh_minimap_draw_building(PharaohMinimapBuilding *,int32_t building,int32_t x,int32_t y);
void pharaoh_minimap_bind_buildings(PharaohMinimap *,PharaohMinimapBuilding *);
#endif
