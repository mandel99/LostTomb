#include "monument_layout_queries.h"
void restore_monument_set_layout_target(int32_t actor,int32_t building)
{
 uint32_t source=restore_monument_worker_building(building),root,mode,kind,target,destination,cell;
 uint16_t y;
 root=(uint32_t)building-(uint32_t)MW_H(source+18u);
 mode=MW_U(0x5e4ef4u);
 kind=MW_B(restore_monument_worker_building((int32_t)root)+84u);
 root+=MW_U(mode==5u ? 0x5f5498u+kind*128u : 0x5f3878u+kind*960u);
 target=restore_monument_worker_building((int32_t)root);
 destination=restore_monument_worker_record(actor);
 MW_W(destination+MW_TARGET_X)=MW_W(target+38u);
 y=MW_W(target+40u);
 cell=MW_U(target+12u);
 MW_W(destination+MW_TARGET_Y)=y;
 MW_U(destination+MW_TARGET_CELL)=cell;
 MW_W(destination+0x44u)=0;
}
void FUN_004f3000(int32_t actor,int32_t building)
{ restore_monument_set_layout_target(actor,building); }
