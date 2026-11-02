#include "flood_simulation.h"
#include "building_footprint.h"
#include "footprint.h"
#include "road_place.h"
#include "map_commit_fields.h"

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0040f8f0(void *destination,uint32_t bytes,uint8_t value);
extern void FUN_00455540(uintptr_t cells,uint32_t count,uint32_t mask);

/* 4BEB90/4BECC0 receive ECX in the original. Ordinary C callers pass it
 * explicitly. List nodes hold the tile object at +12, not the cell itself. */
void FUN_004becc0(uintptr_t tile)
{
 uint32_t cell=U(tile+8u);
 if((U(0xaf598cu+cell*4u)&0x100u)&&!B(0xae8e7cu+cell))
  FUN_00468590((int32_t)cell);
}

static void irrigate_list(uintptr_t bucket)
{
 uint32_t node,tile;
 if(!U(bucket))return;
 node=U(bucket+16u);
 while(node){
  tile=U(node+12u);node=U(node);
  if(!tile)break;
  FUN_004becc0(tile);
 }
}

void FUN_004beb90(uintptr_t receiver)
{
 uint32_t phase=U(receiver+0x32facu);
 if((int32_t)phase<30)irrigate_list(receiver+0x32c64u+phase*28u);
 else irrigate_list(receiver+0x32f90u);
 /* Propagation may change state; the original reloads the phase each time. */
 phase=U(receiver+0x32facu);
 if((int32_t)(phase+1u)<30)irrigate_list(receiver+0x32c80u+phase*28u);
 phase=U(receiver+0x32facu);
 if((int32_t)(phase+2u)<30)irrigate_list(receiver+0x32c9cu+phase*28u);
}

/* 468590: native bounded depth-first walk with a 1000-entry overflow queue.
 * Terrain/image/queue entries are DWORDs; visited/markers are BYTEs;
 * building IDs and kinds are signed WORDs. Keep each native stride. */
void FUN_00468590(int32_t input)
{
 uint32_t cell=(uint32_t)input,threshold,iteration,offset,next,neighbor;
 int32_t building,pending;
 if(!(U(0xaf598cu+cell*4u)&0x100u))return;
 threshold=U(0xf56b84u)+48u;
 FUN_0040f8f0((void *)(uintptr_t)0x78c414u,4000,0);
 U(0x78c410u)=0;U(0x78c40cu)=0;
 for(iteration=1;iteration<=0xcb10u;++iteration){
  if(U(0xaf598cu+cell*4u)&4u)continue;
  building=H(0xb4e6fcu+cell*2u);
  if((!building||W(0x7999e8u+(uint32_t)building*264u)==0x8au)&&
     (int32_t)U(0xb7482cu+cell*4u)>=(int32_t)threshold&&
     !(B(0xb285ccu+cell)&0x80u))
   U(0xb7482cu+cell*4u)-=48u;
  B(0xae8e7cu+cell)=1;
  pending=-1;
  for(offset=0;offset<16u;offset+=4u){
   neighbor=cell+U(0x5cd678u+offset);
   building=H(0xb4e6fcu+neighbor*2u);
   if(building&&W(0x7999e8u+(uint32_t)building*264u)==0x5au){
    next=0x799a0du+(uint32_t)building*264u;
    if(!B(next)){B(next)=2;FUN_00487ec0(building);}
   }else if((U(0xaf598cu+neighbor*4u)&0x100u)&&!B(0xae8e7cu+neighbor)){
    uint32_t row,col,mark=neighbor-458u;
    for(row=0;row<5u;++row){
     for(col=0;col<5u;++col)B(0xaf598cu+(mark+col)*4u+3u)|=1u;
     mark+=228u;
    }
    U(0xbc9768u)=mark;
    if(pending==-1)pending=(int32_t)neighbor;
    else{
     next=U(0x78c40cu);U(0x78c414u+next*4u)=neighbor;
     ++next;U(0x78c40cu)=(int32_t)next<1000?next:0;
    }
   }
  }
  if(pending==-1){
   next=U(0x78c410u);
   if(next==U(0x78c40cu))return;
   pending=(int32_t)U(0x78c414u+next*4u);
   ++next;U(0x78c410u)=(int32_t)next<1000?next:0;
  }
  cell=(uint32_t)pending;
  if(pending<0)return;
 }
}

/* 468790: clear only eligible irrigation cells in the active map rectangle. */
void FUN_00468790(void)
{
 uint32_t cell=U(0xbca2d8u),threshold=U(0xf56b84u)+48u;
 int32_t rows=(int32_t)U(0xbca2d4u),width=(int32_t)U(0xbca2d0u),col,building;
 while(rows>0){
  for(col=0;col<width;++col,++cell){
   uint32_t terrain=U(0xaf598cu+cell*4u);
   if((terrain&4u)||!(terrain&0x100u))continue;
   building=H(0xb4e6fcu+cell*2u);
   if(building&&W(0x7999e8u+(uint32_t)building*264u)!=0x8au)continue;
   B(0xae8e7cu+cell)=0;
   if(!(B(0xb285ccu+cell)&0x80u)&&!building&&
      (int32_t)U(0xb7482cu+cell*4u)<(int32_t)threshold)
    U(0xb7482cu+cell*4u)+=48u;
  }
  cell+=U(0xbca2dcu);--rows;
 }
}

/* 4555D0: rebuild water-lift propagation and well service. Keep the native
 * work-list order and repeated passes: activating one lift may enable another. */
void FUN_004555d0(void)
{
 uint32_t id,record,index,limit,cell,orientation,image,kind;
 uint8_t before,changed;
 int32_t x,y;
 FUN_00455540(0xaf598cu,0xcb10u,0xfeffdfffu);
 FUN_00468790();
 FUN_0040f8f0((void *)(uintptr_t)0x78e178u,8000,0);
 U(0x78d840u)=0;U(0x78d83cu)=0;
 for(id=1;(int32_t)id<=(int32_t)U(0x78d790u);++id){
  record=id*264u;
  if(B(0x7999d8u+record)!=1u||W(0x7999e8u+record)!=0x5au)continue;
  index=U(0x78d83cu);
  W(0x78e178u+index*2u)=(uint16_t)id;U(0x78d83cu)=index+1u;
  before=B(0x799a0du+record);
  B(0x799a0du+record)=!U(0x78d7b0u)&&FUN_00487e60((int32_t)id)?2u:0u;
  if(B(0x799a0du+record)!=before)FUN_00487ec0((int32_t)id);
 }
 FUN_004beb90((uintptr_t)RESTORE_CITY_WORLD);
 limit=U(0x78d83cu);
 do{
  changed=0;U(0x78d840u)=0;
  if((int32_t)limit<1)break;
  do{
   index=U(0x78d840u);id=(uint32_t)(int32_t)H(0x78e178u+index*2u);
   U(0x78d840u)=index+1u;record=id*264u;
   if(B(0x799a0du+record)==2u&&H(0x799a18u+record)>0){
    B(0x799a0du+record)=1;changed=1;
    (void)FUN_0047cc70(H(0x7999deu+record),H(0x7999e0u+record),0,1);
    orientation=B(0x799a82u+record);U(0xbc96dcu)=orientation;
    cell=U(0x7999e4u+record);
    FUN_00468590((int32_t)(cell+U(0x5cd6c8u+(orientation^2u)*4u)));
    orientation=U(0xbc96dcu);
    cell=U(0x7999e4u+record);
    FUN_00468590((int32_t)(cell+U(0x5cd6d8u+(orientation^2u)*4u)));
    limit=U(0x78d83cu);
   }
  }while((int32_t)U(0x78d840u)<(int32_t)limit);
 }while(changed);
 U(0x78d840u)=0;index=0;
 if((int32_t)limit>0)do{
  U(0x78d840u)=index+1u;
  id=(uint32_t)(int32_t)H(0x78e178u+index*2u);record=id*264u;
  if(H(0x799a18u+record)>0&&B(0x799a0du+record)){
   FUN_004824b0(H(0x7999deu+record),H(0x7999e0u+record),2,2,0x1000000u);
   limit=U(0x78d83cu);
  }
  index=U(0x78d840u);
 }while((int32_t)index<(int32_t)limit);
 for(id=1;(int32_t)id<=(int32_t)U(0x78d790u);++id){
  record=id*264u;kind=W(0x7999e8u+record);
  if(B(0x7999d8u+record)!=1u||(kind!=0x5cu&&kind!=0xb4u))continue;
  cell=U(0x7999e4u+record);
  image=U(kind==0x5cu?0xf564d4u:0xf56644u);
  if((int8_t)B(0xac2d4cu+cell)>=30)image+=2u;
  x=H(0x7999deu+record);y=H(0x7999e0u+record);
  if(kind==0x5cu){
   FUN_0047bf50((uint16_t)id,x,y,1,1,image,8);
   FUN_004824b0(H(0x7999deu+record),H(0x7999e0u+record),1,2,0x2000u);
  }else FUN_0047bf50((uint16_t)id,x,y,2,2,image,8);
  B(0x799a0du+record)=1;
 }
}
