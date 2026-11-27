#include "pharaoh/map/surface_paint.h"
#include "pharaoh/building/construction_commit_support.h"
#include "pharaoh/core/memory_strings.h"
#include "pharaoh/simulation/flood_simulation.h"
#include <stddef.h>

_Static_assert(offsetof(PharaohFloodTile, is_floodplain) == 0x18,
    "native garden transient-object BYTE predicate");

static void restore_paint_map(const PharaohSurfacePaintContext *c)
{
    PharaohMapSurfaceContext *s = c->surface;
    pharaoh_memory_copy_forward_words64(c->backup->saved_terrain,
        s->map->terrain, 0x32c40);
    pharaoh_memory_copy_forward_words64(c->backup->saved_bytes[0],
        c->cleared, 0xcb10);
    pharaoh_memory_copy_forward_words64(c->backup->saved_bytes[1],
        s->property_flags, 0xcb10);
    pharaoh_memory_copy_forward_words64(c->backup->saved_bytes[2],
        s->edge_flags, 0xcb10);
    pharaoh_map_restore_free_tile_images(s, c->backup->saved_images);
}

static void sort_paint_bounds(uint32_t *left, uint32_t *top,
    uint32_t *right, uint32_t *bottom)
{
    uint32_t swap;
    if ((int32_t)*left > (int32_t)*right) {
        swap = *left; *left = *right; *right = swap;
    }
    if ((int32_t)*top > (int32_t)*bottom) {
        swap = *top; *top = *bottom; *bottom = swap;
    }
}

static void clip_paint_bounds(const PharaohMapTerrainView *map,
    uint32_t *left, uint32_t *top, uint32_t *right, uint32_t *bottom)
{
    if ((int32_t)*left < 0) *left = 0;
    if ((int32_t)*top < 0) *top = 0;
    if ((int32_t)*right >= map->width) *right = (uint32_t)map->width - 1u;
    if ((int32_t)*bottom >= map->height) *bottom = (uint32_t)map->height - 1u;
}

void pharaoh_construction_paint_paved_roads(const PharaohSurfacePaintContext *c,
    int32_t left_in, int32_t top_in, int32_t right_in, int32_t bottom_in)
{
    PharaohMapSurfaceContext *s = c->surface;
    uint32_t left = (uint32_t)left_in, top = (uint32_t)top_in;
    uint32_t right = (uint32_t)right_in, bottom = (uint32_t)bottom_in;
    sort_paint_bounds(&left, &top, &right, &bottom);
    restore_paint_map(c);
    clip_paint_bounds(s->map, &left, &top, &right, &bottom);
    *c->placed_count = 0;
    uint32_t width = (uint32_t)s->map->width;
    const uint32_t tail = width - right - 1u + left;
    uint32_t cell = (uint32_t)s->map->origin + top * 228u + left;
    if ((int32_t)top <= (int32_t)bottom) {
        uint32_t rows = bottom - top + 1u;
        do {
            if ((int32_t)left <= (int32_t)right) {
                uint32_t columns = right - left + 1u;
                do {
                    const uint32_t flags = s->map->terrain[cell];
                    /* Native47B8E0 writes full EAX0/1 on every path;
                     * widening its logical-byte C result is therefore exact
                     * at this native TEST EAX consumer. */
                    if ((flags & 0x40u) != 0 && (flags & 0xcu) == 0 &&
                        s->building_ids[cell] == 0 &&
                        pharaoh_map_tile_is_buildable_for_editor_at(
                            s->map->terrain, s->height_values, (int32_t)cell) != 0) {
                        const uint8_t old = s->property_flags[cell];
                        if ((old & 0x80u) == 0) ++*c->placed_count;
                        s->image_ids[cell] = 0;
                        s->property_flags[cell] = (uint8_t)((old & 0xf0u) | 0x80u);
                        s->edge_flags[cell] |= 0x40u;
                    }
                    ++cell;
                } while (--columns != 0);
                width = (uint32_t)s->map->width;
            }
            cell += (uint32_t)*s->border_size + tail;
        } while (--rows != 0);
    }
    pharaoh_map_refresh_paved_region(s, 0, 0, (int32_t)(width - 1u),
        (int32_t)((uint32_t)s->map->height - 1u), 1);
}

void pharaoh_construction_paint_gardens(const PharaohSurfacePaintContext *c,
    int32_t left_in, int32_t top_in, int32_t right_in, int32_t bottom_in)
{
    PharaohMapSurfaceContext *s = c->surface;
    uint32_t left = (uint32_t)left_in, top = (uint32_t)top_in;
    uint32_t right = (uint32_t)right_in, bottom = (uint32_t)bottom_in;
    sort_paint_bounds(&left, &top, &right, &bottom);
    restore_paint_map(c);
    clip_paint_bounds(s->map, &left, &top, &right, &bottom);
    const uint32_t tail = (uint32_t)s->map->width - right - 1u + left;
    uint32_t cell = (uint32_t)s->map->origin + top * 228u + left;
    *c->placed_count = 0;
    for (uint32_t y = top; (int32_t)y <= (int32_t)bottom; ++y) {
        for (uint32_t x = left; (int32_t)x <= (int32_t)right; ++x) {
            if ((s->map->terrain[cell] & 0xeeffd77fu) == 0 &&
                (s->flood_tiles[cell] == NULL ||
                    s->flood_tiles[cell]->is_floodplain != 0) &&
                (uint8_t)pharaoh_construction_area_has_entry_point(
                    c->entry_points, (int32_t)x, (int32_t)y,
                    (int32_t)x, (int32_t)y) == 0) {
                ++*c->placed_count;
                s->map->terrain[cell] |= 0x20u;
            }
            ++cell;
        }
        cell += (uint32_t)*s->border_size + tail;
    }
    pharaoh_map_refresh_gardens(s);
}
