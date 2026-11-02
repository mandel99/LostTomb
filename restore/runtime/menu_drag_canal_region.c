#include "canal_region_model.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0047e870(void);
extern int32_t FUN_00484be0(void *,int32_t,int32_t,uint8_t);
extern uint32_t FUN_0047b920(void);
extern int32_t FUN_0051c620(int32_t);
#define SENTRY UINT32_C(0x8e47b123)
static void boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4];(void)site;
 switch(target){
 case 0x47e870u:FUN_0047e870();break;
 case 0x484be0u:f->r[0]=(uint32_t)FUN_00484be0((void *)(uintptr_t)U(sp+4u),(int32_t)U(sp+8u),(int32_t)U(sp+12u),(uint8_t)U(sp+16u));break;
 case 0x47b920u:f->r[0]=FUN_0047b920();break;
 case 0x51c620u:f->r[0]=(uint32_t)FUN_0051c620((int32_t)U(sp+4u));break;
 default:abort();
 }
 f->r[4]=sp+4u;
}
void FUN_0047bae0(int32_t left,int32_t top,int32_t right,int32_t bottom,uint32_t refresh)
{
 uint32_t stack[32];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<32;++i)stack[i]=SENTRY;
 for(i=0;i<8;++i)f.r[i]=0;
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+16);
 stack[17]=(uint32_t)left;stack[18]=(uint32_t)top;stack[19]=(uint32_t)right;stack[20]=(uint32_t)bottom;stack[21]=refresh;
 restore_C_0047bae0(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+17)||stack[16]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<7;++i)if(stack[i]!=SENTRY)abort();
 for(i=22;i<32;++i)if(stack[i]!=SENTRY)abort();
}
