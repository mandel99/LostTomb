#include "monument_worker_motion.h"
#include "building_quality_update.h"
extern uint8_t FUN_0051cd80(int32_t,int32_t,int32_t,int32_t);
/* Whole native 4F02F0:434 bytes,120 instructions. The lost incoming ST0
   decompiler expression is sqrt(signed wrapped DWORD squared distance),
   truncated to an integer before multiplying by15. */
void restore_monument_worker_begin_travel(int32_t actor,int32_t building,uint16_t position,uint8_t select_action)
{
 uint32_t record=restore_monument_worker_record(actor);
 uint32_t destination=restore_monument_worker_building(building),previous;
 uint32_t dx,dy,length,kind,index;
 int32_t old_source=MW_H(record+MW_FROM_BUILDING),x,y,shape,rotation;
 uint8_t raw80[10];
 MW_W(record+MW_FROM_BUILDING)=MW_W(record+MW_TO_BUILDING);
 MW_W(record+MW_FROM_POSITION)=MW_W(record+MW_TO_POSITION);
 MW_W(record+MW_TO_BUILDING)=(uint16_t)building;
 MW_W(record+MW_TO_POSITION)=position;
 x=MW_H(destination+6u);y=MW_H(destination+8u);
 MW_W(record+MW_TARGET_X)=(uint16_t)x;
 MW_W(record+MW_TARGET_Y)=(uint16_t)y;
 MW_U(record+MW_TARGET_CELL)=MW_U(destination+12u);
 dx=(uint32_t)MW_H(record+MW_TARGET_X)-(uint32_t)MW_H(record+MW_X);
 dy=(uint32_t)MW_H(record+MW_TARGET_Y)-(uint32_t)MW_H(record+MW_Y);
 MW_W(record+MW_TRAVEL_PROGRESS)=0;
 length=(uint32_t)restore_building_sqrt_trunc64((int32_t)(dx*dx+dy*dy),raw80);
 MW_W(record+MW_TRAVEL_LENGTH)=(uint16_t)(length*15u);
 /* Direction uses the old source, saved before shifting the two links. */
 previous=restore_monument_worker_building(old_source);
 MW_B(record+MW_DIRECTION)=FUN_0051cd80(MW_H(previous+6u),MW_H(previous+8u),x,y);
 if(!select_action)return;
 kind=MW_B(destination+84u);
 if(MW_U(0x5e4ef4u)==5u){
  index=kind*36u+(uint32_t)MW_H(destination+18u);
  shape=(int32_t)MW_U(0x5e6578u+index*8u);
 }else{
  index=kind*144u+(uint32_t)MW_H(destination+18u);
  shape=(int32_t)MW_U(0x5e4ef8u+index*8u);
 }
 rotation=(int32_t)MW_U(0xbca2e0u)/2;
 if(shape<4){shape=(int32_t)((uint32_t)shape+(uint32_t)rotation);if(shape>=4)shape-=4;}
 else if(shape<8){shape=(int32_t)((uint32_t)shape+(uint32_t)rotation);if(shape>=8)shape-=4;}
 switch(shape){
 case 0:case 1:case 4:case 7:MW_W(record+MW_MONUMENT_ACTION)=3;break;
 default:MW_W(record+MW_MONUMENT_ACTION)=2;break;
 }
}
void FUN_004f02f0(int32_t actor,int32_t building,uint16_t position,uint8_t select_action)
{restore_monument_worker_begin_travel(actor,building,position,select_action);}
