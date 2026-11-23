#ifndef PHARAOH_MONUMENTS_IMAGE_UPDATES_H
#define PHARAOH_MONUMENTS_IMAGE_UPDATES_H

#include "pharaoh/map/rotated_buildings.h"

typedef struct PharaohMonumentImagePart {
    int32_t dx,dy,size,kind,grid_delta;
    int32_t view_x[4],view_y[4];
} PharaohMonumentImagePart;

typedef struct PharaohMonumentImageContext {
    PharaohMapSurfaceContext *surface;
    const PharaohMonumentImagePart *parts; /* descriptor base; size at +8 */
    const PharaohSunTemplePartDefinition *sun_parts; /* size-based view, stride52;
        native00601DE0 rather than the descriptor base00601DD8 */
    const int32_t *city_overlay,*last_part;
    uint32_t *idle;
    PharaohMapRockVariantContext *rock;
    void *user_data;
    uint32_t (*query_image)(void *user_data,int32_t building,uint32_t *flags);
    uint8_t (*workers_idle)(void *user_data,int32_t building);
    void *query_user_data; /* NULL retains user_data. */
} PharaohMonumentImageContext;

void pharaoh_monument_update_image_parts(PharaohMonumentImageContext *c,int32_t building); /* 004017A0 */
void pharaoh_library_update_map(PharaohMonumentImageContext *c,int32_t building); /* 0040DCA0 */
void pharaoh_lighthouse_update_map(PharaohMonumentImageContext *c,int32_t building); /* 004E69C0 */
void pharaoh_sun_temple_update_map(PharaohMonumentImageContext *c,int32_t building); /* 00541920 */
void pharaoh_royal_tomb_update_map(PharaohMonumentImageContext *c,int32_t building); /* 005468D0 */

enum PharaohMonumentImageMethod {
    PHARAOH_IMAGE_PARTS, PHARAOH_IMAGE_LIBRARY, PHARAOH_IMAGE_LIGHTHOUSE,
    PHARAOH_IMAGE_SUN_TEMPLE, PHARAOH_IMAGE_ROYAL_TOMB
};
PharaohBuildingImageBinding pharaoh_monument_image_binding(
    enum PharaohMonumentImageMethod method,PharaohMonumentImageContext *context);

/* The eight receiver contexts follow PharaohRotatedMonument order. Other
 * rotation operations and each context's image query remain caller-owned. */
void pharaoh_rotated_bind_monument_images(PharaohRotatedBuildingOperations *ops,
    PharaohMonumentImageContext *const contexts[PHARAOH_ROTATED_MONUMENT_COUNT]);
void pharaoh_rotated_bind_sun_temple_image(PharaohRotatedBuildingOperations *ops,
    PharaohMonumentImageContext *context);

#endif
