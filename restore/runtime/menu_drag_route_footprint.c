#include "route_footprint.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
void FUN_0047ca50(uint32_t mode,int32_t x_in,int32_t y_in,int32_t width_in,int32_t height_in)
{
 uint32_t x=(uint32_t)x_in,y=(uint32_t)y_in,width=(uint32_t)width_in,height=(uint32_t)height_in,bound,cell,row,column,offset;
 if(x_in<0)return;if(y_in<0)return;
 bound=U(0xbca2d0u);if((int32_t)(x+width-1u)>=(int32_t)bound)return;
 bound=U(0xbca2d4u);if((int32_t)(y+height-1u)>=(int32_t)bound)return;
 cell=U(0xbca2d8u)+y*228u+x;U(0xbc9768u)=cell;
 if(height_in<=0)return;
 for(row=0;row<height;++row){
  if(width_in<=0)continue;
  for(column=0;column<width;++column){
   if(mode!=0)continue;
   offset=U(0x5cc8c0u+(row*6u+column)*8u);
   U(0xeed760u+(cell+offset)*4u)=0;
  }
 }
}
uint32_t FUN_0047f190(uint32_t cell)
{
 uint32_t building=(uint32_t)(int32_t)(int16_t)W(0xb4e6fcu+cell*2u),table,index,offset;
 if(!building)return 0;
 if(W(0x7999e8u+building*264u)!=90u)return 0;
 for(table=0x5cc8c0u;table<0x5cc920u;table+=48u){
  for(index=0;index<2u;++index){
   offset=U(table+index*8u);
   if((uint32_t)(int32_t)(int16_t)W(0xb4e6fcu+(cell+offset)*2u)!=building)return 0;
  }
 }
 return 1;
}
