#include "monument_worker_motion.h"
extern uint8_t FUN_0051cd80(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004b5420(int32_t),FUN_004b52f0(int32_t);
/* Whole native 4F1D10:1103 bytes,313 instructions. Advance progress as a
   wrapping signed WORD; resolve actual positions from the monument parts,
   including interpolation and the three view-dependent neighbor probes. */
uint8_t restore_monument_worker_advance_travel(int32_t actor,int16_t amount)
{
 uint32_t record=restore_monument_worker_record(actor),source,destination,selected;
 uint32_t kind,table,neighbor_table,cell;
 int32_t from,to,rotation,x,y,first,last,candidate,shape,facing,i;
 int32_t progress,length;uint8_t complete;
 if(MW_H(record+MW_TRAVEL_LENGTH)<1)return 1;
 MW_W(record+MW_TRAVEL_PROGRESS)=(uint16_t)(MW_W(record+MW_TRAVEL_PROGRESS)+(uint16_t)amount);
 progress=MW_H(record+MW_TRAVEL_PROGRESS);length=MW_H(record+MW_TRAVEL_LENGTH);
 complete=(uint8_t)(progress>=length);
 if(complete)MW_W(record+MW_TRAVEL_PROGRESS)=(uint16_t)length;
 from=MW_H(record+MW_FROM_BUILDING);to=MW_H(record+MW_TO_BUILDING);
 source=restore_monument_worker_building(from);destination=restore_monument_worker_building(to);
 if(from)MW_B(record+MW_DIRECTION)=FUN_0051cd80(MW_H(source+6u),MW_H(source+8u),MW_H(destination+6u),MW_H(destination+8u));
 else MW_B(record+MW_DIRECTION)=0;
 rotation=(int32_t)MW_U(0xbca2e0u)/2;
 if(!from){
  x=MW_H(destination+6u);y=MW_H(destination+8u);
  cell=MW_U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
 }else if(MW_H(record+MW_MONUMENT_ACTION)==4){
  first=from-MW_H(source+18u);kind=MW_B(source+84u);
  table=MW_U(0x5e4ef4u)==5u?0x56f7ecu:0x56f7d8u;
  last=(int32_t)(MW_U(table+kind*4u)+(uint32_t)first-1u);
  x=MW_H(source+6u);y=MW_H(source+8u);
  progress=MW_H(record+MW_TRAVEL_PROGRESS);length=MW_H(record+MW_TRAVEL_LENGTH);
  /* IMUL keeps the low DWORD before signed IDIV. */
  y=(int32_t)((uint32_t)y+(uint32_t)((int32_t)((uint32_t)(MW_H(destination+8u)-y)*(uint32_t)progress)/length));
  x=(int32_t)((uint32_t)x+(uint32_t)((int32_t)((uint32_t)(MW_H(destination+6u)-x)*(uint32_t)progress)/length));
  cell=MW_U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
  selected=restore_monument_worker_building(MW_H(0xb4e6fcu+cell*2u));
  x=MW_H(selected+6u);y=MW_H(selected+8u);cell=MW_U(selected+12u);
  neighbor_table=0x5700f4u+(uint32_t)rotation*12u;
  for(i=0;i<3;++i){
   candidate=MW_H(0xb4e6fcu+(cell+MW_U(neighbor_table+(uint32_t)i*4u))*2u);
   if(candidate>=first&&candidate<=last){
    selected=restore_monument_worker_building(candidate);
    x=MW_H(selected+6u);y=MW_H(selected+8u);break;
   }
  }
  cell=MW_U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
 }else{
  kind=MW_B(source+84u);
  if(MW_U(0x5e4ef4u)==5u){table=0x5e6578u;selected=kind*36u+(uint32_t)MW_H(source+18u);}
  else{table=0x5e4ef8u;selected=kind*144u+(uint32_t)MW_H(source+18u);}
  shape=(int32_t)MW_U(table+selected*8u);
  if(shape<4){shape=(int32_t)((uint32_t)shape+(uint32_t)rotation);if(shape>=4)shape-=4;}
  else if(shape<8){shape=(int32_t)((uint32_t)shape+(uint32_t)rotation);if(shape>=8)shape-=4;}
  /* Native uses the current actor's DWORD global index for this facing test,
     which need not equal the explicit actor argument. */
  facing=(int32_t)((uint32_t)(int32_t)(int8_t)MW_B(0xbcd609u+MW_U(0xd056f8u)*388u)-MW_U(0xbca2e0u)+1u);
  if(facing<0)facing+=8;
  selected=source;
  if(shape==1||shape==3||(shape==5&&facing>=2&&facing<=5))selected=destination;
  x=MW_H(selected+6u);y=MW_H(selected+8u);cell=MW_U(selected+12u);
  if(shape==0||shape==7){
   first=from-MW_H(source+18u);
   table=MW_U(0x5e4ef4u)==5u?0x56f7ecu:0x56f7d8u;
   last=(int32_t)(MW_U(table+kind*4u)+(uint32_t)first-1u);
   cell=MW_U(source+12u);
   neighbor_table=0x5700f4u+(((uint32_t)rotation-2u)&3u)*12u;
   for(i=0;i<3;++i){
    candidate=MW_H(0xb4e6fcu+(cell+MW_U(neighbor_table+(uint32_t)i*4u))*2u);
    if(candidate>=first&&candidate<=last){
     selected=restore_monument_worker_building(candidate);
     x=MW_H(selected+6u);y=MW_H(selected+8u);break;
    }
   }
   cell=MW_U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
  }
 }
 if(MW_H(record+MW_X)!=x||MW_H(record+MW_Y)!=y){
  FUN_004b5420(actor);
  MW_W(record+MW_X)=(uint16_t)x;MW_W(record+MW_Y)=(uint16_t)y;MW_U(record+MW_CELL)=cell;
  FUN_004b52f0(actor);
 }else{
  MW_W(record+MW_X)=(uint16_t)x;MW_W(record+MW_Y)=(uint16_t)y;MW_U(record+MW_CELL)=cell;
 }
 return complete;
}
uint8_t FUN_004f1d10(int32_t actor,int16_t amount)
{return restore_monument_worker_advance_travel(actor,amount);}
