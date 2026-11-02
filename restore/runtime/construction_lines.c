#include "construction_lines.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_00485100(int32_t building);
static uint32_t direction(uint32_t cell)
{
 static const uint32_t delta[8]={0xffffff1cu,0xfffffe38u,228,456,1,2,0xffffffffu,0xfffffffeu};
 uint32_t vertical=0,horizontal=0,i,at;
 for(i=0;i<8;++i){at=cell+delta[i];if((B(0xaf598cu+at*4u)&4u)&&B(0xadc36cu+at)){if(i<4)++vertical;else ++horizontal;}}
 return vertical>=horizontal?228u:1u;
}
static uint32_t beginning(uint32_t cell,uint32_t step)
{
 while(B(0xaf598cu+(cell-step)*4u)&4u){if(!B(0xadc36cu+cell-step))break;cell-=step;}
 return cell;
}
void FUN_0047f810(uint32_t cell,uint32_t mark)
{
 uint32_t step,flags,next;
 if(!(B(0xaf598cu+cell*4u)&4u))return;if(!B(0xadc36cu+cell))return;
 step=direction(cell);cell=beginning(cell,step);
 if(mark)B(0xb285ccu+cell)|=0x40u;
 else{flags=U(0xaf598cu+cell*4u);B(0xadc36cu+cell)=0;U(0xaf598cu+cell*4u)=flags&0xffffffbfu;}
 next=cell+step;
 while(B(0xaf598cu+next*4u)&4u){
  if(!B(0xadc36cu+next))break;cell=next;
  if(mark)B(0xb285ccu+cell)|=0x40u;
  else{U(0xaf598cu+cell*4u)&=0xffffffbfu;B(0xadc36cu+cell)=0;}
  next=cell+step;
 }
}
uint32_t FUN_0047f9b0(uint32_t cell)
{
 uint32_t step,count,next;
 if(!(B(0xaf598cu+cell*4u)&4u))return 0;if(!B(0xadc36cu+cell))return 0;
 step=direction(cell);cell=beginning(cell,step);count=W(0xb350dcu+cell*2u)!=0;
 B(0xb285ccu+cell)&=0xbfu;next=cell+step;
 while(B(0xaf598cu+next*4u)&4u){
  if(!B(0xadc36cu+next))break;cell=next;
  if(W(0xb350dcu+cell*2u))++count;
  B(0xb285ccu+cell)&=0xbfu;next=cell+step;
 }
 return count;
}
void FUN_004f5e70(int32_t building,uint8_t reserve)
{
 uint32_t index=(uint32_t)building,record,group,offset,scene,cell,child,count;
 index-=(uint32_t)(int32_t)(int16_t)W(0x7999eau+index*264u);
 scene=U(0x5e4ef4u);record=index*264u;group=B(0x799a2cu+record);
 offset=U((scene==5u?0x5700e8u:0x5700d4u)+group*4u);cell=U(0x7999e4u+record)+offset;
 child=(uint32_t)(int32_t)(int16_t)W(0xb4e6fcu+cell*2u);if(!child)return;
 record=child*264u;if(W(0x7999e8u+record)!=215u)return;
 if(reserve){(void)FUN_00485100((int32_t)child);count=U(0xbc971cu)+1u;B(0x7999d8u+record)=6;U(0xbc971cu)=count;}
 else B(0x7999d8u+record)=2;
}
