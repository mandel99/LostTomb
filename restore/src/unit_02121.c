#include "monument_worker_motion.h"
/* Whole native 4F2190:157 bytes,74 instructions. Six fixed perimeter
   descriptors encode two opposing runs and alternating interior links. */
int32_t restore_pyramid_next_work_piece(int32_t building)
{
 int32_t piece=MW_H(restore_monument_worker_building(building)+18u),i;
 uint32_t root=(uint32_t)building-(uint32_t)piece;
 for(i=0;i<6;++i){
  uint32_t descriptor=0x5f5ca8u+(uint32_t)i*16u;
  int32_t low=(int32_t)MW_U(descriptor),high=(int32_t)MW_U(descriptor+12u);
  int32_t end,start;
  if(piece<low||piece>high)continue;
  end=(int32_t)MW_U(descriptor+4u);
  if(piece>=low&&piece<=end){
   if(piece==low)return (int32_t)(root+(uint32_t)end+1u);
   return (int32_t)((uint32_t)building-1u);
  }
  start=(int32_t)MW_U(descriptor+8u);
  if(piece>=start&&piece<=high){
   if(piece==high)return (int32_t)(root+(uint32_t)start-1u);
   return (int32_t)((uint32_t)building+1u);
  }
  return (int32_t)((uint32_t)building+((piece&1)?0xfffffffeu:2u));
 }
 return building;
}
int32_t FUN_004f2190(int32_t building){return restore_pyramid_next_work_piece(building);}
