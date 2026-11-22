#ifndef PHARAOH_MAP_SURFACE_IMAGE_TABLES_H
#define PHARAOH_MAP_SURFACE_IMAGE_TABLES_H
#include "pharaoh/map/image_context.h"
typedef struct PharaohSurfaceImageTables {
    PharaohMapImageContextRecord fault[17];
    PharaohMapImageContextRecord terrain_4000[48];
    PharaohMapImageContextRecord terrain_4000_flood[15];
    PharaohMapImageContextRecord floodplain[26];
    PharaohMapImageContextRecord channel[16];
} PharaohSurfaceImageTables;
/* Initial process-lifetime data. Use a mutable copy and retain its frame
 * bytes across updates and mission loads, as the original globals do. */
extern const PharaohSurfaceImageTables pharaoh_surface_image_initial_tables;
struct PharaohMapSurfaceContext;
void pharaoh_map_surface_bind_image_tables(struct PharaohMapSurfaceContext *context,
    PharaohSurfaceImageTables *tables);
#endif
