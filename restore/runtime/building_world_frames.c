#include "building_world_frames.h"
#include "world_base_model.h"
#include "world_detail_model.h"
#include "world_dispatch_model.h"
#include "world_image_helpers_model.h"
#include "world_metric_helpers_model.h"
#include "world_support_model.h"
#include "world_view_helpers_model.h"
#include "building_food_render.h"
#include "world_render.h"
#include "widget_draw.h"
#include "actor_render.h"
#include "actor_services.h"
#include "actor_teardown_model.h"
#include "menu_building_kind.h"
#include "building_refresh_helpers.h"
#include "sprite_render.h"
#include "graphics_53f250.h"
#include "ground_render.h"
#include "viewport_controller.h"
#include "training_queries.h"
#include "placement_support.h"
#include "menu_prepare_integer.h"
#include "building_controllers.h"
#include "city_prepare.h"
#include "pyramid_widget_draw.h"
#include "camera_navigation.h"
#include <stdlib.h>
extern void FUN_004339c0(uint32_t x,uint32_t y);
extern void FUN_004f0ba0(int32_t x,int32_t y,int32_t *screen_x,int32_t *screen_y);
extern void FUN_004ddec0(void),FUN_004ddf20(void),FUN_004ddf70(void);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
#define A(n) U(sp+4u*(n))

static void frame_food(uint32_t target,struct restore_building_world_frame *f)
{
    RestoreBuildingFoodFrame child;
    child.sp=f->r[4];child.ebx=f->r[3];child.ebp=f->r[5];child.esi=f->r[6];child.edi=f->r[7];
    if(target==0x442280u)f->r[0]=restore_building_food_00442280(&child);
    else restore_building_status_00435e70_frame(&child);
    f->r[4]=child.sp;f->r[3]=child.ebx;f->r[5]=child.ebp;f->r[6]=child.esi;f->r[7]=child.edi;
    /* Native caller continuations discard unspecified volatile outputs;
       the food core alone publishes its full EAX/partial-AL native result. */
}
void restore_building_world_dispatch(uint32_t site,uint32_t target,struct restore_building_world_frame *f)
{
    uint32_t sp=f->r[4],cleanup=0;(void)site;
    switch(target){
    case 0x4dddc0u:restore_building_world_004dddc0(f);return;
    case 0x4fa330u:restore_building_world_004fa330(f);return;
    case 0x431690u:restore_building_world_00431690(f);return;
    case 0x43e300u:restore_building_world_0043e300(f);return;
    case 0x43e420u:restore_building_world_0043e420(f);return;
    case 0x47e230u:f->r[0]=FUN_0047e230(A(1),I(A(2)));break;
    case 0x4b54f0u:restore_building_world_004b54f0(f);return;
    case 0x401f70u:restore_building_world_00401f70(f);return;
    case 0x40f2c0u:restore_building_world_0040f2c0(f);return;
    case 0x430d30u:restore_building_world_00430d30(f);return;
    case 0x430e60u:restore_building_world_00430e60(f);return;
    case 0x432320u:restore_building_world_00432320(f);return;
    case 0x432820u:restore_building_world_00432820(f);return;
    case 0x435050u:restore_building_world_00435050(f);return;
    case 0x435290u:restore_building_world_00435290(f);return;
    case 0x4353a0u:restore_building_world_004353a0(f);return;
    case 0x4354b0u:restore_building_world_004354b0(f);return;
    case 0x435510u:restore_building_world_00435510(f);return;
    case 0x435570u:restore_building_world_00435570(f);return;
    case 0x4355d0u:restore_building_world_004355d0(f);return;
    case 0x435630u:restore_building_world_00435630(f);return;
    case 0x4356a0u:restore_building_world_004356a0(f);return;
    case 0x435700u:restore_building_world_00435700(f);return;
    case 0x435760u:restore_building_world_00435760(f);return;
    case 0x4357c0u:restore_building_world_004357c0(f);return;
    case 0x435820u:restore_building_world_00435820(f);return;
    case 0x435880u:restore_building_world_00435880(f);return;
    case 0x435a10u:restore_building_world_00435a10(f);return;
    case 0x435a70u:restore_building_world_00435a70(f);return;
    case 0x435ac0u:restore_building_world_00435ac0(f);return;
    case 0x435b20u:restore_building_world_00435b20(f);return;
    case 0x435b80u:restore_building_world_00435b80(f);return;
    case 0x435be0u:restore_building_world_00435be0(f);return;
    case 0x435c40u:restore_building_world_00435c40(f);return;
    case 0x435ca0u:restore_building_world_00435ca0(f);return;
    case 0x435d00u:restore_building_world_00435d00(f);return;
    case 0x435d60u:restore_building_world_00435d60(f);return;
    case 0x435de0u:restore_building_world_00435de0(f);return;
    case 0x4360a0u:restore_building_world_004360a0(f);return;
    case 0x436140u:restore_building_world_00436140(f);return;
    case 0x436180u:restore_building_world_00436180(f);return;
    case 0x4361a0u:restore_building_world_004361a0(f);return;
    case 0x436310u:restore_building_world_00436310(f);return;
    case 0x436b60u:restore_building_world_00436b60(f);return;
    case 0x4381b0u:restore_building_world_004381b0(f);return;
    case 0x438210u:restore_building_world_00438210(f);return;
    case 0x438350u:restore_building_world_00438350(f);return;
    case 0x438470u:restore_building_world_00438470(f);return;
    case 0x4384d0u:restore_building_world_004384d0(f);return;
    case 0x438590u:restore_building_world_00438590(f);return;
    case 0x438650u:restore_building_world_00438650(f);return;
    case 0x4386b0u:restore_building_world_004386b0(f);return;
    case 0x438710u:restore_building_world_00438710(f);return;
    case 0x438770u:restore_building_world_00438770(f);return;
    case 0x439f10u:restore_building_world_00439f10(f);return;
    case 0x46f1c0u:restore_building_world_0046f1c0(f);return;
    case 0x47fd30u:restore_building_world_0047fd30(f);return;
    case 0x4c0160u:restore_building_world_004c0160(f);return;
    case 0x4ef550u:restore_building_world_004ef550(f);return;
    case 0x542e10u:restore_building_world_00542e10(f);return;
    case 0x435e70u:case 0x442280u:frame_food(target,f);return;
    /* Complete genuine return-only primitives: C3 and C21000. */
    case 0x51f780u:f->r[4]=sp+4u;return;
    case 0x489500u:f->r[4]=sp+20u;return;
    case 0x488c20u:restore_building_refresh_raw(target,f->r,&f->flags);return;
    case 0x438180u:f->r[0]=(uint32_t)restore_world_image_record((RestoreSpriteLibraries *)(uintptr_t)f->r[1],I(A(1)));cleanup=4;break;
    case 0x4381a0u:f->r[0]=restore_world_image_base((RestoreSpriteLibraries *)(uintptr_t)f->r[1],I(A(1)));cleanup=4;break;
    case 0x53e590u:f->r[0]=(uint32_t)restore_sprite_draw((RestoreSpriteLibraries *)(uintptr_t)f->r[1],I(A(1)),I(A(2)),I(A(3)));cleanup=12;break;
    case 0x53f250u:f->r[0]=(uint32_t)restore_sprite_draw_upper((RestoreSpriteLibraries *)(uintptr_t)f->r[1],I(A(1)),I(A(2)),I(A(3)));cleanup=12;break;
    case 0x488030u:f->r[0]=FUN_00488030(A(1));cleanup=4;break;
    case 0x488050u:f->r[0]=restore_actor_building_gate(I(A(1)));cleanup=4;break;
    case 0x459ef0u:f->r[0]=FUN_00459ef0(I(A(1)));break;
    case 0x475b50u:f->r[0]=FUN_00475b50(A(1));break;
    case 0x4439c0u:f->r[0]=FUN_004439c0(I(A(1)));break;
    case 0x443b90u:f->r[0]=FUN_00443b90(I(A(1)));break;
    case 0x4c53d0u:f->r[0]=(uint32_t)FUN_004c53d0(I(A(1)));break;
    case 0x4cde60u:f->r[0]=(uint32_t)FUN_004cde60(I(A(1)),I(A(2)));break;
    case 0x4f0ba0u:FUN_004f0ba0(I(A(1)),I(A(2)),(int32_t *)(uintptr_t)A(3),(int32_t *)(uintptr_t)A(4));break;
    case 0x430ef0u:FUN_00430ef0();break;
    case 0x4316f0u:FUN_004316f0();break;
    case 0x432d30u:FUN_00432d30(I(A(1)),I(A(2)),I(A(3)),A(4));break;
    case 0x4339c0u:FUN_004339c0(A(1),A(2));break;
    case 0x434aa0u:FUN_00434aa0(I(A(1)),I(A(2)),A(3),A(4));break;
    case 0x438e50u:FUN_00438e50(I(A(1)),A(2),A(3),A(4));break;
    case 0x439740u:FUN_00439740(A(1));break;
    case 0x43b630u:FUN_0043b630();break;
    case 0x41fff0u:FUN_0041fff0(I(A(1)),I(A(2)));break;
    case 0x420050u:FUN_00420050(I(A(1)),I(A(2)));break;
    case 0x4206f0u:FUN_004206f0(I(A(1)),I(A(2)));break;
    case 0x420a30u:FUN_00420a30(I(A(1)),I(A(2)));break;
    case 0x420d60u:FUN_00420d60(I(A(1)),I(A(2)));break;
    case 0x421060u:FUN_00421060(I(A(1)),I(A(2)));break;
    case 0x421920u:FUN_00421920(I(A(1)),I(A(2)),I(A(3)));break;
    case 0x4e5e90u:FUN_004e5e90(I(A(1)),I(A(2)),I(A(3)));break;
    case 0x4cce00u:FUN_004cce00(A(1),(uint8_t)A(2),(const char *)(uintptr_t)A(3),A(4),A(5),A(6),A(7));break;
    case 0x4304c0u:FUN_004304c0(A(1));break;
    case 0x4de080u:FUN_004de080();break;
    case 0x4ddf70u:FUN_004ddf70();break;
    /* These two reviewed camera wrappers also receive the true native tail
       JMP at4FA3A9/4FA3AE. No new call slot is pushed; RET0 consumes the
       original4FA330 return token, so the same sp+4 rule applies. */
    case 0x4ddec0u:FUN_004ddec0();break;
    case 0x4ddf20u:FUN_004ddf20();break;
    default:abort();
    }
    f->r[4]=sp+4u+cleanup;
}
#undef A
void restore_building_world_raw(uint32_t entry,uint32_t registers[8],uint32_t *flags)
{
    struct restore_building_world_frame frame;uint32_t i;
    for(i=0;i<8u;++i)frame.r[i]=registers[i];
    frame.flags=*flags;frame.boundary=restore_building_world_dispatch;
    restore_building_world_dispatch(0,entry,&frame);
    for(i=0;i<8u;++i)registers[i]=frame.r[i];
    *flags=frame.flags;
}
