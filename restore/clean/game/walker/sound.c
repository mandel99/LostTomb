#include "pharaoh/walker/sound.h"
#include "pharaoh/core/x87_numeric.h"

#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Native inherited x87 evaluation requires MSVC Win32.
#endif

/* Immutable remap at53B614, restricted by the native unsigned0..90 guard. */
static int offscreen_sound(uint8_t kind)
{
    switch(kind){
    case 11:case 12:case 13:case 43:case 44:case 45:case 54:case 55:
    case 56:case 77:case 78:case 92:case 93:case 99:case 100:case 101:return 1;
    default:return 0;
    }
}

static int32_t pan_position(int32_t width,int32_t x)
{
    const float span=128.0f;
    PharaohFloat80 product;
    int32_t result;
    __asm {
        fild width
        fdivr dword ptr span
        fild x
        fmul st(0),st(1)
        lea eax,product
        fstp tbyte ptr [eax]
    }
    result=(int32_t)pharaoh_x87_truncate80(&product);
    /* Native leaves the width ratio live until after the truncation call. */
    __asm { fstp st(0) }
    return result;
}

/* Reuse current/old ActorSound.c. Preserve live global read timing and the
 * real incoming coordinate slots: native tile_to_pixel may leave them alone. */
void pharaoh_walker_sound_play(const PharaohWalkerSoundContext *c,
    int32_t walker_id,int32_t sound,int32_t variant)
{
    int32_t width,height,fullscreen,x=sound,y=walker_id,sample,volume;
    int quiet=0;
    if(!*c->enabled||sound>=33||variant>=5||!*c->effects_enabled)return;
    fullscreen=*c->fullscreen;
    width=*c->fullscreen_width;
    if(!fullscreen)width=*c->window_width;
    height=*c->screen_height;
    pharaoh_city_view_tile_to_pixel(c->city_view,c->projection,c->map_bounds,
        c->walkers[walker_id].x,c->walkers[walker_id].y,&x,&y);
    if(x<0||y<0||x>width||y>height){
        if(!offscreen_sound(c->walkers[walker_id].type))return;
        quiet=1;
    }
    sample=c->sample_ids[(int32_t)((uint32_t)sound*5u+(uint32_t)variant)];
    if(sample<=-1)return;
    width=*c->fullscreen?*c->fullscreen_width:*c->window_width;
    c->request->pan=pan_position(width,x);
    volume=*c->volume;
    c->request->volume=quiet?volume/4:volume;
    c->request->file_id=sample;
    c->request->owner=-1;
    c->request->loop_count=1;
    (void)pharaoh_digital_sound_play_sample(c->device,c->operations,c->request);
}

void pharaoh_walker_combat_sound(const PharaohWalkerSoundContext *c,
    int32_t walker_id)
{
    switch(c->walkers[walker_id].type){
    case 11:case 43:case 55:pharaoh_walker_sound_play(c,walker_id,11,2);break;
    case 12:case 44:case 54:pharaoh_walker_sound_play(c,walker_id,12,2);break;
    case 13:case 45:case 56:case 99:pharaoh_walker_sound_play(c,walker_id,13,2);break;
    case 78:case 93:case 100:pharaoh_walker_sound_play(c,walker_id,17,2);break;
    default:break;
    }
}
