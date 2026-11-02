#include "surface_passes.h"
#include "paving_neighbors.h"
#include "neighbors.h"
#include "road_place.h"
#include "water_refresh.h"
#include "map_neighbors_phase.h"
#include <stdlib.h>
extern int32_t FUN_00484be0(void *,int32_t,int32_t,uint8_t);
extern void FUN_0051f780(void);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x8e47b123)
static void boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4];(void)site;
 switch(target){
 case 0x475670u:f->r[0]=FUN_00475670((int32_t)U(sp+4u));break;
 case 0x47cae0u:f->r[0]=FUN_0047cae0((int32_t)U(sp+4u),(int32_t)U(sp+8u),(int32_t)U(sp+12u),(int32_t)U(sp+16u),U(sp+20u),U(sp+24u));break;
 case 0x47bf50u:FUN_0047bf50((uint16_t)U(sp+4u),(int32_t)U(sp+8u),(int32_t)U(sp+12u),(int32_t)U(sp+16u),(int32_t)U(sp+20u),U(sp+24u),U(sp+28u));break;
 case 0x47db00u:FUN_0047db00(U(sp+4u));break;
 case 0x484f00u:FUN_00484f00();break;
 case 0x484be0u:f->r[0]=(uint32_t)FUN_00484be0((void *)(uintptr_t)U(sp+4u),(int32_t)U(sp+8u),(int32_t)U(sp+12u),(uint8_t)U(sp+16u));break;
 case 0x47e000u:f->r[0]=FUN_0047e000();break;
 case 0x47e750u:FUN_0047e750();break;
 case 0x51f780u:FUN_0051f780();break;
 default:abort();
 }
 f->r[4]=sp+4u;
}
static void begin(struct restore_menu_input_frame *f,uint32_t stack[32])
{
 unsigned i;for(i=0;i<32;++i)stack[i]=SENTRY;for(i=0;i<8;++i)f->r[i]=0;
 f->flags=2;f->boundary=boundary;f->r[4]=(uint32_t)(uintptr_t)(stack+16);
}
static void end(const struct restore_menu_input_frame *f,const uint32_t stack[32],unsigned low,unsigned high)
{
 unsigned i;if(f->r[4]!=(uint32_t)(uintptr_t)(stack+17)||stack[16]!=SENTRY||f->r[3]||f->r[5]||f->r[6]||f->r[7])abort();
 for(i=0;i<low;++i)if(stack[i]!=SENTRY)abort();for(i=high;i<32;++i)if(stack[i]!=SENTRY)abort();
}
void FUN_00475720(int32_t left,int32_t top,int32_t right,int32_t bottom,uint32_t clear)
{
 uint32_t stack[32];struct restore_menu_input_frame f;begin(&f,stack);
 stack[17]=(uint32_t)left;stack[18]=(uint32_t)top;stack[19]=(uint32_t)right;stack[20]=(uint32_t)bottom;stack[21]=clear;
 restore_C_00475720(&f);end(&f,stack,3,22);
}
void FUN_00476530(void)
{
 uint32_t stack[32];struct restore_menu_input_frame f;begin(&f,stack);
 restore_C_00476530(&f);end(&f,stack,3,17);
}
void FUN_0047b1a0(void)
{
 uint32_t stack[32];struct restore_menu_input_frame f;begin(&f,stack);
 restore_C_0047b1a0(&f);end(&f,stack,8,17);
}
