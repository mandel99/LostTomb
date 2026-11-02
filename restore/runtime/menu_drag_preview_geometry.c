#include "preview_geometry.h"
#include "footprint.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
uint32_t FUN_00486bf0(int32_t x_in,int32_t y_in)
{
 uint32_t x=(uint32_t)x_in,y=(uint32_t)y_in,orientation=U(0xbca2e0u),bound,cell,building,table,index;
 if(orientation==6u)--y;
 else if(orientation==4u){--x;--y;}
 else if(orientation==2u)--x;
 if((int32_t)x<0)return 999;
 if((int32_t)y<0)return 999;
 bound=U(0xbca2d0u);if((int32_t)(x+1u)>=(int32_t)bound)return 999;
 bound=U(0xbca2d4u);if((int32_t)(y+1u)>=(int32_t)bound)return 999;
 cell=U(0xbca2d8u)+y*228u+x;building=(uint32_t)(int32_t)(int16_t)W(0xb4e6fcu+cell*2u);
 if(!building)return 0;
 if(W(0x7999e8u+building*264u)!=136u)return 0;
 for(table=0x5cc8c0u;table<0x5cc920u;table+=48u){
  for(index=0;index<2u;++index){
   uint32_t neighbor=U(table+index*8u)+cell;
   if((uint32_t)(int32_t)(int16_t)W(0xb4e6fcu+neighbor*2u)!=building)return 0;
  }
 }
 return 1;
}
uint32_t FUN_00486cf0(uint32_t cell)
{
 int32_t offset=(int32_t)(cell-U(0xbca2d8u));
 return FUN_00486bf0(offset%228,offset/228);
}
void FUN_004bfa50(int32_t x_in,int32_t y_in,uint32_t layout)
{
 uint32_t table=0x5df78cu+layout*16u,index;
 for(index=0;index<4u;++index){
  uint32_t record=U(table+index*4u)*16u,x,y,orientation,size;
  if(layout==0u){x=U(0x5df81cu+record)+(uint32_t)x_in;y=U(0x5df818u+record)+(uint32_t)y_in;}
  else if(layout==1u){x=U(0x5df818u+record)+(uint32_t)x_in;y=U(0x5df81cu+record)+(uint32_t)y_in;}
  else{x=(uint32_t)x_in;y=(uint32_t)y_in;}
  orientation=U(0xbca2e0u);
  if(orientation!=0u){
   size=U(0x5df814u+record);
   if(orientation==2u)x+=size-1u;
   else if(orientation==4u){x+=size-1u;y+=size-1u;}
   else y+=size-1u;
  }
  size=U(0x5df814u+record);
  FUN_0047d360((int32_t)size,(int32_t)x,(int32_t)y,UINT32_C(0xffffffbf),0,0);
 }
}
