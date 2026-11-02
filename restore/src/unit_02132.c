#include "pyramid_work_selection.h"
/* Whole native4F2BA0:327 bytes,95 instructions. Compare remaining route
   lengths of linked type85 actors and keep at least seven steps apart. */
uint8_t restore_monument_worker_route_has_gap(int32_t actor)
{
 uint32_t record=restore_monument_worker_record(actor),part,root,index,kind,table,other_record,linked_record;
 int32_t building,remaining,owner,linked,delta;
 if(MW_H(record+PS_ROUTE_SLOT)<=0)return 1;
 building=MW_H(0xb4e6fcu+MW_U(record+MW_TARGET_CELL)*2u);
 if(!building)return 1;
 part=restore_monument_worker_building(building);
 if(MW_W(part+16u)!=183u)return 1;
 remaining=MW_H(record+PS_ROUTE_LENGTH)-MW_H(record+PS_ROUTE_PROGRESS);
 root=(uint32_t)building-(uint32_t)MW_H(part+18u);
 part=restore_monument_worker_building((int32_t)root);kind=MW_B(part+84u);
 for(index=root;;++index,part+=264u){
  table=MW_U(0x5e4ef4u)==5u?0x56f7ecu:0x56f7d8u;
  if((int32_t)index>=(int32_t)(root+MW_U(table+kind*4u)))return 1;
  owner=MW_H(part+100u);other_record=restore_monument_worker_record(owner);
  if(!((MW_B(other_record+MW_TYPE)==81u&&MW_B(other_record+MW_STATE)==10u)||
       (MW_B(other_record+MW_TYPE)==80u&&MW_B(other_record+MW_STATE)==11u)))continue;
  linked=MW_H(other_record+PS_LINKED_ACTOR);if(!linked)continue;
  linked_record=restore_monument_worker_record(linked);
  if(MW_B(linked_record+MW_TYPE)!=85u||MW_B(linked_record+MW_STATE)!=17u||MW_H(linked_record+PS_ROUTE_SLOT)<=0)continue;
  delta=MW_H(linked_record+PS_ROUTE_LENGTH)-MW_H(linked_record+PS_ROUTE_PROGRESS)-remaining;
  if(delta<0)delta=-delta;
  if(delta<7)return 0;
 }
}
uint8_t FUN_004f2ba0(int32_t actor){return restore_monument_worker_route_has_gap(actor);}
