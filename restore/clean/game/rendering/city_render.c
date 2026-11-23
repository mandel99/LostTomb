#include "pharaoh/rendering/city_render.h"
#include "pharaoh/runtime/platform_measure_speed.h"

/* Reused viewport-controller C, qualified against native00430D30. */
static void *pass_data(const PharaohCityRenderOperations *ops,
    enum PharaohCityRenderPass pass)
{
    return ops->pass_user_data[pass]!=0?ops->pass_user_data[pass]:ops->user_data;
}

void pharaoh_city_render(PharaohCityRenderContext *c,
    int32_t camera_x, int32_t camera_y, int32_t grid_origin)
{
    PharaohCityRenderState *const s = c->state;
    const PharaohCityRenderOperations *const ops = c->operations;
    s->camera_x = camera_x;
    s->camera_y = camera_y;
    s->grid_origin = grid_origin;
    c->clip->left = c->viewport->screen_x;
    c->clip->right = c->viewport->screen_right;
    c->clip->top = c->viewport->screen_y;
    c->clip->bottom = c->viewport->screen_bottom;
    const uint32_t now = *c->current_tick;
    const uint32_t delta = now - s->last_animation_tick;
    s->advance_animation = 0;
    if ((delta & UINT32_C(0x80000000)) != 0 || delta > 60) {
        s->last_animation_tick = now;
        s->advance_animation = 1;
    }

    int32_t overlay_pass = 0;
    if (*c->overlay == 0) {
        if (*c->alternate_base == 0) ops->draw_base(pass_data(ops, PHARAOH_CITY_PASS_BASE));
        else ops->draw_overlay_base(pass_data(ops, PHARAOH_CITY_PASS_OVERLAY_BASE));
    } else {
        overlay_pass = *c->overlay != 51 && *c->alternate_base == 0;
        ops->draw_overlay_base(pass_data(ops, PHARAOH_CITY_PASS_OVERLAY_BASE));
    }
    if (overlay_pass) {
        pharaoh_actor_walking_no_operation();
        ops->draw_overlay_top(pass_data(ops, PHARAOH_CITY_PASS_OVERLAY_TOP));
        ops->draw_footprint(pass_data(ops, PHARAOH_CITY_PASS_FOOTPRINT));
        ops->draw_high_walkers(pass_data(ops, PHARAOH_CITY_PASS_HIGH_WALKERS), 9999);
    } else {
        ops->draw_top(pass_data(ops, PHARAOH_CITY_PASS_TOP), 0);
        pharaoh_actor_walking_no_operation();
        ops->draw_footprint(pass_data(ops, PHARAOH_CITY_PASS_FOOTPRINT));
        ops->draw_high_walkers(pass_data(ops, PHARAOH_CITY_PASS_HIGH_WALKERS), 0);
    }
    ops->draw_grid_values(pass_data(ops, PHARAOH_CITY_PASS_GRID_VALUES));
    if (s->highlight_countdown != 0) --s->highlight_countdown;
    c->clip->left = 0;
    c->clip->right = *c->screen_width;
    c->clip->top = 0;
    c->clip->bottom = *c->screen_height;
}

/* Native004DDDC0 snapshots all three arguments before entering the renderer. */
void pharaoh_city_render_current_view(PharaohCityRenderContext *c)
{
    const int32_t origin = *c->grid_origin;
    const int32_t y = c->view->camera_y;
    const int32_t x = c->view->camera_x;
    pharaoh_city_render(c, x, y, origin);
}
