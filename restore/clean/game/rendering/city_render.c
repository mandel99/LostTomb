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
    const PharaohCityRenderLiveBindings *const live=c->live;
    uint32_t *const target_x=live?live->camera_x:(uint32_t *)(void *)&s->camera_x;
    uint32_t *const target_y=live?live->camera_y:(uint32_t *)(void *)&s->camera_y;
    uint32_t *const target_origin=live?live->grid_origin:(uint32_t *)(void *)&s->grid_origin;
    uint32_t *const last_tick=live?live->last_animation_tick:(uint32_t *)(void *)&s->last_animation_tick;
    uint32_t *const advance=live?live->advance_animation:(uint32_t *)(void *)&s->advance_animation;
    uint32_t *const highlight=live?live->highlight_countdown:(uint32_t *)(void *)&s->highlight_countdown;
    int32_t *const left=live?live->clip_left:&c->clip->left;
    int32_t *const right=live?live->clip_right:&c->clip->right;
    int32_t *const top=live?live->clip_top:&c->clip->top;
    int32_t *const bottom=live?live->clip_bottom:&c->clip->bottom;
    *target_x = (uint32_t)camera_x;
    *target_y = (uint32_t)camera_y;
    *target_origin = (uint32_t)grid_origin;
    *left = c->viewport->screen_x;
    *right = c->viewport->screen_right;
    *top = c->viewport->screen_y;
    *bottom = c->viewport->screen_bottom;
    const uint32_t now = *c->current_tick;
    const uint32_t delta = now - *last_tick;
    *advance = 0;
    if ((delta & UINT32_C(0x80000000)) != 0 || delta > 60) {
        *last_tick = now;
        *advance = 1;
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
    if (*highlight != 0) --*highlight;
    *left = 0;
    *right = *c->screen_width;
    *top = 0;
    *bottom = *c->screen_height;
}

/* Native004DDDC0 snapshots all three arguments before entering the renderer. */
void pharaoh_city_render_current_view(PharaohCityRenderContext *c)
{
    const int32_t origin = *c->grid_origin;
    const int32_t y = c->view->camera_y;
    const int32_t x = c->view->camera_x;
    pharaoh_city_render(c, x, y, origin);
}

int32_t pharaoh_city_render_bound_for_current_mode(const PharaohCityRenderContext *c)
{
    if(!c||!c->viewport||!c->view||!c->grid_origin||!c->current_tick||
       !c->overlay||!c->alternate_base||!c->screen_width||!c->screen_height||!c->operations)return 0;
    if(c->live) {
        const PharaohCityRenderLiveBindings *const l=c->live;
        if(!l->camera_x||!l->camera_y||!l->grid_origin||!l->clip_left||!l->clip_right||
           !l->clip_top||!l->clip_bottom||!l->last_animation_tick||!l->advance_animation||
           !l->highlight_countdown)return 0;
    } else if(!c->state||!c->clip)return 0;
    const PharaohCityRenderOperations *const o=c->operations;
    if(!o->draw_footprint||!o->draw_high_walkers||!o->draw_grid_values)return 0;
    if(*c->overlay==0&&*c->alternate_base==0)return o->draw_base&&o->draw_top;
    if(!o->draw_overlay_base)return 0;
    if(*c->overlay!=0&&*c->overlay!=51&&*c->alternate_base==0)return o->draw_overlay_top!=0;
    return o->draw_top!=0;
}
