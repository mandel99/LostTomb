#include "road_paint_model.h"
#include "city_prepare.h"
#include "terrain_redraw.h"
#include "canal_refresh.h"
#include "map_refresh_postpass4.h"
#include "map_refresh_postpass5.h"
#include "clear_helpers.h"
#include <stdlib.h>
extern void FUN_004746d0(int32_t,int32_t,int32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x8e47b123)
static void boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4];(void)site;
 switch(target){
 case 0x46f930u:f->r[0]=FUN_0046f930((int32_t)U(sp+4u),(int32_t)U(sp+8u));break;
 case 0x47db00u:FUN_0047db00(U(sp+4u));break;
 case 0x47be30u:FUN_0047be30((int32_t)U(sp+4u));break;
 case 0x47e5c0u:FUN_0047e5c0();break;
 case 0x47b670u:FUN_0047b670();break;
 case 0x47e870u:FUN_0047e870();break;
 case 0x4746d0u:FUN_004746d0((int32_t)U(sp+4u),(int32_t)U(sp+8u),(int32_t)U(sp+12u));break;
 default:abort();
 }
 f->r[4]=sp+4u;
}
static void execute(void (*body)(struct restore_menu_input_frame *),int32_t x,int32_t y,int32_t a,int32_t b,unsigned count)
{
 uint32_t stack[32];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<32;++i)stack[i]=SENTRY;for(i=0;i<8;++i)f.r[i]=0;
 stack[17]=(uint32_t)x;stack[18]=(uint32_t)y;stack[19]=(uint32_t)a;if(count==4)stack[20]=(uint32_t)b;
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+16);body(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+17)||stack[16]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<8;++i)if(stack[i]!=SENTRY)abort();for(i=17+count;i<32;++i)if(stack[i]!=SENTRY)abort();
}
void FUN_0047b310(int32_t x,int32_t y,int32_t set)
{execute(restore_C_0047b310,x,y,set,0,3);}
void FUN_0047b960(int32_t x,int32_t y,int32_t alternate,int32_t set)
{execute(restore_C_0047b960,x,y,alternate,set,4);}
