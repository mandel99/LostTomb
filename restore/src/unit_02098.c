#include "pyramid_work_selection.h"
/* Whole native4EEA10:306 bytes,121 instructions. Only AL is a logical
   return. Unused claims are cleared; unrelated live actor states retain
   the claim WORD while returning false, as in the original. */
uint8_t restore_pyramid_worker_claim_active(int32_t building)
{
 uint32_t record=restore_monument_worker_building(building),actor_record;
 int32_t owner=MW_H(record+100u);uint32_t state,type;
 if(!owner)return 0;
 actor_record=restore_monument_worker_record(owner);
 if(!MW_B(actor_record+PS_CLAIM_VALIDITY))goto clear;
 state=MW_B(actor_record+MW_STATE);
 if(state==6u||state==7u)state=MW_B(actor_record+PS_SAVED_STATE);
 type=MW_B(actor_record+MW_TYPE);
 switch(type){
 case 79:return (uint8_t)(state==9u||state==12u||state==10u);
 case 80:return (uint8_t)(state==10u||state==13u||state==15u||state==12u||state==14u||state==11u);
 case 81:
  if(restore_pyramid_available_worker_slot(building))goto clear;
  if(state==9u||state==13u||state==15u||state==11u||state==14u||state==16u)return 1;
  if(state==10u){
   if(MW_H(0xb4e6fcu+MW_U(actor_record+MW_TARGET_CELL)*2u)==building)return 1;
   goto clear;
  }
  return 0;
 case 85:return (uint8_t)(state==15u||state==16u||state==17u||state==30u||state==18u||state==19u||state==20u||state==21u||state==29u||state==22u);
 default:return 0;
 }
clear:
 MW_W(record+100u)=0;return 0;
}
uint8_t FUN_004eea10(int32_t building){return restore_pyramid_worker_claim_active(building);}
