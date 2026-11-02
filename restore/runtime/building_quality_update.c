#include "building_quality_update.h"
#include "menu_full_input_numeric.h"
#include "building_nearest_search.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
uint64_t restore_building_sqrt_trunc64(int32_t squared,uint8_t raw80[10])
{
 __asm { fild squared }
 __asm { fsqrt }
 __asm { mov eax,raw80 }
 __asm { fstp tbyte ptr [eax] }
 return restore_menu_input_trunc80(raw80);
}
static uint32_t distance(uint32_t record,int32_t x,int32_t y)
{
 uint32_t dx=(uint32_t)(int32_t)H(record+6u)-(uint32_t)x,dy=(uint32_t)(int32_t)H(record+8u)-(uint32_t)y;
 uint8_t raw[10];
 return (uint32_t)restore_building_sqrt_trunc64((int32_t)(dx*dx+dy*dy),raw);
}
void restore_building_quality_004e7f00(int32_t building,uint32_t recalculate)
{
 uint32_t history=U(0xd3b1c4u),left,right,i,player,record=0x7999d8u+(uint32_t)building*264u,variant,value,water,road,bonus=16,divisor=1;
 int32_t radius,found_x,found_y;uint8_t difficulty;
 U(0xd3b1c8u+history*4u)=U(0xd3b374u);++history;U(0xd3b1c4u)=history;if((int32_t)history>=100)U(0xd3b1c4u)=0;
 left=U(0xd3b384u);right=U(0xd3b380u);
 for(i=0;i<31;++i){left=(left>>1)|((((left>>4)^left)&1u)<<30);right=(right>>1)|((((right>>4)^right)&1u)<<30);}
 U(0xd3b384u)=left;U(0xd3b380u)=right;U(0xd3b370u)=left&0x7fffu;U(0xd3b374u)=left&0x7fu;U(0xd3b378u)=left&7u;U(0xd3b36cu)=right&7u;U(0xd3b364u)=right&0x7fffu;
 player=U(0xeaac28u);U(0xd3b368u)=right&0x7fu;
 variant=(uint32_t)(int32_t)H(record+18u);
 value=U(record+252u)+U(0x70ce20u+variant*88u)+(50u-U(0xea1888u+player*18904u))*2u;
 U(record+252u)=value;if((int32_t)value<0)U(record+252u)=0;if((int32_t)U(record+252u)>1000)U(record+252u)=1000;
 if((uint8_t)recalculate){
  radius=(int32_t)U(0xbca2d0u);if((int32_t)U(0xbca2d4u)>radius)radius=(int32_t)U(0xbca2d4u);
  if(restore_building_nearest_search(H(record+6u),H(record+8u),0,0,0x40000u,&found_x,&found_y,radius,0))water=distance(record,found_x,found_y);else water=(uint32_t)radius;
  if(restore_building_nearest_search(H(record+6u),H(record+8u),0,0,4u,&found_x,&found_y,radius,0))road=distance(record,found_x,found_y);else road=(uint32_t)radius;
  player=U(0xeaac28u);U(record+244u)=road;U(record+248u)=water;
 }else{water=U(record+248u);road=U(record+244u);}
 difficulty=B(0x784cb0u);
 if(difficulty==0)road=1;else if(difficulty==2){bonus=8;divisor=2;}
 variant=(uint32_t)(int32_t)H(record+18u);
 value=U(0x70ce1cu+variant*88u)-U(0xea1888u+player*18904u)+(55u-water)*2u-road+bonus;
 value=U(record+232u)+(uint32_t)((int32_t)value/(int32_t)divisor);
 U(record+232u)=value;if((int32_t)value<0)U(record+232u)=0;if((int32_t)U(record+232u)>1000)U(record+232u)=1000;
}
