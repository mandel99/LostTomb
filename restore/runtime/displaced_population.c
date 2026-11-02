#include "displaced_population.h"
#include "actor_create.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
void FUN_004e9350(int32_t player,int32_t people)
{
 uint32_t cursor=U(0xd3b1c4u),left=(uint32_t)people,misses=0,base=(uint32_t)player*0x49d8u,index,at,value;
 if(left){
  do{
   index=U(0xd3b1c8u+cursor*4u)&63u;at=0xea3e34u+base+index*2u;value=W(at);
   if((int16_t)value>0){--value;--left;W(at)=(uint16_t)value;misses=0;}else ++misses;
   ++cursor;if((int32_t)cursor>=100)cursor=0;
   if(misses>=100)break;
  }while(left);
 }
 misses=0;cursor=10;
 if(left)do{
  at=0xea3e34u+base+cursor*2u;value=W(at);
  if((int16_t)value>0){--value;--left;W(at)=(uint16_t)value;misses=0;}else ++misses;
  ++cursor;if((int32_t)cursor>=100)cursor=0;
  if(misses>=100)break;
 }while(left);
}
void FUN_004e9250(int32_t player,int32_t people)
{
 uint32_t base=(uint32_t)player*0x49d8u,total,high,value,i;
 value=U(0xea3f68u+base);U(0xea3f68u+base)=value+(uint32_t)people;
 FUN_004e9350(player,people);U(0xea1894u+base)=0;
 total=0;
 for(i=0;i<100;++i){
  value=(uint32_t)(int32_t)(int16_t)W(0xea3e34u+base+i*2u);total=U(0xea1894u+base);total+=value;U(0xea1894u+base)=total;
 }
 high=U(0xea5f0cu+base);if((int32_t)total>(int32_t)high)U(0xea5f0cu+base)=total;
}
void FUN_00459020(int32_t player,int32_t x,int32_t y,int32_t people)
{
 uint32_t actor=(uint32_t)FUN_004b4800((uint8_t)player,3,x,y,0),record=0xbcd5f8u+actor*388u;
 B(record+56u)=8;W(record+54u)=0;B(record+100u)=(uint8_t)people;
 FUN_004e9250(player,people);
}
