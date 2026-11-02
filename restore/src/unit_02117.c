#include "pyramid_work_selection.h"
/* Whole native4F0EF0:1030 bytes,322 instructions. Selection first favors
   an unclaimed piece over one owned by another actor, then the larger
   signed wrapped distance score; ties use paired-worker and pair flags. */
static int32_t squared_distance(int32_t x,int32_t y,uint32_t record)
{
 uint32_t dx=(uint32_t)x-(uint32_t)MW_H(record+6u),dy=(uint32_t)y-(uint32_t)MW_H(record+8u);
 return (int32_t)(dx*dx+dy*dy);
}
void restore_pyramid_worker_select_piece(int32_t actor,uint8_t refresh_position,
 uint8_t failed_state,uint8_t ready_state,uint8_t travelling_state)
{
 uint32_t record=restore_monument_worker_record(actor),building_record,table,count,i,kind;
 int32_t building,root,anchor,position,x,y,candidate,best=0,best_score=0,score,pair,paired_score;
 int32_t owner,best_owner,mode;uint8_t best_pair=0,best_worker=0,has_pair,has_worker,second;
 building=MW_H(0xb4e6fcu+MW_U(record+MW_CELL)*2u);
 building_record=restore_monument_worker_building(building);
 if(MW_W(building_record+16u)!=183u)goto failed;
 root=building-MW_H(building_record+18u);
 restore_pyramid_collect_work_pieces(root);
 if((int32_t)MW_U(0xeaaf58u)<1)goto failed;
 anchor=(int32_t)MW_U(0xeaac44u);
 building_record=restore_monument_worker_building(anchor);
 anchor=(int32_t)((uint32_t)anchor-(uint32_t)MW_H(building_record+18u));
 position=(int32_t)(MW_B(building_record+86u)+MW_B(building_record+87u)*6u);
 if(refresh_position)MW_W(record+MW_TO_POSITION)=(uint16_t)position;
 if(position!=MW_H(record+MW_TO_POSITION))goto failed;
 kind=MW_B(restore_monument_worker_building(root)+84u);
 if(MW_U(0x5e4ef4u)==5u){table=0x5f5498u+kind*128u;count=16;}
 else{table=0x5f4b38u+kind*480u;count=60;}
 root=anchor;
 for(i=0;i<count;++i){
  anchor=(int32_t)((uint32_t)root+MW_U(table+i*8u));
  if((int32_t)MW_U(table+i*8u+4u)>=position)break;
 }
 building_record=restore_monument_worker_building(anchor);x=MW_H(building_record+6u);y=MW_H(building_record+8u);
 for(i=0;(int32_t)i<(int32_t)MW_U(0xeaaf58u);++i){
  candidate=(int32_t)MW_U(0xeaac44u+i*4u);
  has_pair=restore_pyramid_piece_has_pending_work(candidate);
  mode=(int32_t)MW_U(0x5e4ef4u);
  if(mode==2){
   if(MW_B(record+MW_TYPE)==80u){if(!has_pair)continue;}
   else if(has_pair)continue;
  }else if(mode!=1&&mode!=3&&mode!=4&&mode!=5)continue;
  building_record=restore_monument_worker_building(candidate);
  score=squared_distance(x,y,building_record);
  pair=restore_pyramid_find_paired_work_piece(candidate,&second);has_pair=(uint8_t)(pair>0);
  if(has_pair){
   paired_score=squared_distance(x,y,restore_monument_worker_building(pair));
   if(score<paired_score)score=paired_score;
  }
  score=(int32_t)((uint32_t)score*2u);
  has_worker=(uint8_t)(restore_pyramid_paired_worker(candidate)>0);
  if(best){
   best_owner=MW_H(restore_monument_worker_building(best)+100u);owner=MW_H(building_record+100u);
   if(best_owner>0&&best_owner!=actor){
    if(owner==0||owner==actor)goto choose;
   }else if(owner!=0&&owner!=actor)continue;
   if(score<best_score)continue;
   if(score==best_score&&!((has_worker&&!best_worker)||(has_pair&&!best_pair)))continue;
  }
choose:
  best=candidate;best_score=score;best_worker=has_worker;best_pair=has_pair;
 }
 if(!best)goto failed;
 building_record=restore_monument_worker_building(best);owner=MW_H(building_record+100u);
 if(owner!=0&&owner!=actor&&restore_pyramid_worker_claim_active(best))goto failed;
 if(MW_B(building_record+85u)==1u){
  MW_W(building_record+100u)=0;MW_B(record+MW_STATE)=ready_state;return;
 }
 MW_W(building_record+100u)=(uint16_t)actor;
 MW_W(record+MW_MONUMENT_ACTION)=4;MW_B(record+MW_STATE)=travelling_state;
 restore_monument_worker_begin_travel(actor,best,(uint16_t)MW_H(record+MW_FROM_POSITION),0);return;
failed:
 MW_B(record+MW_STATE)=failed_state;MW_W(record+MW_MONUMENT_ACTION)=0;
}
void FUN_004f0ef0(int32_t actor,uint8_t refresh_position,uint8_t failed_state,uint8_t ready_state,uint8_t travelling_state)
{restore_pyramid_worker_select_piece(actor,refresh_position,failed_state,ready_state,travelling_state);}
