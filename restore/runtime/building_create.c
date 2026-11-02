#include "building_create_model.h"
#include "building_support.h"
#include "building_widgets.h"
#include "building_methods.h"
#include "building_refresh_helpers.h"
#include "building_allocator.h"
#include "construction_tiles.h"
#include "bridge_place.h"
#include "construction_refresh.h"
#include "road_place.h"
#include "footprint.h"
#include "ground_render.h"
#include "editor_brush_validation.h"
#include "actor_predicate.h"
#include "sprite_libraries.h"
#include "resource_availability.h"
#include "building_footprint.h"
#include "building_controllers.h"
#include "placement_queries.h"
#include "placement_palette.h"
#include "road_variant.h"
#include "redraw_dispatch_model.h"
#include <stdio.h>
#include <stdlib.h>
extern uint32_t FUN_00451bc0(int32_t);
extern uint32_t FUN_0045b490(int32_t);
extern int32_t FUN_00475b50(int32_t);
extern uint32_t FUN_0047d350(void);
extern void FUN_0053d850(void);
extern uint32_t FUN_00562348(void);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x8e47b123)

void restore_building_create_dispatch(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4],a,b,c,d,cleanup=0;
 if(restore_building_method_has(target)){restore_building_method_dispatch(site,target,f);return;}
 switch(target){
 case 0x488e60u:case 0x488c20u:case 0x4df9d0u:case 0x4dfbb0u:
 case 0x4e57f0u:case 0x4e5840u:case 0x4ee4a0u:case 0x4ee780u:
  restore_building_refresh_raw(target,f->r,&f->flags);return;
 case 0x401800u:case 0x40de80u:case 0x417310u:case 0x545760u:
 case 0x558c00u:case 0x555ea0u:case 0x54f490u:case 0x4e6a70u:
 case 0x401af0u:case 0x40eb90u:case 0x417490u:case 0x545a30u:
 case 0x558f00u:case 0x556230u:case 0x54f8a0u:case 0x4e6ae0u:
 case 0x547ba0u:
  restore_placement_queries_dispatch(site,target,f);return;
 }
 switch(target){
 case 0x405ba0u:f->r[0]=FUN_00405ba0();goto done;
 case 0x47d350u:f->r[0]=FUN_0047d350();goto done;
 case 0x4f6520u:f->r[0]=restore_ring_004f6520((void *)(uintptr_t)f->r[1]);goto done;
 case 0x53d850u:FUN_0053d850();goto done;
 case 0x5596d0u:FUN_005596d0();goto done;
 case 0x562348u:f->r[0]=FUN_00562348();goto done;
 case 0x41ff90u:FUN_0041ff90();goto done;
 case 0x41ffc0u:FUN_0041ffc0();goto done;
 case 0x4206a0u:FUN_004206a0();goto done;
 case 0x4209e0u:FUN_004209e0();goto done;
 case 0x420d10u:FUN_00420d10();goto done;
 case 0x421010u:FUN_00421010();goto done;
 case 0x4212f0u:FUN_004212f0();goto done;
 case 0x4dea50u:FUN_004dea50();goto done;
 }
 a=U(sp+4);
 switch(target){
 case 0x451bc0u:f->r[0]=FUN_00451bc0((int32_t)a);goto done;
 case 0x45b490u:f->r[0]=FUN_0045b490((int32_t)a);goto done;
 case 0x46cf40u:FUN_0046cf40((int32_t)a);goto done;
 case 0x46efd0u:f->r[0]=(uint32_t)FUN_0046efd0(a);goto done;
 case 0x475b50u:f->r[0]=(uint32_t)FUN_00475b50((int32_t)a);goto done;
 case 0x47a1c0u:f->r[0]=FUN_0047a1c0((int32_t)a);goto done;
 case 0x485100u:f->r[0]=FUN_00485100((int32_t)a);goto done;
 case 0x4c5120u:f->r[0]=FUN_004c5120((int32_t)a);goto done;
 case 0x4f4f80u:f->r[0]=(uint32_t)FUN_004f4f80((int32_t)a);goto done;
 case 0x487ec0u:FUN_00487ec0((int32_t)a);goto done;
 case 0x4e5b90u:FUN_004e5b90((int32_t)a);goto done;
 case 0x41fca0u:f->r[0]=restore_widget_initialize_0041fca0((void *)(uintptr_t)f->r[1],(uint16_t)a);cleanup=4;goto done;
 case 0x46b100u:f->r[0]=(uint32_t)FUN_0046b100((int32_t)a);goto done;
 case 0x4b6e10u:f->r[0]=(uint32_t)FUN_004b6e10((int32_t)a);goto done;
 case 0x4bfb30u:FUN_004bfb30((int32_t)a);goto done;
 case 0x475fd0u:f->r[0]=(uint32_t)FUN_00475fd0(a);goto done;
 }
 b=U(sp+8);
 if(target==0x46cea0u){FUN_0046cea0((int32_t)a,(int16_t)b);goto done;}
 if(target==0x53dd90u){f->r[0]=(f->r[0]&0xffffff00u)|FUN_0053dd90((RestoreSpriteLibraries *)(uintptr_t)f->r[1],(const char *)(uintptr_t)a,(int32_t)b);cleanup=8;goto done;}
 if(target==0x4c0d50u){FUN_004c0d50((int32_t)a,(int32_t)b);goto done;}
 if(target==0x5414b0u){f->r[0]=restore_placement_image_005414b0((int32_t)a,b);goto done;}
 c=U(sp+12);
 switch(target){
 case 0x440060u:FUN_00440060(a,(int32_t)b,(uint16_t)c);goto done;
 case 0x440540u:f->r[1]=restore_bridge_00440540((uint32_t *)(uintptr_t)(sp+4u),f->r[1]);goto done;
 case 0x47afd0u:FUN_0047afd0((int32_t)a,(int32_t)b,(int32_t)c);goto done;
 case 0x47d060u:f->r[0]=(uint32_t)FUN_0047d060((int32_t)a,(int32_t)b,c);goto done;
 }
 d=U(sp+16);
 if(target==0x47cc70u){f->r[0]=(uint32_t)FUN_0047cc70((int32_t)a,(int32_t)b,c,(uint8_t)d);goto done;}
 if(target==0x472790u){FUN_00472790((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d);goto done;}
 if(target==0x47cf00u){FUN_0047cf00(a,(int32_t)b,(int32_t)c,d);goto done;}
 if(target==0x47d1f0u){FUN_0047d1f0(a,(int32_t)b,(int32_t)c,d);goto done;}
 if(target==0x47c2b0u){FUN_0047c2b0((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d,(int32_t)U(sp+20));goto done;}
 if(target==0x482580u){FUN_00482580((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d,U(sp+20));goto done;}
 if(target==0x46a400u){f->r[0]=(uint32_t)restore_building_allocate_0046a400(a,b,(int32_t)c,(int32_t)d,(int32_t)U(sp+20),(int32_t)U(sp+24));goto done;}
 if(target==0x47bf50u){FUN_0047bf50((uint16_t)a,(int32_t)b,(int32_t)c,(int32_t)d,(int32_t)U(sp+20),U(sp+24),U(sp+28));goto done;}
 if(target==0x4c3420u){FUN_004c3420((int32_t)a,(int32_t)b,(int32_t)c,d,(int32_t)U(sp+20),U(sp+24));goto done;}
 fprintf(stderr,"Unsupported470990 virtual/call target %08lx at %08lx.\n",(unsigned long)target,(unsigned long)site);abort();
done:
 f->r[4]=sp+4u+cleanup;
}

void restore_placement_create_00470990(uint32_t owner,int32_t kind,int32_t building,int32_t x,int32_t y,int32_t size,uint32_t initialize,uint32_t mode)
{
 uint32_t stack[256];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<256;++i)stack[i]=SENTRY;for(i=0;i<8;++i)f.r[i]=0;
 stack[129]=owner;stack[130]=(uint32_t)kind;stack[131]=(uint32_t)building;stack[132]=(uint32_t)x;
 stack[133]=(uint32_t)y;stack[134]=(uint32_t)size;stack[135]=initialize;stack[136]=mode;
 f.flags=2;f.boundary=restore_building_create_dispatch;f.r[4]=(uint32_t)(uintptr_t)(stack+128);restore_C_00470990(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+129)||stack[128]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 /* Whole50-function raw-frame CFG: conservative combined floor SP-420;
    16 low sentry DWORDs leave448 bytes, including every native CALL slot.
    Recursive F5F10 returns through this typed facade with a fresh frame. */
 for(i=0;i<16;++i)if(stack[i]!=SENTRY)abort();for(i=137;i<256;++i)if(stack[i]!=SENTRY)abort();
}
void FUN_00470990(uint32_t owner,int32_t kind,int32_t building,int32_t x,int32_t y,int32_t size,uint32_t initialize,uint32_t mode)
{restore_placement_create_00470990(owner,kind,building,x,y,size,initialize,mode);}
