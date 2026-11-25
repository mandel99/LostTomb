#include "pharaoh/rendering/obelisk_draw.h"

static int32_t add_i32(int32_t a, int32_t b)
{ return (int32_t)((uint32_t)a + (uint32_t)b); }

/* Complete three-endpoint C translation of Pharaoh.exe 0x004E5E90. */
void pharaoh_obelisk_draw_overlay(const PharaohObeliskDrawContext *c,
    int32_t building_id, int32_t x, int32_t y)
{
    const int32_t small = c->buildings[building_id].building_type == 207;
    const PharaohMonumentWorkSpotBinding *spots =
        small ? c->small_spots : c->large_spots;
    const int32_t spot_count = small ? 10 : 18;
    const int32_t width = small ? 1 : 6;
    int32_t amount = 0;
    int32_t dx = small ? 90 : 152;
    int32_t dy = small ? 18 : 14;
    uint8_t progress = c->progress(c->user_data, building_id);
    float drawn = 0.0f;
    float step = small ? 25.0f : 12.5f;

    if (progress >= 100) return;
    if (progress >= 66) amount = (100 - progress) * 3;
    else if (progress >= 33) amount = progress * 3 - 99;
    if (progress >= 33 && amount > 100) amount = 100;
    x = add_i32(x, dx);
    y = add_i32(y, dy);
    for (int32_t count = 0; drawn < (float)amount; ++count) {
        int32_t column = (count & 1) != 0 ? width : -60 - width;
        c->operations->draw_image(c->operations->user_data,
            c->ground_image_first, add_i32(x, column),
            add_i32(y, -((count / 2 + 1) * 60) - width / 2));
        drawn += step;
    }
    for (int32_t i = 0; i < spot_count; ++i) {
        int32_t frame_x, frame_y, image;
        pharaoh_monument_spot_get_draw_info(c->spot_draw, spots[i].spot,
            &frame_x, &frame_y, &image);
        if (image != 0)
            c->operations->draw_image(c->operations->user_data, image,
                add_i32(x, frame_x), add_i32(y, frame_y));
    }
}
