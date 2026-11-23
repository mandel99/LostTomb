#ifndef PHARAOH_MAP_BUILDING_TILES_H
#define PHARAOH_MAP_BUILDING_TILES_H
#include "pharaoh/map/surface_tiles.h"

typedef struct PharaohMapBuildingTilesContext {
    PharaohMapSurfaceContext *surface;
    const PharaohSunTemplePartDefinition *sun_parts;
    uint32_t *rows,*columns; /* native BC9738/BC973C, live after children */
    uint8_t *native_a9010c,*native_adc36c; /* native byte grids */
    uint16_t *native_a9cc1c; /* native WORD grid, distinct from pointer units */
} PharaohMapBuildingTilesContext;

uint32_t pharaoh_map_building_tiles_top_left(PharaohMapBuildingTilesContext *c,
    int32_t x,int32_t y); /* 0047C7B0 */
void pharaoh_map_clear_building_tiles(PharaohMapBuildingTilesContext *c,
    int32_t building,int32_t x,int32_t y); /* 0047C480 */

typedef struct PharaohHouseImageData {
    uint32_t normal_variants[20],image_groups[20],merged_variants[20];
} PharaohHouseImageData;
extern const PharaohHouseImageData pharaoh_house_image_initial_data;
void pharaoh_house_change_type_image(PharaohMapSurfaceContext *c,
    const PharaohHouseImageData *data,int32_t building,int16_t kind); /* 0046CEA0 */
#endif
