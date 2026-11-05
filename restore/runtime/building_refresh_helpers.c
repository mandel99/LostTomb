#include "actor_value_contracts.h"
#include "building_refresh_helpers.h"
#include "building_image.h"
#include "building_allocator.h"
#include "building_support.h"
#include "building_widgets.h"
#include "city_footprint_validation.h"
#include "map_commit_population.h"
#include "menu_building_kind.h"
#include "menu_actor_message.h"
#include "sprite_render.h"
#include "road_place.h"
#include "terrain_redraw.h"
#include "map_refresh_postpass1.h"
#include "editor_brush_validation.h"
#include "building_graph.h"
#include <stdlib.h>
extern void FUN_00475bf0(int32_t,int32_t,int32_t,int32_t);
/* Genuine SCC return to the unique whole construction provider; the old
 * global unit01020 must never satisfy this dependency. */
extern void restore_placement_create_00470990(uint32_t,int32_t,int32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define A(n) U(sp+(n)*4u)

static void actor_query(uint32_t target,struct restore_building_refresh_frame *f)
{
 struct restore_actor_message_frame a;uint32_t i;
 for(i=0;i<8;++i)a.r[i]=f->r[i];a.flags=f->flags;a.boundary=restore_actor_message_dispatch;
 if(target==0x4893c0u)restore_actor_message_C_004893c0(&a);
 else restore_actor_message_C_004894a0(&a);
 for(i=0;i<8;++i)f->r[i]=a.r[i];f->flags=a.flags;
}
void restore_building_refresh_dispatch(uint32_t site,uint32_t target,struct restore_building_refresh_frame *f)
{
 uint32_t sp=f->r[4],cleanup=0;(void)site;
 switch(target){
 case 0x488690u:restore_building_refresh_C_00488690(f);return;
 case 0x488840u:restore_building_refresh_C_00488840(f);return;
 case 0x488c20u:restore_building_refresh_C_00488c20(f);return;
 case 0x488e60u:restore_building_refresh_C_00488e60(f);return;
 case 0x4894d0u:restore_building_refresh_C_004894d0(f);return;
 case 0x4df9d0u:restore_building_refresh_C_004df9d0(f);return;
 case 0x4dfbb0u:restore_building_refresh_C_004dfbb0(f);return;
 case 0x4e57f0u:restore_building_refresh_C_004e57f0(f);return;
 case 0x4e5840u:restore_building_refresh_C_004e5840(f);return;
 case 0x4edf50u:restore_building_refresh_C_004edf50(f);return;
 case 0x4ee4a0u:restore_building_refresh_C_004ee4a0(f);return;
 case 0x4ee6c0u:restore_building_refresh_C_004ee6c0(f);return;
 case 0x4ee780u:restore_building_refresh_C_004ee780(f);return;
 case 0x4f3c90u:restore_building_refresh_C_004f3c90(f);return;
 case 0x4f4160u:restore_building_refresh_C_004f4160(f);return;
 case 0x4f5f10u:restore_building_refresh_C_004f5f10(f);return;
 case 0x4893c0u:case 0x4894a0u:actor_query(target,f);return;
 case 0x488030u:f->r[0]=FUN_00488030(A(1));cleanup=4;break;
 case 0x4890f0u:f->r[0]=restore_building_active_004890f0((int32_t)A(1));cleanup=4;break;
 case 0x489050u:f->r[0]=(uint32_t)restore_building_area_00489050((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));cleanup=16;break;
 case 0x47bf50u:FUN_0047bf50((uint16_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),A(6),A(7));break;
 case 0x53e590u:f->r[0]=(uint32_t)restore_sprite_draw((RestoreSpriteLibraries *)(uintptr_t)f->r[1],(int32_t)A(1),(int32_t)A(2),(int32_t)A(3));cleanup=12;break;
 case 0x53dd90u:f->r[0]=(f->r[0]&0xffffff00u)|FUN_0053dd90((RestoreSpriteLibraries *)(uintptr_t)f->r[1],(const char *)(uintptr_t)A(1),(int32_t)A(2));cleanup=8;break;
 case 0x4f4f80u:f->r[0]=(uint32_t)FUN_004f4f80((int32_t)A(1));break;
 case 0x4f4aa0u:f->r[0]=(uint32_t)restore_building_slot_004f4aa0((int32_t)A(1));break;
 case 0x4df360u:f->r[0]=(uint32_t)restore_building_image_004df360((int32_t)A(1),(uint32_t *)(uintptr_t)A(2));break;
 case 0x4ded50u:f->r[0]=(uint32_t)(uintptr_t)restore_city_geometry_004ded50((int32_t *)(uintptr_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
 case 0x46a400u:f->r[0]=(uint32_t)restore_building_allocate_0046a400(A(1),A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6));break;
 case 0x485100u:f->r[0]=FUN_00485100((int32_t)A(1));break;
 case 0x475bf0u:FUN_00475bf0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
 case 0x4767d0u:FUN_004767d0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),A(5),A(6));break;
 case 0x477500u:FUN_00477500((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
 case 0x4e5b90u:FUN_004e5b90((int32_t)A(1));break;
 case 0x4ecea0u:f->r[0]=(f->r[0]&0xffffff00u)|FUN_004ecea0((int32_t)A(1));break;
 case 0x4ed360u:FUN_004ed360((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),A(4));break;
 case 0x4f3f70u:f->r[0]=FUN_004f3f70((int32_t)A(1),A(2),(int32_t)A(3),A(4));break;
 case 0x470990u:restore_placement_create_00470990(A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6),A(7),A(8));break;
 default:abort();
 }
 f->r[4]=sp+4u+cleanup;
}
#undef A
void restore_building_refresh_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
 struct restore_building_refresh_frame f;uint32_t i;
 for(i=0;i<8;++i)f.r[i]=registers[i];f.flags=*flags;f.boundary=restore_building_refresh_dispatch;
 restore_building_refresh_dispatch(0,target,&f);
 for(i=0;i<8;++i)registers[i]=f.r[i];*flags=f.flags;
}

/* 4E5840 has no incoming receiver/scratch input. Its cdecl result is void;
   the retained physical C provider owns widths, sprite receiver and children. */
void restore_obelisk_refresh_progress(int32_t building,int32_t x,int32_t y)
{
 uint32_t stack[8192],r[8]={0},flags=2,sp=(uint32_t)(uintptr_t)(stack+8000);
 stack[8000]=0;stack[8001]=(uint32_t)building;
 stack[8002]=(uint32_t)x;stack[8003]=(uint32_t)y;r[4]=sp;
 restore_building_refresh_raw(0x4e5840u,r,&flags);
 if(r[4]!=sp+4u)abort();
}
