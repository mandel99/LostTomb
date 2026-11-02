#include "building_allocator.h"
#include "actor_teardown_model.h"
#include "pending_message.h"
#include "building_reservation.h"
#include "building_neighbor_query.h"
#include "building_quality_update.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
void restore_building_repair_slot_0046a280(int32_t building)
{
 uint32_t slot;
 for(slot=0;slot<10u;++slot)if(!B(0x9783c0u+slot*28u)){
  B(0x799aa3u+(uint32_t)building*264u)=(uint8_t)(slot+1u);
  B(0x9783c0u+slot*28u)=1;U(0x9783d8u+slot*28u)=(uint32_t)building;return;
 }
 OutputDebugStringA((const char *)(uintptr_t)0x5dbba0u);
}
int32_t restore_building_allocate_0046a400(uint32_t owner,uint32_t kind,int32_t x,int32_t y,int32_t contiguous,int32_t selected)
{
 uint32_t id=(uint32_t)selected,r,config=kind*20u,i,limit,start,serial,cell,value,angle,quarter;
 int16_t type;
 if(!id){
  id=1;limit=(uint32_t)contiguous+1u;
  for(;;){
   if(!B(0x7999d8u+id*264u)&&!FUN_004851b0((int32_t)id)){
    if(contiguous<=1)break;
    start=limit+(1u-(uint32_t)contiguous);
    if((int32_t)start>=(int32_t)limit)break;
    for(i=start;(int32_t)i<(int32_t)limit;++i)if(B(0x7999d8u+i*264u))goto next;
    for(i=start;(int32_t)i<(int32_t)limit;++i)if(FUN_004851b0((int32_t)i))goto next;
    break;
   }
next:
   ++id;++limit;if(id>=4000u)goto failure;
  }
 }else if(B(0x7999d8u+id*264u)||FUN_004851b0((int32_t)id))goto failure;
 r=id*264u;
 B(0x7999d8u+r)=3;B(0x7999d9u+r)=(uint8_t)owner;B(0x7999dau+r)=B(0xea1875u+owner*18904u);
 W(0x7999e8u+r)=(uint16_t)kind;B(0x7999dbu+r)=B(0x5d06dcu+config);
 serial=U(0x78d7b4u);W(0x7999eeu+r)=(uint16_t)serial;B(0x799a9au+r)=50;W(0x7999f8u+r)=0;U(0x78d7b4u)=serial+1u;
 B(0x7999ddu+r)=0;type=H(0x7999e8u+r);
 if(type>=10&&type<=19)B(0x7999ddu+r)=1;
 else if(type>=20&&type<=23)B(0x7999ddu+r)=2;
 else if(type>=24&&type<=27)B(0x7999ddu+r)=3;
 else if(type>=28&&type<=29)B(0x7999ddu+r)=4;
 type=H(0x7999e8u+r);W(0x7999eau+r)=(type>=10&&type<=29)?(uint16_t)(kind-10u):0;
 W(0x799aa6u+r)=0;
 if(B(0x7999ddu+r)){
  U(0x799abcu+r)=U(0x70ce0cu+(uint32_t)(int32_t)H(0x7999eau+r)*88u);
  U(0x799ac0u+r)=0;U(0x799ad4u+r)=0;restore_building_quality_004e7f00((int32_t)id,1);
 }else U(0x799abcu+r)=0;
 type=H(0x7999e8u+r);
 if((type>=140&&type<=149)||type==202)B(0x799a82u+r)=(uint8_t)U(0xbc9714u);
 else if(type>=41&&type<=43){
  value=U(0xbc9714u);angle=(uint32_t)((int32_t)value%4);quarter=(uint32_t)((int32_t)(value+4u)/4);
  if((int32_t)quarter>=4)quarter=0;
  value=(uint32_t)((int32_t)U(0xbca2e0u)/2)+quarter+1u;if((int32_t)value>=4)value-=4u;
  B(0x799a82u+r)=(uint8_t)((uint8_t)(angle<<4)+(uint8_t)value);
 }
 type=H(0x7999e8u+r);
 /* This switch preserves every branch's ordered BYTE/WORD writes. */
 switch(type){
 case 0x66:B(0x799a1bu+r)=1;break;case 0x67:B(0x799a1bu+r)=3;break;case 0x68:B(0x799a1bu+r)=5;break;
 case 0x65:B(0x799a1bu+r)=16;break;case 0xe0:B(0x799a1bu+r)=32;break;case 0x69:B(0x799a1bu+r)=4;break;case 0x64:B(0x799a1bu+r)=14;break;
 case 0xc2:B(0x799a1bu+r)=2;W(0x7999eau+r)=13;break;case 0xc4:B(0x799a1bu+r)=6;break;case 0xc3:B(0x799a1bu+r)=22;break;
 case 0x6a:B(0x799a1bu+r)=24;break;case 0xdd:B(0x799a1bu+r)=30;break;case 0x6b:B(0x799a1bu+r)=25;break;case 0xd8:B(0x799a1bu+r)=26;break;
 case 0xa1:B(0x799a1bu+r)=21;break;case 0xd9:B(0x799a1bu+r)=29;break;case 0xa2:B(0x799a1bu+r)=18;break;case 0x6c:B(0x799a1bu+r)=20;break;
 case 0x6d:B(0x799a1bu+r)=11;break;case 0x6e:B(0x799a1bu+r)=15;W(0x7999eau+r)=9;break;case 0x6f:B(0x799a1bu+r)=17;W(0x7999eau+r)=1;break;
 case 0xcb:B(0x799a1bu+r)=23;W(0x7999eau+r)=11;break;case 0xcc:B(0x799a1bu+r)=12;W(0x7999eau+r)=5;W(0x799aa6u+r)=13;break;
 case 0xe8:B(0x799a1bu+r)=34;W(0x7999eau+r)=23;W(0x799aa6u+r)=24;break;case 0xe9:B(0x799a1bu+r)=33;W(0x7999eau+r)=20;break;
 case 0xcd:B(0x799a1bu+r)=28;W(0x7999eau+r)=4;break;case 0x70:B(0x799a1bu+r)=10;W(0x7999eau+r)=17;break;
 case 0x71:B(0x799a1bu+r)=19;W(0x7999eau+r)=10;break;case 0x72:B(0x799a1bu+r)=13;W(0x7999eau+r)=5;break;
 case 0xb1:B(0x799a1bu+r)=0;W(0x7999eau+r)=4;break;case 0xe7:B(0x799a1bu+r)=0;W(0x7999eau+r)=25;W(0x799aa6u+r)=5;break;
 case 0xb2:case 0xb3:B(0x799a1bu+r)=0;W(0x7999eau+r)=0;break;case 0x5f:B(0x799a1bu+r)=0;W(0x7999eau+r)=8;W(0x799aa6u+r)=14;break;
 case 0x4a:B(0x799a1bu+r)=0;W(0x7999eau+r)=4;restore_building_repair_slot_0046a280((int32_t)id);break;
 case 0xbb:case 0xbc:case 0xbd:B(0x799a1bu+r)=0;W(0x7999eau+r)=15;break;case 0x35:case 0x33:B(0x799a1bu+r)=0;W(0x7999eau+r)=12;break;
 case 0x2f:B(0x799a1bu+r)=0;W(0x7999eau+r)=16;break;case 0x20:B(0x799a1bu+r)=0;W(0x7999eau+r)=19;break;
 default:B(0x799a1bu+r)=0;if(type==0xe2){W(0x7999eau+r)=22;W(0x799aa6u+r)=13;}break;
 }
 type=H(0x7999e8u+r);if(type==0x47||type==0xbf)W(0x799a2eu+r)=3200;
 cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;W(0x7999deu+r)=(uint16_t)x;W(0x7999e0u+r)=(uint16_t)y;U(0x7999e4u+r)=cell;
 value=B(0xacf85cu+cell)&127u;B(0x799a25u+r)=(uint8_t)value;B(0x799a0cu+r)=(uint8_t)(value&6u);B(0x799a24u+r)=B(0x5d06e0u+config);
 B(0x799a98u+r)=(uint8_t)FUN_00481000(x,y,B(0x7999dbu+r));
 if(H(0x7999e8u+r)==136){
  value=U(0xbca310u);if(!value)U(0xbc9800u)=id;
  else if(value==1||value==2){value=U(0xbc9800u);U(0x799a9cu+r)=value;U(0x799a9cu+value*264u)=id;}
 }
 type=H(0x7999e8u+r);if(type==31||type==30||type==33){
  W(0x799a76u+r)=0;W(0x799a78u+r)=0;W(0x799a7au+r)=0;W(0x799a7cu+r)=0;B(0x799a7fu+r)=0;B(0x799a80u+r)=0;B(0x799a81u+r)=0;
 }
 type=H(0x7999e8u+r);
 if(type==115||type==108||type==177||type==178||type==179||type==231||type==195||restore_actor_building_gate((int32_t)id)||H(0x7999e8u+r)==199)B(0x799a7eu+r)=0;
 if((int32_t)U(0x78d790u)<=(int32_t)id)U(0x78d790u)=id;
 return (int32_t)id;
failure:FUN_004e2970(65);return 0;
}
