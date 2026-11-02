#include "pyramid_worker_state.h"
/* Whole native4F29E0:436 bytes,138 instructions. Keep the assigned actor
   cached across claim validation. A missing paired piece returns-1 and
   intentionally passes the native nonzero test here. */
int32_t restore_pyramid_find_available_worker(int32_t building,uint8_t detach_waiting)
{
 uint32_t i,record=restore_monument_worker_building(building),actor_record,linked_record,state;
 int32_t piece,owner,linked;uint8_t has_link,busy;
 restore_pyramid_collect_work_pieces(building-MW_H(record+18u));
 restore_pyramid_sort_work_list();
 for(i=0;(int32_t)i<(int32_t)MW_U(0xeaaf58u);++i){
  piece=(int32_t)MW_U(0xeaac44u+i*4u);
  if(restore_pyramid_is_second_work_piece(piece))continue;
  record=restore_monument_worker_building(piece);owner=MW_H(record+100u);
  if(!owner)continue;
  if(!restore_pyramid_worker_claim_active(piece)){
   if(MW_W(record+100u))MW_W(record+100u)=0;
   continue;
  }
  actor_record=restore_monument_worker_record(owner);
  if(!((MW_B(actor_record+MW_TYPE)==81u&&MW_B(actor_record+MW_STATE)==10u)||
       (MW_B(actor_record+MW_TYPE)==80u&&MW_B(actor_record+MW_STATE)==11u)))continue;
  linked=MW_H(actor_record+PS_LINKED_ACTOR);has_link=0;busy=0;
  if(linked&&restore_pyramid_linked_worker_active(linked,owner)){
   has_link=1;linked_record=restore_monument_worker_record(linked);
   state=MW_B(linked_record+MW_STATE);if(state==6u||state==7u)state=MW_B(linked_record+PS_SAVED_STATE);
   if(state==18u||state==19u||state==20u||state==29u||state==17u||state==30u)busy=1;
  }
  if(restore_pyramid_paired_worker(piece)!=0){
   if(!has_link)return owner;
   if(!busy&&detach_waiting){MW_W(actor_record+PS_LINKED_ACTOR)=0;return owner;}
  }
 }
 return 0;
}
int32_t FUN_004f29e0(int32_t building,uint8_t detach_waiting)
{return restore_pyramid_find_available_worker(building,detach_waiting);}
