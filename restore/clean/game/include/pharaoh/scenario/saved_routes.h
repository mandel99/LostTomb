#ifndef PHARAOH_SCENARIO_SAVED_ROUTES_H
#define PHARAOH_SCENARIO_SAVED_ROUTES_H
#include "pharaoh/scenario/saved_game_state_read.h"

typedef struct PharaohRouteMapObject {
    uint8_t kind, in_use, reserved_02[2];
    int16_t x, y;
    uint8_t reserved_08[4];
    uint16_t image;
    uint8_t reserved_0e[84];
} PharaohRouteMapObject;

typedef struct PharaohRouteImage {
    uint8_t reserved_00[12];
    int16_t width, height;
    uint8_t reserved_10[48];
} PharaohRouteImage;

typedef struct PharaohSavedRouteBindings {
    void *user_data;
    PharaohChunkAllocate allocate;
    PharaohChunkFree release;
    const PharaohRouteMapObject *objects; /*200 native records */
    const PharaohRouteImage *images;
    int32_t image_count;
} PharaohSavedRouteBindings;

/* Native004D4720. Logical return is AL; callers do not consume upper EAX. */
uint8_t pharaoh_saved_route_rebuild(PharaohEditorRoute *route,
    const PharaohSavedRouteBindings *bindings);
void pharaoh_saved_game_bind_route_samples(PharaohSavedGameReadOperations *ops,
    PharaohSavedRouteBindings *bindings);
#endif
