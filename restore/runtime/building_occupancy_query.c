#include "building_occupancy_query.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
uint32_t restore_region_has_blocking_tile(int32_t x,int32_t y,int32_t width,int32_t height,uint32_t mask)
{
 uint32_t origin=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x,row,column,at,tile,object;
 U(0xbc9768u)=origin;
 if(height>0&&width>0)for(row=0;(int32_t)row<height;++row){
  at=0x5cc8c0u+row*48u;
  for(column=0;(int32_t)column<width;++column,at+=8u){
   tile=origin+U(at);
   if(mask&0x10000u){object=U(0xd05720u+tile*4u);if(object&&!B(object+24u))return(tile&0xffffff00u)|1u;}
   if(U(0xaf598cu+tile*4u)&mask)return(tile&0xffffff00u)|1u;
  }
 }
 return (uint32_t)height&0xffffff00u;
}
uint32_t restore_region_has_active_occupant(int32_t x,int32_t y,int32_t width,int32_t height)
{return restore_region_has_blocking_tile(x,y,width,height,0x10000);}
uint32_t restore_building_has_active_occupant(int32_t building)
{
 uint32_t record=(uint32_t)building*264u;int32_t y=(int32_t)(int16_t)W(0x7999e0u+record),x=(int32_t)(int16_t)W(0x7999deu+record);
 return restore_region_has_active_occupant(x,y,3,3);
}
