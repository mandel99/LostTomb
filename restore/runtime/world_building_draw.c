#include "world_building_draw.h"
#include "building_refresh_helpers.h"
#include <stdlib.h>
void restore_building_draw_00488c20(uint32_t self,uint32_t saved_ebx,int32_t building,int32_t x,int32_t y,uint32_t mode)
{
 uint32_t stack[256],registers[8]={0},flags=2,sp,i;
 for(i=0;i<256;++i)stack[i]=0x488c20a1u;
 sp=(uint32_t)(uintptr_t)(stack+192);
 stack[193]=(uint32_t)building;stack[194]=(uint32_t)x;stack[195]=(uint32_t)y;stack[196]=mode;
 registers[1]=self;registers[3]=saved_ebx;registers[4]=sp;
 restore_building_refresh_raw(0x488c20u,registers,&flags);
 if(registers[4]!=sp+20u||registers[3]!=saved_ebx||registers[5]||registers[6]||registers[7])abort();
 for(i=0;i<32;++i)if(stack[i]!=0x488c20a1u)abort();
 for(i=197;i<256;++i)if(stack[i]!=0x488c20a1u)abort();
}
void restore_world_building_draw_00488c20(uint32_t self,int32_t building,int32_t x,int32_t y,uint32_t mode)
{
 restore_building_draw_00488c20(self,(uint32_t)building,building,x,y,mode);
}
