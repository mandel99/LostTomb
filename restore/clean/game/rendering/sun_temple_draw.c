#include "pharaoh/rendering/sun_temple_draw.h"

static int32_t wrapped_add(int32_t a, int32_t b)
{
    return (int32_t)((uint32_t)a + (uint32_t)b);
}

/* Complete one-endpoint C translation of Pharaoh.exe 0x00542E10. */
void pharaoh_sun_temple_draw_stage_blocks(
    const PharaohSunTempleDrawContext *context,
    int32_t building_id, int32_t x, int32_t y)
{
    static const int32_t positions[4][2] = {
        {60, -29}, {187, -29}, {82, -96}, {170, -100}
    };
    int32_t count;

    switch (context->buildings[building_id].monument_stage) {
    default: count = 0; break;
    case 1: case 10: case 11: count = 1; break;
    case 2: case 8: case 9: count = 2; break;
    case 3: case 6: case 7: count = 3; break;
    case 4: case 5: count = 4; break;
    }
    for (int32_t i = 0; i < count; ++i) {
        context->draw_image(context->user_data,
            context->block_image_first + 13 + (i & 1),
            wrapped_add(x, positions[i][0]),
            wrapped_add(y, positions[i][1]));
    }
}
