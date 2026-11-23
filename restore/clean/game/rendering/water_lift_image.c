#include "pharaoh/rendering/water_lift_image.h"

/* Complete C translation of Pharaoh.exe 0x00487D30. */
int32_t pharaoh_map_water_lift_image(
    const PharaohWaterLiftImageState *state,
    int32_t direction, int32_t alternate_set)
{
    int32_t image_index;
    uint32_t variant = 0;
    if (state->uses_variant != 0) {
        variant = state->variant_high != 0 ? 8u : 4u;
    }
    if (direction == 0) image_index = 0;
    else if (direction == 1) image_index = 1;
    else if (direction == 2) image_index = 2;
    else image_index = 3;
    const int32_t base = (uint8_t)alternate_set != 0 ?
        state->alternate_images[image_index] :
        state->primary_images[image_index];
    return (int32_t)((uint32_t)base + variant);
}
