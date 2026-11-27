#ifndef PHARAOH_BUILDING_CONSTRUCTION_UNDO_H
#define PHARAOH_BUILDING_CONSTRUCTION_UNDO_H
#include "pharaoh/map/surface_tiles.h"

typedef struct PharaohConstructionUndoContext {
    PharaohMapBuildingUndo *buildings; /*live4000,backup525,ids525,countBC9684/availableBC9694*/
    const int32_t *current_tool;
    int32_t *saved_tool; /*BC9690*/
    uint32_t *costs,*value_bc95f0,*value_bc9688,*value_bc968c;
    const uint32_t *live_images,*live_terrain;
    uint32_t *saved_images,*saved_terrain;
    /* Native copy order AE8E7C,B285CC,B67D1C,ADC36C to their undo owners. */
    const uint8_t *live_bytes[4];
    uint8_t *saved_bytes[4];
} PharaohConstructionUndoContext;

/* Complete00484F60. Actual aligned native buffers required. State2 aborts
   after clearing; state6 disables availability but still copies/returns1. */
uint32_t pharaoh_construction_backup_map(const PharaohConstructionUndoContext *);
#endif
