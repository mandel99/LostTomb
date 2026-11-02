#include "world_support_model.h"
#include "world_image_helpers_model.h"
#include "sprite_render.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

/* These complete stock/animation bodies have one external provider:
   thiscall53E590/RET12. Keep this closure separate from world dispatch. */
static void stock_sprite(uint32_t site,uint32_t target,struct restore_building_world_frame *f)
{
    uint32_t sp=f->r[4];(void)site;
    if(target!=0x53e590u)abort();
    f->r[0]=(uint32_t)restore_sprite_draw((RestoreSpriteLibraries *)(uintptr_t)f->r[1],
        (int32_t)U(sp+4u),(int32_t)U(sp+8u),(int32_t)U(sp+12u));
    f->r[4]=sp+16u;
}

static uint32_t image_call(void (*body)(struct restore_building_world_frame *),
                           const uint32_t *args,uint32_t count)
{
    /* The largest owned local table is64 bytes. Native variants0..3 (and
       the fishing actor's second four-pair block) consume no caller array. */
    uint32_t stack[64],sp=(uint32_t)(uintptr_t)(stack+48),i;
    struct restore_building_world_frame f={{0},2,stock_sprite};
    stack[48]=0;for(i=0;i<count;++i)stack[49+i]=args[i];
    f.r[4]=sp;
    body(&f);
    if(f.r[4]!=sp+4u||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
    return f.r[0];
}
void FUN_00436310(int32_t building,int32_t x,int32_t y)
{const uint32_t a[]={(uint32_t)building,(uint32_t)x,(uint32_t)y};(void)image_call(restore_building_world_00436310,a,3);}
void FUN_00438210(int32_t image,int32_t frame,int32_t x,int32_t y,uint32_t actor_tag)
{const uint32_t a[]={(uint32_t)image,(uint32_t)frame,(uint32_t)x,(uint32_t)y,actor_tag};(void)image_call(restore_building_world_00438210,a,5);}
int32_t FUN_00438350(int32_t image,int32_t frame,int32_t x,int32_t y,int32_t actor)
{const uint32_t a[]={(uint32_t)image,(uint32_t)frame,(uint32_t)x,(uint32_t)y,(uint32_t)actor};return (int32_t)image_call(restore_building_world_00438350,a,5);}
#define ANIMATION4(address) \
int32_t FUN_##address(int32_t image,int32_t frame,int32_t x,int32_t y) \
{const uint32_t a[]={(uint32_t)image,(uint32_t)frame,(uint32_t)x,(uint32_t)y};return (int32_t)image_call(restore_building_world_##address,a,4);}
ANIMATION4(00438470)
ANIMATION4(004384d0)
ANIMATION4(00438590)
ANIMATION4(00438650)
ANIMATION4(004386b0)
ANIMATION4(00438710)
ANIMATION4(00438770)
#undef ANIMATION4
