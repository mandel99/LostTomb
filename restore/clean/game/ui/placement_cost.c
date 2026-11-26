#include "pharaoh/ui/placement_cost.h"

static int32_t add(int32_t a, int32_t b)
{ return (int32_t)((uint32_t)a + (uint32_t)b); }
static int32_t sub(int32_t a, int32_t b)
{ return (int32_t)((uint32_t)a - (uint32_t)b); }

void pharaoh_placement_cost_draw(const PharaohPlacementCostContext *c)
{
    if (*c->active == 0 || *c->hidden != 0 || *c->cost == 0) return;
    int32_t x, y;
    pharaoh_city_view_tile_to_pixel(c->city->state, c->city->projection,
        c->bounds, *c->tile_x, *c->tile_y, &x, &y);
    const PharaohCityViewportGeometry *const viewport = &c->city->state->viewport;
    c->clip->clip_x_min = viewport->screen_x;
    c->clip->clip_x_max = viewport->screen_right;
    c->clip->clip_y_min = viewport->screen_y;
    c->clip->clip_y_max = viewport->screen_bottom;
    y = add(y, viewport->tile_half_height);
    const int32_t amount = *c->cost;
    const uint32_t affordable = amount <= c->players[*c->current_player].treasury;
    pharaoh_bitmap_text_draw_number_native(c->font, c->text, c->number_text,
        amount, '@', " ", add(add(viewport->tile_width, 1), x),
        add(sub(y, viewport->tile_half_height), 1), *c->font_image, 0);
    const uint16_t color = (uint16_t)(((affordable - 1u) & UINT32_C(0xfffffd3f)) +
        UINT32_C(0xfac1));
    pharaoh_bitmap_text_draw_number_native(c->font, c->text, c->number_text,
        amount, '@', " ", add(viewport->tile_width, x),
        sub(y, viewport->tile_half_height), *c->font_image, color);
    *c->cost = 0;
    c->clip->clip_x_min = 0;
    c->clip->clip_x_max = *c->screen_width;
    c->clip->clip_y_min = 0;
    c->clip->clip_y_max = *c->screen_height;
}
