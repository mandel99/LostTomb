#include "pharaoh/runtime/camera_scroll.h"
#include "pharaoh/core/static_objects.h"

static int32_t add(int32_t a,int32_t b)
{
    const uint32_t bits=(uint32_t)a+(uint32_t)b;
    return bits<=INT32_MAX?(int32_t)bits:
        (int32_t)((int64_t)bits-INT64_C(0x100000000));
}

/* Reuse of the complete old mixed Unit01771 algorithm, with native cells
   relocated to typed live inputs. No absolute-address or instruction model. */
int32_t pharaoh_camera_scroll_direction(const PharaohCameraScrollInput *c)
{
    if(pharaoh_city_scroll_tick_gate_poll(c->last_ready_tick,
        *c->scroll_speed,c->clock)==0) return 8;
    const int32_t x=*c->cursor_x;
    int west=x<1;
    *c->requested=(uint32_t)west;
    int east=x>=add(*c->screen_width,-1);
    if(east) *c->requested=1;
    const int32_t y=*c->cursor_y;
    int north=y<1;
    if(north) *c->requested=1;
    int south=y>=add(*c->screen_height,-1);
    if(south) *c->requested=1;
    if(*c->west!=0) {west=1;*c->requested=1;}
    if(*c->east!=0) {east=1;*c->requested=1;}
    if(*c->north!=0) {north=1;*c->requested=1;}
    if(*c->south!=0) {south=1;*c->requested=1;}
    *c->east=0;*c->west=0;*c->south=0;*c->north=0;
    if(west) {
        if(north) return 7;
        if(south) return 5;
    }
    if(east) {
        if(north) {
            *c->east=0;*c->west=0;*c->south=0;*c->north=0;
            return 1;
        }
        if(south) return 3;
    }
    if(west) return 6;
    if(north) return 0;
    if(south) return 4;
    return east?2:8;
}

/* Complete old mixed Unit01879 control, using the actual C children. */
void pharaoh_camera_scroll_apply(const PharaohCameraScrollContext *c,int32_t direction)
{
    const int32_t step=*c->large_vertical_step!=0?4:1;
    if(direction==8) return;
    PharaohCityViewState *const s=c->view->state;
    switch(direction) {
    case 0:s->camera_y=add(s->camera_y,-step);break;
    case 1:s->camera_y=add(s->camera_y,-step);s->camera_x=add(s->camera_x,1);break;
    case 2:s->camera_x=add(s->camera_x,1);break;
    case 3:s->camera_y=add(s->camera_y,step);s->camera_x=add(s->camera_x,1);break;
    case 4:s->camera_y=add(s->camera_y,step);break;
    case 5:s->camera_y=add(s->camera_y,step);s->camera_x=add(s->camera_x,-1);break;
    case 6:s->camera_x=add(s->camera_x,-1);break;
    case 7:s->camera_y=add(s->camera_y,-step);s->camera_x=add(s->camera_x,-1);break;
    }
    pharaoh_native_service_records_decay_0041d6f0(c->decay_records);
    pharaoh_city_view_check_camera_boundaries(c->view);
}
