#include "building_removal.h"
#include "actor_create.h"
#include "actor_callbacks_human.h"
#include "building_footprint.h"
#include "clear_helpers.h"
#include "god_building_effects.h"
#include "map_commit_connect.h"
#include "map_refresh_postpass1.h"
#include "map_refresh_postpass3.h"
#include "mission_unlock.h"
#include "road_paint_model.h"
#include "surface_passes.h"
#include "terrain_redraw.h"
#include "water_refresh.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))
extern void FUN_00475cc0(int32_t,int32_t,int32_t,int32_t,uint32_t);

/* Whole4C03D0: only one subtraction of8 is made from the signed combined
   orientation. The four-entry layout is cached; the record orientation and
   coordinates are reloaded after each footprint child. */
void restore_building_clear_four_parts(int32_t building)
{
 uint32_t record=(uint32_t)building*264u,angle=B(0x799a82u+record)+U(0xbca2e0u),table,index,part,x,y,width;uint8_t orientation;
 if(I(angle)>=8)angle-=8u;table=0x5df78cu+angle*16u;
 for(index=0;index<4u;++index){
  part=U(table+index*4u);orientation=B(0x799a82u+record);x=(uint32_t)H(0x7999deu+record);y=(uint32_t)H(0x7999e0u+record);
  if(orientation==0u){x+=U(0x5df81cu+part*16u);y+=U(0x5df818u+part*16u);}
  else if(orientation==1u){x+=U(0x5df818u+part*16u);y+=U(0x5df81cu+part*16u);}
  width=U(0x5df814u+part*16u);FUN_0047c2b0(building,I(x),I(y),I(width),I(width));
 }
}

/* Whole4695D0: independent backward/forward signed-WORD chains, each
   limited to525 successful links. The starting building is not processed. */
void restore_building_clear_linked_parts(int32_t building,int32_t destroy)
{
 uint32_t side,step,current,record,size;int32_t next;
 for(side=0;side<2u;++side){
  current=(uint32_t)building;
  for(step=0;step<525u;++step){
   next=H(0x799a10u+current*264u+side*2u);if(next<=0)break;current=(uint32_t)next;
   if(destroy)restore_god_building_0046d100(next,0);
   else{
    record=current*264u;size=B(0x7999dbu+record);
    FUN_0047c2b0(next,H(0x7999deu+record),H(0x7999e0u+record),I(size),I(size));B(0x7999d8u+record)=4;
   }
  }
 }
}

/* Whole43FD00. The cleared tile origin is the record's stored grid cell,
   not its X/Y fields. Signed division by228 truncates toward zero. Native
   rectangle refreshes include the extra far-edge row/column. */
void restore_building_clear_farm_tiles(int32_t building,int32_t kind)
{
 uint32_t record=(uint32_t)building*264u,size,cell,row,col,flags,start_x,start_y,x,y,end_x,end_y;int32_t delta;uint8_t marker;
 if(W(0x7999e8u+record)==209u)W(0xea6084u+U(0xeaac28u)*0x49d8u)=0;
 switch(kind){case 30:size=3;break;case 31:size=2;break;case 33:size=4;break;case 209:size=5;break;default:size=0;break;}
 cell=U(0x799aa8u+record);delta=I(cell-U(0xbca2d8u));start_x=(uint32_t)(delta%228);start_y=(uint32_t)(delta/228);
 for(row=0;row<size;++row){
  x=start_x;y=start_y+row;
  for(col=0;col<size;++col){
   marker=B(0xb285ccu+cell);B(0xadc36cu+cell)=0;B(0xae8e7cu+cell)=0;B(0xb285ccu+cell)=(uint8_t)(marker&0xe0u);
   marker=B(0xb67d1cu+cell);W(0xb4e6fcu+cell*2u)=0;W(0xa9cc1cu+cell*2u)=0;B(0xb67d1cu+cell)=(uint8_t)((marker&0xc0u)|0x40u);
   flags=U(0xaf598cu+cell*4u)&0x03052ec4u;U(0xb7482cu+cell*4u)=0;U(0xaf598cu+cell*4u)=flags;B(0xa9010cu+cell)=(uint8_t)kind;
   if(flags&64u)FUN_0047b310(I(x),I(y),1);else if(U(0x77567cu))FUN_0047a120(I(x),I(y));
   ++x;++cell;
  }
  cell+=228u-size;
 }
 end_x=start_x+size;end_y=start_y+size;U(0x77567cu)=0;
 FUN_004767d0(I(start_x),I(start_y),I(end_x),I(end_y),0,0);
 FUN_004767d0(I(start_x-5u),I(start_y-5u),I(end_x+5u),I(end_y+5u),1,0);
 FUN_00477500(I(start_x),I(start_y),I(end_x),I(end_y));FUN_004777c0(I(start_x),I(start_y),I(end_x),I(end_y));
 FUN_00475cc0(I(start_x),I(start_y),I(end_x),I(end_y),0);FUN_004776f0(I(start_x),I(start_y),I(end_x),I(end_y));
 FUN_00477890(I(start_x),I(start_y),I(end_x),I(end_y));FUN_00475720(I(start_x),I(start_y),I(end_x),I(end_y),1);
 FUN_00516f90(I(start_x),I(start_y),I(end_x),I(end_y),1);
}

/* Whole48C0E0: four actors for low-BYTE zero, sixteen and sound17 otherwise.
   Coordinate offsets and actor WORD arithmetic retain native wrapping. */
void restore_building_spawn_removal_effects(int32_t x,int32_t y,uint32_t size,uint32_t expanded)
{
 uint32_t offset=U(0x5dc5fcu+size*4u),pixel=U(0x5dc614u+size*4u),spawn_x=(uint32_t)x+offset,spawn_y=(uint32_t)y+offset;
 uint32_t count=(uint8_t)expanded?16u:4u,index,actor,record,pixel_x,pixel_y;uint16_t dx,dy;uint8_t direction;
 for(index=0;index<count;++index){
  actor=(uint32_t)FUN_004b4800((uint8_t)U(0xeaac28u),6,I(spawn_x),I(spawn_y),0);if(!actor)continue;record=actor*388u;
  dx=W(0x5dc62cu+index*8u);dy=W(0x5dc630u+index*8u);
  W(0xbcd642u+record)=(uint16_t)(W(0xbcd642u+record)+pixel);W(0xbcd644u+record)=(uint16_t)(W(0xbcd644u+record)+pixel);
  W(0xbcd61cu+record)=(uint16_t)(W(0xbcd61cu+record)+dx);W(0xbcd61eu+record)=(uint16_t)(W(0xbcd61eu+record)+dy);
  pixel_y=(uint32_t)H(0xbcd61eu+record)*15u+pixel;pixel_x=(uint32_t)H(0xbcd61cu+record)*15u+pixel;
  FUN_004b4220(I(actor),H(0xbcd642u+record),H(0xbcd644u+record),I(pixel_x),I(pixel_y),0);
  direction=B(0x5dc6acu+index*4u);B(0xbcd651u+record)=direction;U(0xbcd754u+record)=direction<3u;
 }
 if((uint8_t)expanded)FUN_0053b090(17);
}
