#include "small_constructor_model.h"
#include "road_check_model.h"
#include "road_scan_model.h"
#include "world_offset_model.h"
#include "construction_tiles.h"
#include "building_allocator.h"
#include "road_place.h"
#include "map_commit_state.h"
#include "editor_brush_validation.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x8e47b123)
static void boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4],a,b,c;(void)site;
 if(target==0x485830u){FUN_00485830();goto done;}
 a=U(sp+4);
 if(target==0x43f410u){f->r[0]=FUN_0043f410((int32_t)a);goto done;}
 if(target==0x485100u){f->r[0]=FUN_00485100((int32_t)a);goto done;}
 b=U(sp+8);c=U(sp+12);
 switch(target){
 case 0x4413b0u:FUN_004413b0((int32_t)a,(int32_t)b,(int32_t)c);break;
 case 0x441130u:f->r[0]=FUN_00441130((int32_t)a,(int32_t)b,(int32_t *)(uintptr_t)c,(int32_t *)(uintptr_t)U(sp+16),(int32_t)U(sp+20),(int32_t)U(sp+24));break;
 case 0x40f860u:FUN_0040f860((void *)(uintptr_t)a,(void *)(uintptr_t)b,(int32_t)c);break;
 case 0x4405c0u:FUN_004405c0((int32_t)a,(int32_t *)(uintptr_t)b,(int32_t *)(uintptr_t)c);break;
 case 0x46a400u:f->r[0]=(uint32_t)restore_building_allocate_0046a400(a,b,(int32_t)c,(int32_t)U(sp+16),(int32_t)U(sp+20),(int32_t)U(sp+24));break;
 case 0x47bf50u:FUN_0047bf50((uint16_t)a,(int32_t)b,(int32_t)c,(int32_t)U(sp+16),(int32_t)U(sp+20),U(sp+24),U(sp+28));break;
 case 0x440060u:FUN_00440060(a,(int32_t)b,(uint16_t)c);break;
 default:abort();
 }
 done:f->r[4]=sp+4u;
}
static uint32_t execute(void (*body)(struct restore_menu_input_frame *),uint32_t preview,uint32_t ignored_a,uint32_t ignored_b,int32_t x,int32_t y)
{
 uint32_t stack[128];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<128;++i)stack[i]=SENTRY;for(i=0;i<8;++i)f.r[i]=0;
 stack[65]=preview;stack[66]=ignored_a;stack[67]=ignored_b;stack[68]=(uint32_t)x;stack[69]=(uint32_t)y;
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+64);body(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+65)||stack[64]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<27;++i)if(stack[i]!=SENTRY)abort();for(i=70;i<128;++i)if(stack[i]!=SENTRY)abort();
 return f.r[0];
}
#define ENTRY(name,body) uint32_t name(uint32_t preview,uint32_t ignored_a,uint32_t ignored_b,int32_t x,int32_t y){return execute(body,preview,ignored_a,ignored_b,x,y);}
ENTRY(FUN_00440700,restore_C_00440700)
ENTRY(FUN_00440850,restore_C_00440850)
ENTRY(FUN_00440b10,restore_C_00440b10)
ENTRY(FUN_00440ed0,restore_C_00440ed0)
