#ifndef PHARAOH_BUILDING_VENUE_IMAGES_H
#define PHARAOH_BUILDING_VENUE_IMAGES_H
#include <stdint.h>
void pharaoh_venue_orientation_images(int32_t view_orientation,
    const uint32_t *base_image,int32_t which,int32_t *main_image,int32_t *next_image);
struct PharaohMapSurfaceContext;
struct PharaohRotatedBuildingOperations;
/* Native callers provide direction0/1 and even orientations0..6. */
void pharaoh_map_publish_venue_adjacent_images(struct PharaohMapSurfaceContext *surface,
    int32_t building,int32_t x,int32_t y); /* 00440540 */
void pharaoh_rotated_bind_venue_adjacent_images(struct PharaohRotatedBuildingOperations *ops,
    struct PharaohMapSurfaceContext *surface);
#endif
