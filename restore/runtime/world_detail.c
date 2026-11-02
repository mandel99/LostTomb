#include "world_detail_model.h"
#include "world_view_helpers_model.h"
#include "world_metric_helpers_model.h"
#include "world_support_model.h"
#include "world_image_helpers_model.h"
#include "world_render.h"
#include "sprite_render.h"
#include "graphics_53f250.h"
#include "ground_render.h"
#include "menu_building_kind.h"
#include "building_status_render.h"
#include "training_queries.h"
#include "menu_prepare_integer.h"
#include "placement_support.h"
#include "actor_teardown_model.h"
#include "city_prepare.h"
#include <stdlib.h>
extern void FUN_004339c0(uint32_t x,uint32_t y);
extern void FUN_00438e50(int32_t actor,uint32_t x,uint32_t y,uint32_t mode);
extern void FUN_004f0ba0(int32_t x,int32_t y,int32_t *screen_x,int32_t *screen_y);
extern uint32_t FUN_00442280(int32_t building,int32_t *tile_slot,int32_t x,int32_t y);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* This closure preserves the producer's owned native locals and the complete
   reviewed metric/image bodies. Do not pull in the unrelated world/frame graph.
   Food remains the existing typed provider: this entry does not manufacture
   inherited frames for442280 or promote its unresolved >8-icon stack reads. */
void restore_world_detail_dispatch(uint32_t site,uint32_t target,struct restore_building_world_frame *f)
{
    uint32_t sp=f->r[4],cleanup=0;(void)site;
#define A(n) U(sp+4u*(n))
#define RAW(token) case 0x##token##u:restore_building_world_##token(f);return
    switch(target){
    RAW(0040f2c0);RAW(00431690);RAW(00432320);RAW(00432820);
    RAW(00435050);RAW(00435290);RAW(004353a0);RAW(004354b0);
    RAW(00435510);RAW(00435570);RAW(004355d0);RAW(00435630);
    RAW(004356a0);RAW(00435700);RAW(00435760);RAW(004357c0);
    RAW(00435820);RAW(00435880);RAW(00435a10);RAW(00435a70);
    RAW(00435ac0);RAW(00435b20);RAW(00435b80);RAW(00435be0);
    RAW(00435c40);RAW(00435ca0);RAW(00435d00);RAW(00435d60);
    RAW(00435de0);RAW(004360a0);RAW(00436140);RAW(00436180);
    RAW(004361a0);RAW(00436310);RAW(004381b0);RAW(00439f10);
    RAW(0046f1c0);RAW(0047fd30);
    case 0x438180u:
        f->r[0]=(uint32_t)restore_world_image_record((RestoreSpriteLibraries *)(uintptr_t)f->r[1],I(A(1)));cleanup=4;break;
    case 0x4381a0u:
        f->r[0]=restore_world_image_base((RestoreSpriteLibraries *)(uintptr_t)f->r[1],I(A(1)));cleanup=4;break;
    case 0x53e590u:
        f->r[0]=(uint32_t)restore_sprite_draw((RestoreSpriteLibraries *)(uintptr_t)f->r[1],I(A(1)),I(A(2)),I(A(3)));cleanup=12;break;
    case 0x53f250u:
        f->r[0]=(uint32_t)restore_sprite_draw_upper((RestoreSpriteLibraries *)(uintptr_t)f->r[1],I(A(1)),I(A(2)),I(A(3)));cleanup=12;break;
    case 0x488030u:f->r[0]=FUN_00488030(A(1));cleanup=4;break;
    case 0x488050u:f->r[0]=restore_actor_building_gate(I(A(1)));cleanup=4;break;
    case 0x459ef0u:f->r[0]=FUN_00459ef0(I(A(1)));break;
    case 0x475b50u:f->r[0]=FUN_00475b50(A(1));break;
    case 0x4439c0u:f->r[0]=FUN_004439c0(I(A(1)));break;
    case 0x443b90u:f->r[0]=FUN_00443b90(I(A(1)));break;
    case 0x4cde60u:f->r[0]=(uint32_t)FUN_004cde60(I(A(1)),I(A(2)));break;
    case 0x4f0ba0u:FUN_004f0ba0(I(A(1)),I(A(2)),(int32_t *)(uintptr_t)A(3),(int32_t *)(uintptr_t)A(4));break;
    case 0x432d30u:FUN_00432d30(I(A(1)),I(A(2)),I(A(3)),A(4));break;
    case 0x4339c0u:FUN_004339c0(A(1),A(2));break;
    case 0x434aa0u:FUN_00434aa0(I(A(1)),I(A(2)),A(3),A(4));break;
    case 0x435e70u:FUN_00435e70(I(A(1)),I(A(2)),I(A(3)));break;
    case 0x438e50u:FUN_00438e50(I(A(1)),A(2),A(3),A(4));break;
    case 0x442280u:
        f->r[0]=FUN_00442280(I(A(1)),(int32_t *)(uintptr_t)A(2),I(A(3)),I(A(4)));break;
    /* Complete native return-only bodies, including489500's RET16. */
    case 0x489500u:cleanup=16;break;
    case 0x51f780u:break;
    default:abort();
    }
    f->r[4]=sp+4u+cleanup;
#undef RAW
#undef A
}

void FUN_00439f10(void)
{
    /* Owned backing for the root's184 local bytes, saved registers, metric
       callees and their call slots. Unwritten scratch is not initialized;
       no original higher caller frame or food offsets are synthesized. */
    uint32_t stack[256],sp=(uint32_t)(uintptr_t)(stack+240);
    struct restore_building_world_frame f={{0},2,restore_world_detail_dispatch};
    stack[240]=0;f.r[4]=sp;
    restore_building_world_00439f10(&f);
    if(f.r[4]!=sp+4u||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
}
