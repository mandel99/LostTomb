#include "widget_position.h"
#include <string.h>
#include <stdlib.h>
typedef char widget_pointer32[sizeof(void*)==4?1:-1];

void restore_widget_position_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
 struct restore_widget_position_frame f;
 memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=0;
 switch(target){
 case 0x41f910u:restore_widget_C_0041f910(&f);break;
 case 0x41fad0u:restore_widget_C_0041fad0(&f);break;
 default:abort();
 }
 memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}
static void position(uint32_t target,uint32_t self,int32_t *x,int32_t *y,uint32_t *image,int32_t direction)
{
 uint32_t stack[64],i,entry,regs[8]={0},flags=0,cleanup=target==0x41f910u?16u:20u;
 /* Native own minima are -24/-20, no callees. 32 words below entry.
    Output pointers remain caller supplied, including intentional aliases. */
 for(i=0;i<64;++i)stack[i]=0xa5a55a5au;
 entry=(uint32_t)(uintptr_t)(stack+32);
 stack[32]=0xdead0041u;stack[33]=(uint32_t)(uintptr_t)x;
 stack[34]=(uint32_t)(uintptr_t)y;stack[35]=(uint32_t)(uintptr_t)image;
 stack[36]=(uint32_t)direction;
 regs[1]=self;regs[4]=entry;
 restore_widget_position_raw(target,regs,&flags);
 if(regs[4]!=entry+cleanup||stack[32]!=0xdead0041u)abort();
 for(i=0;i<8;++i)if(stack[i]!=0xa5a55a5au)abort();
 for(i=37;i<64;++i)if(stack[i]!=0xa5a55a5au)abort();
}
void restore_widget_position_0041f910(uint32_t self,int32_t *x,int32_t *y,uint32_t *image)
{position(0x41f910u,self,x,y,image,0);}
void restore_widget_position_0041fad0(uint32_t self,int32_t *x,int32_t *y,uint32_t *image,int32_t direction)
{position(0x41fad0u,self,x,y,image,direction);}
