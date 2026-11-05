#include "actor_value_contracts.h"
#include "pyramid_work_selection.h"
#include "editor_brush_validation.h"

/* 4EFE30: native selection returns an actual piece/root ID, or zero.
   The first two legacy arguments are unused. Work-list counts and family
   are live across children; the outer root and its byte offset are cached. */
int32_t restore_pyramid_select_idle_piece_or_access(int32_t actor,int32_t mode)
{
 uint32_t root=1,record,list,index,piece,claim,family,kind,target,actor_record;
 uint32_t chosen,other,scan,step;
 int32_t delta;uint16_t target_y;uint8_t pending,conflict;
 actor_record=restore_monument_worker_record(actor);
 for(;;){
  record=restore_monument_worker_building((int32_t)root);
  if(MW_B(record)==1u&&MW_W(record+16u)==183u){
   if(MW_W(record+200u)!=0){
    restore_pyramid_collect_work_pieces((int32_t)root);
    if((int32_t)MW_U(0xeaaf58u)>0){
     list=0xeaac44u;index=0;
     do{
      piece=MW_U(list);
      if(!restore_pyramid_available_worker_slot((int32_t)piece)){
       claim=restore_monument_worker_building((int32_t)piece)+100u;
       if(MW_W(claim)!=0){
        if(!restore_pyramid_worker_claim_active((int32_t)piece))MW_W(claim)=0;
        if(MW_W(claim)!=0)goto next_piece;
       }
       pending=restore_pyramid_piece_has_pending_work((int32_t)piece);
       family=MW_U(0x5e4ef4u);
       if(family==1u||family==3u||family==4u||family==5u||
          (family==2u&&(mode==0?pending!=0:pending==0))){
        MW_W(restore_monument_worker_building((int32_t)piece)+100u)=(uint16_t)actor;
        kind=MW_B(record+84u);
        if(family==5u){
         target=root+MW_U(0x5f5498u+kind*128u);
         target=restore_monument_worker_building((int32_t)target);
         MW_W(actor_record+MW_TARGET_X)=MW_W(target+6u);
         target_y=(uint16_t)(MW_W(target+8u)+2u);
        }else{
         target=root+MW_U(0x5f3878u+kind*960u);
         target=restore_monument_worker_building((int32_t)target);
         MW_W(actor_record+MW_TARGET_X)=MW_W(target+38u);
         target_y=MW_W(target+40u);
        }
        chosen=MW_U(target+12u);
        MW_W(actor_record+MW_TARGET_Y)=target_y;
        MW_U(actor_record+MW_TARGET_CELL)=chosen;
        MW_W(actor_record+RESTORE_PYRAMID_SEARCH_ROUTE_RESET)=0;
        return (int32_t)piece;
       }
      }
next_piece:
      ++index;list+=4u;
     }while((int32_t)index<(int32_t)MW_U(0xeaaf58u));
    }
   }
   if(mode==1&&MW_B(record+85u)<=1u){
    kind=MW_B(record+84u);
    if(FUN_004ecea0((int32_t)kind)){
     delta=(int32_t)(MW_U(0x5f5d08u+kind*4u)+MW_U(record+12u)-MW_U(0xbca2d8u));
     /* The brush child's physical EAX is discarded. Read its memory output. */
     FUN_004ed360(delta%228,delta/228,3,0);
     chosen=MW_U(0xeaae90u);
     if(chosen){
      conflict=0;other=1;scan=0xbcd786u;
      do{
       if((uint32_t)actor!=other&&MW_B(scan+4u)!=0&&
          MW_B(scan)==MW_B(actor_record+MW_TYPE)&&MW_U(scan+30u)==chosen&&
          MW_H(scan+84u)==(int32_t)root){conflict=1;break;}
       scan+=388u;++other;
      }while((int32_t)scan<(int32_t)0xc8ad42u);
      if(!conflict){
       delta=(int32_t)(chosen-MW_U(0xbca2d8u));
       MW_W(actor_record+MW_TARGET_X)=(uint16_t)(delta%228);
       MW_W(actor_record+MW_TARGET_Y)=(uint16_t)(delta/228);
       MW_U(actor_record+MW_TARGET_CELL)=chosen;
       MW_W(actor_record+RESTORE_PYRAMID_SEARCH_ROUTE_RESET)=0;
       MW_W(actor_record+RESTORE_PYRAMID_SEARCH_CLAIM_ROOT)=(uint16_t)root;
       return (int32_t)root;
      }
     }
    }
   }
   family=MW_U(0x5e4ef4u);kind=MW_B(record+84u);
   step=MW_U((family==5u?0x56f7ecu:0x56f7d8u)+kind*4u);
   root=root+step-1u;
  }
  ++root;
  if((int32_t)root>=4000)return 0;
 }
}
