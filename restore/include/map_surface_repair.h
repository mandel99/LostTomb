#ifndef RESTORE_MAP_SURFACE_REPAIR_H
#define RESTORE_MAP_SURFACE_REPAIR_H
#include "surface_passes.h"
/* Terrain0x20: two-pass sprite reset, then 3x3/2x2/single-tile grouping. */
void restore_map_refresh_garden_tiles(void);
/* Terrain0x4000: neighbor-pattern and road-crossing image selection. */
void restore_map_refresh_canal_tiles(void);
#endif
