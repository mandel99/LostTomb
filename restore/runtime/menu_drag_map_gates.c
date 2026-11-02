#include "map_gates.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x8e47b123)
static void begin(struct restore_menu_input_frame *f,uint32_t stack[32])
{
 unsigned i;for(i=0;i<32;++i)stack[i]=SENTRY;for(i=0;i<8;++i)f->r[i]=0;
 f->flags=2;f->boundary=0;f->r[4]=(uint32_t)(uintptr_t)(stack+16);
}
static void end(const struct restore_menu_input_frame *f,const uint32_t stack[32],unsigned low,unsigned high)
{
 unsigned i;if(f->r[4]!=(uint32_t)(uintptr_t)(stack+17)||stack[16]!=SENTRY||f->r[3]||f->r[5]||f->r[6]||f->r[7])abort();
 for(i=0;i<low;++i)if(stack[i]!=SENTRY)abort();for(i=high;i<32;++i)if(stack[i]!=SENTRY)abort();
}
uint32_t FUN_00482810(int32_t x,int32_t y,int32_t size,int32_t radius,int32_t ignored_cell)
{
 uint32_t stack[32];struct restore_menu_input_frame f;begin(&f,stack);
 stack[17]=(uint32_t)x;stack[18]=(uint32_t)y;stack[19]=(uint32_t)size;stack[20]=(uint32_t)radius;stack[21]=(uint32_t)ignored_cell;
 restore_C_00482810(&f);end(&f,stack,12,22);return f.r[0];
}
static void boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4];(void)site;if(target!=0x482810u)abort();
 f->r[0]=FUN_00482810((int32_t)U(sp+4u),(int32_t)U(sp+8u),(int32_t)U(sp+12u),(int32_t)U(sp+16u),(int32_t)U(sp+20u));f->r[4]=sp+4u;
}
void FUN_0047d870(uint32_t clear)
{
 uint32_t stack[32];struct restore_menu_input_frame f;begin(&f,stack);f.boundary=boundary;stack[17]=clear;
 restore_C_0047d870(&f);end(&f,stack,6,18);
}
