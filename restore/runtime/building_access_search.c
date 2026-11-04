#include "building_access_search.h"
#include "building_footprint.h"
#include "event_publisher.h"
#include "god_building_effects.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include "menu_prepare_integer.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

static uint32_t region_rank(uint32_t city_offset,uint8_t color)
{
 uint32_t i;
 for(i=0;i<10u;++i)if(U(0xea5f1cu+city_offset+i*8u)==color)return i;
 return 11;
}

/* Whole481060: prefer a road-only candidate in the city's ordered region
   list. An unmatched color still has rank11. If none qualifies, choose the
   first smallest positive distance below100000, without a terrain test. */
uint32_t restore_building_find_access(int32_t x,int32_t y,uint32_t size)
{
 uint32_t cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
 uint32_t table=0x5cc9e0u+size*96u,i,delta,neighbor,flags,rank,best_rank=12,best=0;
 uint32_t off,color,index,distance,min_distance=100000,found=UINT32_MAX,out_x=(uint32_t)x,out_y=(uint32_t)y;
 for(i=0;i<24u;++i){
  delta=U(table+i*4u);if(!delta)break;neighbor=cell+delta;flags=U(0xaf598cu+neighbor*4u);
  if(!(flags&64u)||(flags&4u))continue;
  if(I(U(0xeed760u+neighbor*4u))<=0)continue;
  off=U(0xeaac28u)*0x49d8u;color=B(0xa76aecu+neighbor);rank=region_rank(off,(uint8_t)color);
  if(I(rank)<I(best_rank)){best_rank=rank;best=i;}
 }
 if(best_rank<12u){
  index=size*24u+best;
  U(0xd3b4e4u)=U(0x5cce98u+index*8u)+(uint32_t)x;
  U(0xd3b4e0u)=U(0x5cce9cu+index*8u)+(uint32_t)y;
  return U(0x5cc9e0u+index*4u)+cell+1u;
 }
 for(i=0;i<24u;++i){
  delta=U(table+i*4u);if(!delta)break;neighbor=cell+delta;distance=U(0xeed760u+neighbor*4u);
  if(I(distance)>0&&I(distance)<I(min_distance)){
   index=size*24u+i;out_x=U(0x5cce98u+index*8u)+(uint32_t)x;out_y=U(0x5cce9cu+index*8u)+(uint32_t)y;
   min_distance=distance;found=neighbor;
  }
 }
 if(I(found)<0)return 0;
 U(0xd3b4e4u)=out_x;U(0xd3b4e0u)=out_y;return found+1u;
}

/* Whole481210 scans three footprints at horizontal shifts0,3,6. Its ranked
   phase accepts terrain bits64+4 together. The fallback adds the winning
   shift again to published X and the return cell; retain that native quirk. */
uint32_t restore_building_find_wide_access(int32_t x,int32_t y,uint32_t size)
{
 uint32_t cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
 uint32_t table=0x5cc9e0u+size*96u,shift,i,delta,neighbor,rank,best_rank=12,best=0,best_shift=0;
 uint32_t color,off,index,distance,min_distance=100000,found=UINT32_MAX,out_x=(uint32_t)x,out_y=(uint32_t)y;
 for(shift=0;shift<=6u;shift+=3u)for(i=0;i<24u;++i){
  delta=U(table+i*4u);if(!delta)break;neighbor=cell+shift+delta;
  if(!(B(0xaf598cu+neighbor*4u)&64u)||I(U(0xeed760u+neighbor*4u))<=0)continue;
  color=B(0xa76aecu+neighbor);off=U(0xeaac28u)*0x49d8u;rank=region_rank(off,(uint8_t)color);
  if(I(rank)<I(best_rank)){best_rank=rank;best=i;best_shift=shift;}
 }
 if(best_rank<12u){
  index=size*24u+best;
  U(0xd3b4e4u)=U(0x5cce98u+index*8u)+best_shift+(uint32_t)x;
  U(0xd3b4e0u)=U(0x5cce9cu+index*8u)+(uint32_t)y;
  return U(0x5cc9e0u+index*4u)+best_shift+cell+1u;
 }
 best_shift=0;
 for(shift=0;shift<=6u;shift+=3u)for(i=0;i<24u;++i){
  delta=U(table+i*4u);if(!delta)break;neighbor=cell+shift+delta;distance=U(0xeed760u+neighbor*4u);
  if(I(distance)>0&&I(distance)<I(min_distance)){
   index=size*24u+i;out_x=U(0x5cce98u+index*8u)+shift+(uint32_t)x;out_y=U(0x5cce9cu+index*8u)+(uint32_t)y;
   min_distance=distance;found=neighbor;best_shift=shift;
  }
 }
 if(I(found)<0)return 0;
 U(0xd3b4e0u)=out_y;U(0xd3b4e4u)=out_x+best_shift;return found+best_shift+1u;
}

/* Whole487D00 publishes Y before X and returns a zero-based cell. */
uint32_t restore_building_fixed_entry(int32_t x,int32_t y)
{
 uint32_t out_x=(uint32_t)x+9u,out_y=(uint32_t)y+6u;
 U(0xd3b4e0u)=out_y;U(0xd3b4e4u)=out_x;
 return U(0xbca2d8u)+out_y*228u+out_x;
}

/* Whole4F30B0. Output pointers can alias each other or record fields. The
   native full-EAX result keeps the tile pointer (mode5) or grid base (other
   modes) in its high24 bits, with AL forced to1. */
uint32_t restore_building_special_entry(int32_t building,uint32_t *tile,int32_t *x,int32_t *y)
{
 uint32_t id=(uint32_t)building,record=id*264u,root=id-(uint32_t)H(0x7999eau+record),variant,offset,direction,value,base;
 uint32_t tx=(uint32_t)(uintptr_t)x,ty=(uint32_t)(uintptr_t)y,tt=(uint32_t)(uintptr_t)tile;
 if(U(0x5e4ef4u)==5u){
  variant=B(0x799a2cu+root*264u);offset=U(0x5f5498u+variant*128u);record=(root+offset)*264u;
  U(tx)=(uint32_t)H(0x7999deu+record);value=(uint32_t)H(0x7999e0u+record)+2u;U(ty)=value;
  value=value*228u+U(tx)+U(0xbca2d8u);U(tt)=value;return(tt&0xffffff00u)|1u;
 }
 variant=B(0x799a2cu+root*264u);offset=U(0x5f3878u+variant*960u);record=(root+offset)*264u;
 variant=B(0x799a2cu+record);offset=variant*144u+(uint32_t)H(0x7999eau+record);direction=U(0x5e4ef8u+offset*8u);
 U(tx)=(uint32_t)H(0x7999deu+record);value=(uint32_t)H(0x7999e0u+record);U(ty)=value;
 switch(direction){case 4:U(tx)=U(tx)-1u;break;case 5:U(ty)=value+2u;break;case 6:U(tx)=U(tx)+2u;break;case 7:U(ty)=value-1u;break;default:break;}
 value=U(ty)*228u;value+=U(tx);base=U(0xbca2d8u);U(tt)=value+base;return(base&0xffffff00u)|1u;
}

/* Whole458270: cached high-water count, signed WORD creation serial, first strict
   maximum among status1/3. Children can change the chosen record before each
   following reload. Width/height arguments consume unsigned BYTE size. */
void restore_building_remove_highest_serial(void)
{
 uint32_t count=U(0x78d790u),id,chosen=0,record,size,x,y;int32_t best=0,serial,kind;
 for(id=1;I(id)<=I(count);++id){
  record=id*264u;if(B(0x7999d8u+record)!=1u&&B(0x7999d8u+record)!=3u)continue;
  serial=H(0x7999eeu+record);if(serial>best){best=serial;chosen=id;}
 }
 if(!chosen)return;
 chosen=FUN_00475b50(chosen);record=chosen*264u;U(0xe950dcu)=1;
 FUN_004e0cf0(279,0,U(0x7999e4u+record),0,0);B(0x7999d8u+record)=4;kind=H(0x7999e8u+record);U(0xbc9694u)=0;
 if(kind==202)restore_building_access_remove_special(I(chosen));
 else{size=B(0x7999dbu+record);x=(uint32_t)H(0x7999deu+record);y=(uint32_t)H(0x7999e0u+record);FUN_0047c2b0(I(chosen),I(x),I(y),I(size),I(size));}
 kind=H(0x7999e8u+record);
 if(kind==31||kind==30||kind==33||kind==209)restore_god_building_0043fd00(I(chosen),kind);
 x=(uint32_t)H(0x7999deu+record);y=(uint32_t)H(0x7999e0u+record);size=B(0x7999dbu+record);
 restore_building_access_refresh_terrain(I(x),I(y),size);restore_god_building_004695d0(I(chosen),0);
 x=(uint32_t)H(0x7999deu+record);y=(uint32_t)H(0x7999e0u+record);size=B(0x7999dbu+record);
 FUN_00516f90(I(x-size),I(y-size),I(x+size),I(y+size),1);
 x=(uint32_t)H(0x7999deu+record);y=(uint32_t)H(0x7999e0u+record);size=B(0x7999dbu+record);
 FUN_005173a0(I(x-size),I(y-size),I(x+size),I(y+size));
}
