#include "building_footprint.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_0047a1c0(int32_t x);
extern void FUN_0047a220(int32_t x,int32_t y,int32_t set);
static int32_t add(int32_t a,uint32_t b){return (int32_t)((uint32_t)a+b);}

/* Complete 47CF00/47D1F0. The matrix has six columns of eight-byte entries;
 * the stored tile offset is a DWORD and the footprint marker is one byte. */
static void footprint(uint32_t building,int32_t x,int32_t y,uint32_t image,uint32_t size)
{
 uint32_t rotation=U(0xe94e04u),last=size-1u,corner_x,corner_y,base,row,col,entry,cell,terrain;uint8_t marker;
 if(rotation==0){corner_x=0;corner_y=last;}
 else if(rotation==2){corner_x=0;corner_y=0;}
 else if(rotation==4){corner_x=last;corner_y=0;}
 else{corner_x=last;corner_y=last;}
 if(x<0||y<0||add(x,last)>=(int32_t)U(0xbca2d0u)||add(y,last)>=(int32_t)U(0xbca2d4u)){
  U(0xbc9734u)=1;return;
 }
 base=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;U(0xbc9768u)=base;
 for(row=0;row<size;++row)for(col=0;col<size;++col){
  entry=0x5cc8c4u+row*48u+col*8u;cell=U(entry-4u)+base;
  terrain=U(0xaf598cu+cell*4u)|8u;U(0xaf598cu+cell*4u)=terrain;
  if(!(terrain&4u))U(0xaf598cu+cell*4u)=(terrain&0x03052e80u)|8u;
  W(0xb4e6fcu+cell*2u)=(uint16_t)building;
  marker=B(0xb285ccu+cell);marker=size==2?(uint8_t)((marker&0xe1u)|1u):(uint8_t)((marker&0xefu)|2u);
  B(0xb285ccu+cell)=marker;U(0xb7482cu+cell*4u)=image;
  marker=B(entry);B(0xb67d1cu+cell)=marker;
  if(col==corner_x&&row==corner_y)B(0xb67d1cu+cell)=(uint8_t)(marker|0x40u);
 }
}
void FUN_0047cf00(uint32_t building,int32_t x,int32_t y,uint32_t image)
{footprint(building,x,y,image,2);}
void FUN_0047d1f0(uint32_t building,int32_t x,int32_t y,uint32_t image)
{footprint(building,x,y,image,3);}

/* Complete 47C2B0. The first DWORD is an identifier, not an int pointer. */
void FUN_0047c2b0(int32_t building,int32_t x,int32_t y,int32_t width,int32_t height)
{
 uint32_t base,row,col,cell,terrain,record,image;
 if(x<0||y<0||add(x,(uint32_t)width-1u)>=(int32_t)U(0xbca2d0u)||
                add(y,(uint32_t)height-1u)>=(int32_t)U(0xbca2d4u)){
  U(0xbc9734u)=1;return;
 }
 base=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
 for(row=0;(int32_t)row<height;++row)for(col=0;(int32_t)col<width;++col){
  cell=U(0x5cc8c0u+row*48u+col*8u)+base;
  if((int32_t)H(0xb4e6fcu+cell*2u)!=building)continue;
  if(building){
   record=0x7999e8u+(uint32_t)building*264u;
   if(W(record)!=99u)B(0xa9010cu+cell)=B(record);
  }
  image=B(0xb285ccu+cell)&0xe0u;B(0xae8e7cu+cell)=0;B(0xb285ccu+cell)=(uint8_t)image;
  terrain=U(0xaf598cu+cell*4u);W(0xb4e6fcu+cell*2u)=0;W(0xa9cc1cu+cell*2u)=0;
  B(0xadc36cu+cell)=0;B(0xb67d1cu+cell)=0x40;
  if(terrain&4u){
   U(0xaf598cu+cell*4u)=4;FUN_0047a220(add(x,col),add(y,row),1);
  }else{
   U(0xaf598cu+cell*4u)=(terrain&0x03052e80u)|0x1000u;
   image=B(0xacf85cu+cell)&7u;image+=U(0xf56c4cu);U(0xb7482cu+cell*4u)=image;
  }
 }
}

uint32_t FUN_00487dc0(int32_t x,int32_t y)
{
 uint32_t cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x,object;
 object=U(0xd05720u+cell*4u);if(object&&!B(object+24u))return 1;
 object=U(0xd05724u+cell*4u);if(object&&!B(object+24u))return 1;
 object=U(0xd05ab0u+cell*4u);if(object&&!B(object+24u))return 1;
 object=U(0xd05ab4u+cell*4u);if(object&&!B(object+24u))return 1;
 return 0;
}
uint32_t FUN_00487e30(int32_t building)
{
 uint32_t offset=(uint32_t)building*264u;int32_t y=H(0x7999e0u+offset),x=H(0x7999deu+offset);
 return FUN_00487dc0(x,y);
}
uint32_t FUN_00487d30(int32_t orientation,uint32_t water)
{
 uint32_t offset=0,address;
 if(B(0xbca32cu))offset=B(0xbca32du)?8u:4u;
 if((uint8_t)water){
  if(orientation==0)address=0xf57284u;else if(orientation==1)address=0xf5728cu;
  else if(orientation==2)address=0xf57294u;else address=0xf5729cu;
 }else{
  if(orientation==0)address=0xf56cccu;else if(orientation==1)address=0xf56c7cu;
  else if(orientation==2)address=0xf56c74u;else address=0xf56c6cu;
 }
 return offset+U(address);
}
void FUN_00487ec0(int32_t building)
{
 uint32_t offset=(uint32_t)building*264u,water,image;int32_t angle,direction,x,unused_y;
 B(0xbca32cu)=(uint8_t)FUN_00487e30(building);
 B(0xbca32du)=(uint8_t)(B(0x799a0du+offset)!=0);
 angle=(int32_t)U(0xbca2e0u);direction=add((int32_t)B(0x799a82u+offset),0u-(uint32_t)(angle/2));
 if(direction<0)direction=add(direction,4);
 unused_y=H(0x7999e0u+offset);x=H(0x7999deu+offset);(void)unused_y;
 water=FUN_0047a1c0(x);image=FUN_00487d30(direction,(uint8_t)water);
 unused_y=H(0x7999e0u+offset);x=H(0x7999deu+offset);
 FUN_0047cf00((uint32_t)building,x,unused_y,image);
}

/* 487E60: inspect both native intake offsets for the building orientation. */
uint32_t FUN_00487e60(int32_t building)
{
 uint32_t record=(uint32_t)building*264u,orientation=B(0x799a82u+record);
 uint32_t cell=U(0x7999e4u+record),neighbor=cell+U(0x5cd6c8u+orientation*4u);
 if(B(0xaf598cu+neighbor*4u)&4u)return 1;
 neighbor=cell+U(0x5cd6d8u+orientation*4u);
 return (B(0xaf598cu+neighbor*4u)&4u)?1u:0u;
}
