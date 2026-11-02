#include "monument_footprint.h"
#include "placement_queries.h"
#include "road_place.h"
#include "road_variant.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
/* Exact ordered offset-table indices from native5468D0. Each row starts
   with its count. Offsets are DWORDs at5CC8C0+8*index, with6 columns. */
static const uint8_t shapes[8][4][16]={
 {{2,1,7},{2,6,7},{2,0,6},{2,0,1}},
 {{6,0,1,2,6,7,8},{6,1,2,7,8,13,14},{6,6,7,8,12,13,14},{6,0,1,6,7,12,13}},
 {{3,0,1,2},{3,2,8,14},{3,12,13,14},{3,0,6,12}},
 {{3,2,8,14},{3,12,13,14},{3,0,6,12},{3,0,1,2}},
 {{10,3,4,9,10,15,16,21,22,27,28},{10,18,19,20,21,22,24,25,26,27,28},
  {10,0,6,12,18,24,1,7,13,19,25},{10,0,1,2,3,4,6,7,8,9,10}},
 {{15,0,1,2,3,4,6,7,8,9,10,12,13,14,15,16},{15,2,8,14,20,26,3,9,15,21,27,4,10,16,22,28},
  {15,12,13,14,15,16,18,19,20,21,22,24,25,26,27,28},{15,0,6,12,18,24,1,7,13,19,25,2,8,14,20,26}},
 {{2,0,14},{2,12,14},{2,2,14},{2,0,2}},
 {{10,0,1,2,3,4,6,7,8,9,10},{10,3,9,15,21,27,4,10,16,22,28},
  {10,18,19,20,21,22,24,25,26,27,28},{10,0,6,12,18,24,1,7,13,19,25}}
};
static uint32_t tile_origin(uint32_t record)
{return U(0xbca2d8u)+(uint32_t)H(record+6u)+(uint32_t)H(record+8u)*228u;}
/* Whole native5468D0:3647 bytes,882 instructions. The virtual image query
   writes one DWORD flags value. Footprint size is the earlier cached BYTE
   from the building record, not an uninitialized decompiler local. */
void restore_monument_commit_footprint(uint32_t self,int32_t building)
{
 uint32_t record=0x7999d8u+(uint32_t)building*264u;
 uint32_t root=(uint32_t)building-(uint32_t)H(record+18u),size,base,cell,row,col,image,flags,shape,rotation,i;
 int32_t pattern;
 if(H(record+18u)==0)U(self+84u)=restore_placement_activity_00547ba0(self,building)&255u;
 size=B(record+3u);
 if(U(0x799a30u+root*264u)==1u&&U(0xe94e0cu)!=51u){
  base=tile_origin(record);
  if((uint32_t)H(record+18u)==U(self+80u)){
   cell=base+U(0x5cc8c0u);
   B(0xb285ccu+cell)&=0xe0u;
   image=(B(0xacf85cu+cell)&7u)+U(0xf56b54u);
   B(0xb67d1cu+cell)|=0x40u;U(0xb7482cu+cell*4u)=image;return;
  }
  for(row=0;row<size;++row)for(col=0;col<size;++col){
   cell=base+U(0x5cc8c0u+row*48u+col*8u);
   image=(uint32_t)FUN_00475fd0(cell);
   B(0xb285ccu+cell)&=0xe0u;image+=U(0xf572ecu);
   B(0xb67d1cu+cell)|=0x40u;U(0xb7482cu+cell*4u)=image;
  }
  return;
 }
 image=restore_placement_image_query(self,building,(uint32_t)(uintptr_t)&flags);
 if(flags&1u){
  base=tile_origin(record);
  for(row=0;row<size;++row)for(col=0;col<size;++col){
   cell=base+U(0x5cc8c0u+row*48u+col*8u);
   U(0xaf598cu+cell*4u)=8;W(0xb4e6fcu+cell*2u)=(uint16_t)building;
   B(0xb285ccu+cell)&=0xe0u;B(0xb67d1cu+cell)|=0x40u;
   if(image)U(0xb7482cu+cell*4u)=image==U(0xf56b54u)?image+(B(0xacf85cu+cell)&7u):image;
  }
  return;
 }
 FUN_0047bf50((uint16_t)building,H(record+6u),H(record+8u),(int32_t)size,(int32_t)size,image,0x40000000u);
 /* The rectangle provider may update position, shape or orientation. */
 base=tile_origin(record);shape=U(record+104u);
 if(shape==27u||shape==28u){U(0xaf598cu+(base+U(0x5cc8c0u))*4u)=8;return;}
 switch(shape){
 case 1:case 29:pattern=0;break;case 2:pattern=1;break;
 case 9:case 10:case 13:case 14:pattern=2;break;case 11:pattern=3;break;
 case 19:pattern=4;break;case 22:pattern=5;break;case 24:pattern=6;break;
 case 26:pattern=7;break;default:return;
 }
 rotation=U(record+144u);if(rotation>3u)return;
 for(i=1;i<=(uint32_t)shapes[pattern][rotation][0];++i){
  cell=base+U(0x5cc8c0u+(uint32_t)shapes[pattern][rotation][i]*8u);
  U(0xaf598cu+cell*4u)=8;
 }
}
void FUN_005468d0(uint32_t self,int32_t building){restore_monument_commit_footprint(self,building);}
