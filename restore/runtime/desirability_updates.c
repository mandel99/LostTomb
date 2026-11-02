#include "map_refresh.h"
#include <string.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

static void clamp_cell(uint32_t cell)
{
 int32_t value=(int8_t)B(0xac2d4cu+cell);
 if(value < -100)B(0xac2d4cu+cell)=156;
 if((int8_t)B(0xac2d4cu+cell)>100)B(0xac2d4cu+cell)=100;
}

/* 4818A0: ring is an integer, not a pointer. Counts, offsets and deltas
 * are signed WORDs; the field itself wraps as a BYTE before native clamping. */
void FUN_004818a0(int32_t x,int32_t y,int32_t size,int32_t ring,int8_t amount)
{
 static const uint32_t count_tables[]={0x5c9afeu,0x5c9b16u,0x5c9b22u,0x5c9b2eu,0x5c9b3au,0x5c9b46u};
 static const uint32_t offset_tables[]={0x5c9b52u,0x5c9b6au,0x5c9b76u,0x5c9b82u,0x5c9b8eu,0x5c9b9au};
 static const uint32_t delta_tables[]={0x70c8ecu,0x70c76cu,0x70c5bcu,0x70c3dcu,0x70c1ccu,0x70bf8cu};
 uint32_t family=(uint32_t)size-1u,index=(uint32_t)ring*2u,origin,pointer,cell;
 int32_t count,point,clipped;
 clipped=(int32_t)((uint32_t)x-(uint32_t)ring)<-1||
         (int32_t)((uint32_t)y-(uint32_t)ring)<-1||
         (int32_t)((uint32_t)x+(uint32_t)size+(uint32_t)ring-1u)>(int32_t)U(0xbca2d0u)||
         (int32_t)((uint32_t)y+(uint32_t)size+(uint32_t)ring-1u)>(int32_t)U(0xbca2d4u);
 if(family>5u)return;
 count=H(count_tables[family]+index);
 pointer=delta_tables[family]+(uint32_t)(int32_t)H(offset_tables[family]+index)*2u;
 origin=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
 U(0xbc9768u)=origin;
 for(point=0;point<count;++point,pointer+=2u){
  if(clipped&&FUN_00481aa0(x,y,size,ring,point))continue;
  if(clipped)origin=U(0xbc9768u);
  cell=origin+(uint32_t)(int32_t)H(pointer);
  B(0xac2d4cu+cell)=(uint8_t)(B(0xac2d4cu+cell)+(uint8_t)amount);
  /* The clipped native path clamps the origin; the interior path clamps
   * the written cell. Preserve this observable distinction. */
  clamp_cell(clipped?origin:cell);
 }
}

/* 481830: at most six rings, with DWORD cadence and wrapping adjustment. */
void FUN_00481830(int32_t x,int32_t y,int32_t size,int32_t amount,int32_t interval,int32_t step,int32_t range)
{
 uint32_t cadence=0,value=(uint32_t)amount;int32_t ring;
 if(size<=0)return;
 if(range>6)range=6;
 for(ring=1;ring<=range;++ring){
  FUN_004818a0(x,y,size,ring,(int8_t)(uint8_t)value);
  ++cadence;
  if((int32_t)cadence>=interval){value+=(uint32_t)step;cadence=0;}
 }
}

/* 46A0C0: live ordered building scan and40-byte model records. */
void FUN_0046a0c0(void)
{
 uint32_t building=1,record,model;
 memset((void *)(uintptr_t)0xac2d4cu,0,0xcb10u);
 while((int32_t)building<=(int32_t)U(0x78d790u)){
  record=0x7999d8u+building*264u;
  if(B(record)==1){
   model=(uint32_t)(int32_t)H(record+16u)*40u;
   FUN_00481830(H(record+6u),H(record+8u),B(record+3u),
    (int32_t)U(0x705ea8u+model),(int32_t)U(0x705eacu+model),
    (int32_t)U(0x705eb0u+model),(int32_t)U(0x705eb4u+model));
  }
  ++building;
 }
}

/* 46A140 follows the building pass in the same simulation phase. */
void FUN_0046a140(void)
{
 uint32_t cell=U(0xbca2d8u),x,y=0,flags,model;uint8_t marker;
 while((int32_t)y<(int32_t)U(0xbca2d4u)){
  x=0;
  while((int32_t)x<(int32_t)U(0xbca2d0u)){
   marker=B(0xb285ccu+cell);flags=U(0xaf598cu+cell*4u);
   if(marker&128u){
    if(flags&64u)model=38u*40u;
    else if(flags&2u)model=10u*40u;
    else{B(0xb285ccu+cell)=marker&127u;goto next;}
    FUN_00481830((int32_t)x,(int32_t)y,1,(int32_t)U(0x705ea8u+model),
     (int32_t)U(0x705eacu+model),(int32_t)U(0x705eb0u+model),(int32_t)U(0x705eb4u+model));
   }else if(flags&32u){
    FUN_00481830((int32_t)x,(int32_t)y,1,(int32_t)U(0x7064c0u),
     (int32_t)U(0x7064c4u),(int32_t)U(0x7064c8u),(int32_t)U(0x7064ccu));
   }else if(flags&0x1000u)FUN_00481830((int32_t)x,(int32_t)y,1,-2,1,1,2);
next:
   ++x;++cell;
  }
  ++y;cell+=U(0xbca2dcu);
 }
}
