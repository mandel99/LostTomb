#include "pyramid_worker_state.h"
extern void FUN_004b5420(int32_t),FUN_004b52f0(int32_t);
/* Whole native4F04D0:999 bytes,293 instructions. The current target record
   and layout table stay cached across grid callbacks. Mode is a full DWORD;
   actor states are BYTEs and action/progress/position values are WORDs. */
void restore_pyramid_worker_update_travel(int32_t actor,int32_t mode,uint8_t failed_state,uint8_t ready_state)
{
 uint32_t record=restore_monument_worker_record(actor),building_record,table,limit,i,root,first=0,next_record;
 int32_t building,destination;uint16_t old_target;uint8_t same;
 building=MW_H(0xb4e6fcu+MW_U(record+MW_TARGET_CELL)*2u);
 building_record=restore_monument_worker_building(building);
 if(MW_W(building_record+16u)!=183u){
  MW_B(restore_monument_worker_record((int32_t)MW_U(0xd056f8u))+PS_CLAIM_VALIDITY)=2;return;
 }
 if(restore_monument_worker_advance_travel(actor,1)!=1)return;
 if(MW_U(0x5e4ef4u)==5u){limit=16;table=0x5f5498u+MW_B(building_record+84u)*128u;}
 else{limit=60;table=0x5f4b38u+MW_B(building_record+84u)*480u;}
 FUN_004b5420(actor);
 MW_W(record+MW_X)=MW_W(record+MW_TARGET_X);MW_W(record+MW_Y)=MW_W(record+MW_TARGET_Y);
 MW_U(record+MW_CELL)=MW_U(record+MW_TARGET_CELL);
 FUN_004b52f0(actor);
 if(mode==1){
  if(MW_W(building_record+100u)==(uint16_t)actor){
   if(restore_pyramid_available_worker_slot(building))goto ready;
   goto failed;
  }
  root=(uint32_t)building-(uint32_t)MW_H(building_record+18u);
  for(i=0;i<limit;++i){
   destination=(int32_t)(root+MW_U(table+i*8u));
   if(MW_W(restore_monument_worker_building(destination)+100u)==(uint16_t)actor&&restore_pyramid_available_worker_slot(destination))goto arrived;
  }
  goto failed;
 }
arrived:
 if(MW_H(record+MW_MONUMENT_ACTION)==1){
  restore_pyramid_collect_work_pieces(building-MW_H(building_record+18u));
  if((int32_t)MW_U(0xeaaf58u)<1)goto failed;
  next_record=restore_monument_worker_building((int32_t)MW_U(0xeaac44u));
  if(MW_H(record+MW_TO_POSITION)>=(int32_t)(MW_B(next_record+86u)+MW_B(next_record+87u)*6u))goto ready;
  i=0;goto next_leg;
 }
 if(mode==0||mode==2){
  restore_pyramid_collect_work_pieces(building-MW_H(building_record+18u));
  if((int32_t)MW_U(0xeaaf58u)<1)goto failed;
  first=MW_U(0xeaac44u);next_record=restore_monument_worker_building((int32_t)first);
  if(MW_H(record+MW_TO_POSITION)>=(int32_t)(MW_B(next_record+86u)+MW_B(next_record+87u)*6u))goto ready;
 }
 for(i=0;i<limit;++i){
  if((int32_t)MW_U(table+i*8u)==MW_H(building_record+18u)&&
     (int32_t)MW_U(table+i*8u+4u)==MW_H(record+MW_TO_POSITION))break;
 }
 if(i==limit)goto failed;
 ++i;
 /* The native code deliberately reads the next table row even when the
    matching row was the last nominal entry. Do not add a different bound. */
 if((int32_t)MW_U(table+i*8u)<0){
  if(mode==1)goto failed;
  if(mode==0&&MW_B(record+MW_TYPE)==81u&&MW_B(restore_monument_worker_building((int32_t)first)+85u)==1u){
   MW_B(record+MW_STATE)=16;
   destination=restore_pyramid_next_work_piece(MW_H(record+MW_TO_BUILDING));
   restore_monument_worker_begin_travel(actor,destination,(uint16_t)MW_H(record+MW_TO_POSITION),0);return;
  }
  goto ready;
 }
next_leg:
 destination=(int32_t)((uint32_t)building-(uint32_t)MW_H(building_record+18u)+MW_U(table+i*8u));
 if(mode==2&&restore_pyramid_available_worker_slot(destination)){
  /* Exact native BYTE1 reset; its wider actor-state meaning is pending. */
  MW_B(record+1u)=0;return;
 }
 old_target=MW_W(record+MW_TO_BUILDING);
 same=(uint8_t)(destination==(int32_t)(int16_t)old_target);
 restore_monument_worker_begin_travel(actor,destination,(uint16_t)MW_U(table+i*8u+4u),1);
 if(same)MW_W(record+MW_TRAVEL_PROGRESS)=MW_W(record+MW_TRAVEL_LENGTH);
 return;
ready:
 MW_W(record+MW_MONUMENT_ACTION)=4;MW_B(record+MW_STATE)=ready_state;return;
failed:
 MW_B(record+MW_STATE)=failed_state;MW_W(record+MW_MONUMENT_ACTION)=0;
}
void FUN_004f04d0(int32_t actor,int32_t mode,uint8_t failed_state,uint8_t ready_state)
{restore_pyramid_worker_update_travel(actor,mode,failed_state,ready_state);}
