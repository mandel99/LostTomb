#include "pharaoh/map/farm_tiles.h"

static int32_t add_i32(int32_t a, int32_t b)
{ return (int32_t)((uint32_t)a + (uint32_t)b); }

/* Complete one-callback C translation of Pharaoh.exe 0x00488690. */
void pharaoh_map_farm_tiles_meadow(const PharaohFarmTileDrawContext *c,
    int32_t building_id, int32_t x, int32_t y, int32_t image,
    int32_t parts, uint8_t extra)
{
    static const int32_t meadow_x[5] = {-2,-1,0,1,2};
    static const int32_t meadow_y[5] = {0,1,2,1,0};
    static const int32_t order[5] = {0,4,1,3,2};
    int32_t crops[5];
    int32_t step;

    if (extra == 0) {
        int32_t dry = c->has_water_access(c->user_data, building_id) == 0;
        pharaoh_map_place_surface_square(c->surface, (uint16_t)building_id,
            x, y, 3, 3, (uint32_t)(c->meadow_farm_image_first + dry), 8);
        return;
    }
    step = parts / 10;
    for (int32_t i = 0; i < 5; ++i) {
        crops[i] = image + (step < 4 ? step : 5);
        step -= 4;
        if (step < 0) step = 0;
    }
    for (int32_t i = 0; i < 5; ++i) {
        int32_t part = order[i];
        const PharaohSpriteImageRecord *record = pharaoh_sprite_image_record(
            c->draw->images, *c->draw->image_count, crops[part]);
        int32_t px = add_i32(x, meadow_x[part] * c->tile_half_width);
        int32_t py = add_i32(y, meadow_y[part] * c->tile_half_height);
        py = add_i32(py, c->tile_height * 2 - record->height);
        pharaoh_image_draw(c->draw, (uint32_t)crops[part],
            (uint32_t)px, (uint32_t)py);
    }
}
