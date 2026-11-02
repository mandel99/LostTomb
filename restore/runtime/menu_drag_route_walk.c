#include "route_walk_model.h"
#include "road_paint_model.h"
#include "route_seed.h"
#include "actor_walking.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x8e47b123)
static void boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4],a=U(sp+4u);(void)site;
 switch(target){
 case 0x51c390u:f->r[0]=FUN_0051c390(a);break;
 case 0x51c630u:f->r[0]=FUN_0051c630(a);break;
 case 0x47b310u:FUN_0047b310((int32_t)a,(int32_t)U(sp+8u),(int32_t)U(sp+12u));break;
 case 0x47b960u:FUN_0047b960((int32_t)a,(int32_t)U(sp+8u),(int32_t)U(sp+12u),(int32_t)U(sp+16u));break;
 case 0x51cba0u:f->r[0]=FUN_0051cba0((int32_t)a,(int32_t)U(sp+8u),(int32_t)U(sp+12u),(int32_t)U(sp+16u));break;
 default:abort();
 }
 f->r[4]=sp+4u;
}
uint32_t FUN_0051c930(uint32_t unused,int32_t target_x,int32_t target_y,int32_t x,int32_t y,int32_t mode)
{
 uint32_t stack[32];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<32;++i)stack[i]=SENTRY;for(i=0;i<8;++i)f.r[i]=0;
 stack[17]=unused;stack[18]=(uint32_t)target_x;stack[19]=(uint32_t)target_y;stack[20]=(uint32_t)x;stack[21]=(uint32_t)y;stack[22]=(uint32_t)mode;
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+16);restore_C_0051c930(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+17)||stack[16]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<5;++i)if(stack[i]!=SENTRY)abort();for(i=23;i<32;++i)if(stack[i]!=SENTRY)abort();
 return f.r[0];
}
