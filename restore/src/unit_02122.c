#include "monument_worker_motion.h"
/* Whole native 4F2230:467 bytes,155 instructions. The CALL4F2190 only
   consumes one argument. Its two delayed stack values are the remaining
   position/select_action arguments for the subsequent CALL4F02F0. */
static uint8_t available(int32_t building)
{
 int32_t owner=MW_H(restore_monument_worker_building(building)+100u);
 uint32_t record;uint8_t type;
 if(owner<=0)return 1;
 record=restore_monument_worker_record(owner);type=MW_B(record+MW_TYPE);
 if(type==81&&MW_B(record+MW_STATE)==11)return 0;
 if(type==80&&MW_B(record+MW_STATE)==12)return 0;
 return 1;
}
void restore_monument_worker_find_work(int32_t actor)
{
 uint32_t record=restore_monument_worker_record(actor),i,count,piece;
 int32_t original,current,next;
 uint16_t position;
 if(MW_H(record+MW_MONUMENT_ACTION)<1){MW_B(record+MW_STATE)=12;return;}
 if(restore_monument_worker_advance_travel(actor,1)!=1)return;
 restore_monument_worker_snap_to_target(actor);
 original=MW_H(record+MW_TO_BUILDING);
 restore_pyramid_collect_work_pieces(original-MW_H(restore_monument_worker_building(original)+18u));
 count=MW_U(0xeaaf58u);
 for(i=0;(int32_t)i<(int32_t)count;++i){
  piece=MW_U(0xeaac44u+i*4u);
  if((int32_t)piece==original&&available((int32_t)piece)){
   MW_B(record+MW_STATE)=11;MW_W(record+MW_ACTION_CLOCK)=0;
   MW_W(restore_monument_worker_building((int32_t)piece)+100u)=(uint16_t)actor;return;
  }
 }
 current=original;
 for(;;){
  count=MW_U(0xeaaf58u);
  for(i=0;(int32_t)i<(int32_t)count;++i){
   piece=MW_U(0xeaac44u+i*4u);
   if((int32_t)piece==current&&available((int32_t)piece)){
    position=(uint16_t)MW_H(record+MW_TO_POSITION);
    MW_W(restore_monument_worker_building((int32_t)piece)+100u)=(uint16_t)actor;
    /* Native starts this leg from the original piece, even when the
       reservation was found later in the ring. Preserve that choice. */
    next=restore_pyramid_next_work_piece(original);
    restore_monument_worker_begin_travel(actor,next,position,1);return;
   }
  }
  current=restore_pyramid_next_work_piece(current);
  if(current==original){MW_B(record+MW_STATE)=12;MW_W(record+MW_MONUMENT_ACTION)=0;return;}
 }
}
void FUN_004f2230(int32_t actor){restore_monument_worker_find_work(actor);}
