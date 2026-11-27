#ifndef PHARAOH_MAP_CONSTRUCTION_FOOTPRINT_H
#define PHARAOH_MAP_CONSTRUCTION_FOOTPRINT_H
#include "pharaoh/map/surface_tiles.h"
#include "pharaoh/building/temple_images.h"

typedef struct PharaohConstructionFootprintTables {
    uint32_t offsets_size2[4]; /* 5CCCC8 */
    uint32_t water_size2[4][4]; /* 5DBDA8 */
    int32_t water_borders[3][4][2]; /* 5DBDE8/5DBE08/5DBE28 */
} PharaohConstructionFootprintTables;
extern const PharaohConstructionFootprintTables pharaoh_construction_footprint_tables;

typedef struct PharaohConstructionFootprintContext {
    PharaohMapSurfaceContext *surface;
    uint32_t *rows, *columns; /* BC9738/BC973C, same live scratch as all callers. */
    int32_t *placement_ok; /* 605994 */
    const uint16_t *blocking_objects; /* B350DC WORDs */
    int32_t *raw_orientation, *adjusted_orientation; /* BC96D8/BC96DC */
    const PharaohConstructionFootprintTables *tables;
    const PharaohTempleImageTables *temple_tables;
} PharaohConstructionFootprintContext;

void pharaoh_construction_mark_current_footprint(PharaohConstructionFootprintContext *c);
void pharaoh_construction_mark_footprint(PharaohConstructionFootprintContext *c,
    int32_t size,int32_t x,int32_t y,uint32_t mask,int32_t fixed,uint8_t allow);
int32_t pharaoh_construction_footprint_is_free(PharaohConstructionFootprintContext *c,
    int32_t size,int32_t x,int32_t y,uint32_t mask,uint8_t allow);
int32_t pharaoh_map_water_determine_orientation_size2(PharaohConstructionFootprintContext *c,
    int32_t x,int32_t y,uint32_t rotate,uint8_t allow);
void pharaoh_gatehouse_mark_construction(PharaohConstructionFootprintContext *c,
    int32_t x,int32_t y,uint32_t layout);
#endif
