#include "world_special_buildings.h"
#include "world_support_model.h"
#include "sprite_render.h"
#include "graphics_53f250.h"
#include "actor_render.h"
#include "building_controllers.h"
#include <stdlib.h>

extern void FUN_004f0ba0(int32_t x,int32_t y,int32_t *screen_x,int32_t *screen_y);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

static void special_boundary(uint32_t site,uint32_t target,struct restore_building_world_frame *f)
{
    uint32_t sp=f->r[4],cleanup=0;(void)site;
#define A(n) U(sp+4u*(n))
    switch(target){
    case 0x4c53d0u:f->r[0]=(uint32_t)FUN_004c53d0(I(A(1)));break;
    case 0x4f0ba0u:
        FUN_004f0ba0(I(A(1)),I(A(2)),(int32_t *)(uintptr_t)A(3),(int32_t *)(uintptr_t)A(4));break;
    case 0x438e50u:FUN_00438e50(I(A(1)),A(2),A(3),A(4));break;
    case 0x53e590u:
        f->r[0]=(uint32_t)restore_sprite_draw((RestoreSpriteLibraries *)(uintptr_t)f->r[1],I(A(1)),I(A(2)),I(A(3)));cleanup=12;break;
    case 0x53f250u:
        f->r[0]=(uint32_t)restore_sprite_draw_upper((RestoreSpriteLibraries *)(uintptr_t)f->r[1],I(A(1)),I(A(2)),I(A(3)));cleanup=12;break;
    default:abort();
    }
    f->r[4]=sp+4u+cleanup;
#undef A
}

static void special_call(void (*body)(struct restore_building_world_frame *),
                         const uint32_t *args,uint32_t count,uint32_t cleanup)
{
    /* Largest owned root locals: Abu Simbel's140 bytes, plus saved registers
       and call slots. No inherited arrays or higher caller bytes are read. */
    uint32_t stack[128],sp=(uint32_t)(uintptr_t)(stack+112),i;
    struct restore_building_world_frame f={{0},2,special_boundary};
    stack[112]=0;for(i=0;i<count;++i)stack[113+i]=args[i];
    f.r[4]=sp;body(&f);
    if(f.r[4]!=sp+4u+cleanup||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
}

void FUN_00401f70(int32_t building,int32_t x,int32_t y)
{
    const uint32_t a[]={(uint32_t)building,(uint32_t)x,(uint32_t)y};
    special_call(restore_building_world_00401f70,a,3,12);
}
void FUN_004c0160(int32_t building)
{
    const uint32_t a[]={(uint32_t)building};
    special_call(restore_building_world_004c0160,a,1,0);
}
void FUN_004ef550(uint32_t image,int32_t x,int32_t y,int32_t building)
{
    const uint32_t a[]={image,(uint32_t)x,(uint32_t)y,(uint32_t)building};
    special_call(restore_building_world_004ef550,a,4,0);
}
void FUN_00542e10(int32_t building,int32_t x,int32_t y)
{
    const uint32_t a[]={(uint32_t)building,(uint32_t)x,(uint32_t)y};
    special_call(restore_building_world_00542e10,a,3,0);
}
