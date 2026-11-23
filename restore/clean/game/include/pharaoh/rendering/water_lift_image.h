#ifndef PHARAOH_RENDERING_WATER_LIFT_IMAGE_H
#define PHARAOH_RENDERING_WATER_LIFT_IMAGE_H

#include <stdint.h>

typedef struct PharaohWaterLiftImageState {
    uint8_t uses_variant;
    uint8_t variant_high;
    int32_t primary_images[4];
    int32_t alternate_images[4];
} PharaohWaterLiftImageState;

int32_t pharaoh_map_water_lift_image(
    const PharaohWaterLiftImageState *state,
    int32_t direction, int32_t alternate_set);

#endif
