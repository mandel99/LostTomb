#include "pyramid_work_selection.h"
/* Whole native4EF100:250 bytes,87 instructions. The first two thresholds
   in each20-byte descriptor select worker slot1 or2. Both completed part
   and completed root phases suppress further assignments. */
int32_t restore_pyramid_available_worker_slot(int32_t building)
{
 uint32_t record=restore_monument_worker_building(building),kind,root,i,count,table;
 int32_t piece,threshold,position;
 if(MW_U(0x5e4ef4u)==5u)return 0;
 kind=MW_B(record+84u);
 if(MW_B(record+85u)==1u){
  root=(uint32_t)building-(uint32_t)MW_H(record+18u);
  if(MW_B(restore_monument_worker_building((int32_t)root)+85u)==1u)return 0;
 }
 count=MW_U(0x570070u+kind*4u);piece=MW_H(record+18u);table=0x5f3878u+kind*960u;
 for(i=0;(int32_t)i<(int32_t)count;++i){
  uint32_t descriptor=table+i*20u;
  if(piece!=(int32_t)MW_U(descriptor))continue;
  threshold=(int32_t)MW_U(descriptor+4u);
  position=(int32_t)(MW_B(record+86u)+MW_B(record+87u)*6u);
  if(threshold>0&&threshold<=position&&!MW_B(record+89u))return 1;
  threshold=(int32_t)MW_U(descriptor+8u);
  if(threshold<=0||position<threshold||MW_B(record+90u))return 0;
  return 2;
 }
 return 0;
}
int32_t FUN_004ef100(int32_t building){return restore_pyramid_available_worker_slot(building);}
