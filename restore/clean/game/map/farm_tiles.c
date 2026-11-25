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

static int32_t flood_farm_tile(const PharaohFarmTileDrawContext *c,
    const PharaohBuildingTypeRecord *building)
{
    const int32_t base = c->surface->map->origin + building->x +
        building->y * 228;
    const int32_t size = building->size;
    switch (*c->surface->orientation) {
    case 0: return base + size - 1;
    case 2: return base;
    case 4: return base + (size - 1) * 228;
    case 6: return base + (size - 1) * 229;
    default: return 0;
    }
}

static int32_t farm_fertility_average(const PharaohFarmTileDrawContext *c,
    int32_t x, int32_t y)
{
    const int32_t origin = c->surface->map->origin + y * 228 + x;
    double divisor = 0.0;
    int32_t sum = 0;
    *c->surface->current_tile = (uint32_t)origin;
    for (int32_t row = 0; row < 3; ++row) {
        for (int32_t column = 0; column < 3; ++column) {
            const uint32_t tile = (uint32_t)origin +
                c->surface->footprints[row][column].grid_offset;
            divisor += 1.0;
            sum += (int32_t)c->surface->fertility[tile] + 1;
        }
    }
    return (int32_t)((double)sum / divisor) + 1;
}

/* Complete typed C translation of Pharaoh.exe 0x00488840. */
void pharaoh_map_farm_tiles_floodplain(const PharaohFarmTileDrawContext *c,
    int32_t building_id, int32_t x, int32_t y, int32_t image,
    int32_t parts, uint8_t extra)
{
    static const int32_t flood_x[9] = {0,1,2,-1,0,1,-2,-1,0};
    static const int32_t flood_y[9] = {-2,-1,0,-1,0,1,0,1,2};
    const PharaohBuildingTypeRecord *building =
        &c->surface->buildings[building_id];
    int32_t tile, sprite, scaled, kind = 0, slot = -1, step;

    if (extra == 0) {
        const int32_t fertility = farm_fertility_average(c, x, y);
        pharaoh_map_place_surface_square(c->surface, (uint16_t)building_id,
            x, y, 3, 3,
            (uint32_t)(fertility / 14 + c->floodplain_farm_image_first), 8);
        return;
    }
    tile = flood_farm_tile(c, building);
    sprite = c->sprites[tile];
    scaled = 10 * parts;
    if (sprite != 0 && building->num_workers > 0) {
        if (scaled < c->farm_max_progress / 5) {
            kind = 1;
            slot = 4;
        } else if (c->flood_state == 1) {
            kind = 3;
            slot = c->surface->random[tile + c->game_day] % 7;
        } else {
            kind = 2;
            slot = (int32_t)(((double)scaled /
                (double)(c->farm_max_progress + 1)) * 9.0);
        }
    }
    step = parts / 10;
    for (int32_t i = 0; i < 9; ++i) {
        const int32_t crop = image + (step < 5 ? step : 5);
        const PharaohSpriteImageRecord *record = pharaoh_sprite_image_record(
            c->draw->images, *c->draw->image_count, crop);
        int32_t px = add_i32(x, flood_x[i] * c->tile_half_width);
        int32_t py = add_i32(y, flood_y[i] * c->tile_half_height);
        py = add_i32(py, c->tile_height * 2 - record->height);
        pharaoh_image_draw(c->draw, (uint32_t)crop, (uint32_t)px,
            (uint32_t)py);
        step -= 2;
        if (step < 0) step = 0;
        if (kind != 0 && slot == i && c->locusts_countdown < 1) {
            int32_t frame, base, worker, worker_x, worker_y;
            if (kind == 1) {
                frame = ((c->game_tick >> 3) + building_id) & 7;
                base = c->laborer_grown_image_first;
            } else if (kind == 2) {
                frame = (c->surface->random[tile] + i) % 7;
                base = c->laborer_work_image_first;
            } else {
                frame = c->surface->random[tile] % 7;
                base = c->laborer_harvest_image_first;
            }
            worker = base + frame + sprite * 8 - 8;
            record = pharaoh_sprite_image_record(c->draw->images,
                *c->draw->image_count, worker);
            worker_x = px + c->tile_half_width - record->sprite_offset_x;
            worker_y = py + c->tile_half_height * 3 -
                record->sprite_offset_y - 5;
            pharaoh_image_draw(c->draw, (uint32_t)worker,
                (uint32_t)worker_x, (uint32_t)worker_y);
        }
    }
}
