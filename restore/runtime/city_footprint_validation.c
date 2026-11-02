#include "city_footprint_validation.h"
#include "terrain_redraw.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
int32_t *restore_city_geometry_004ded50(int32_t output[13],int32_t index,int32_t rotation,int32_t orientation)
{
 uint32_t source=0x5e2ad8u+(uint32_t)index*52u,x,y,size;unsigned i;
 for(i=0;i<13;++i)U((uint32_t)(uintptr_t)output+i*4u)=U(source+i*4u);
 if(orientation!=0&&orientation!=4){
  if(rotation==4||rotation==6){
   x=U((uint32_t)(uintptr_t)output);y=U((uint32_t)(uintptr_t)output+4u);size=U((uint32_t)(uintptr_t)output+8u);
   U((uint32_t)(uintptr_t)output)=22u-x-size;U((uint32_t)(uintptr_t)output+4u)=8u-y-size;
  }
  x=U((uint32_t)(uintptr_t)output);y=U((uint32_t)(uintptr_t)output+4u);
  U((uint32_t)(uintptr_t)output)=y;U((uint32_t)(uintptr_t)output+4u)=x;U((uint32_t)(uintptr_t)output+16u)=y+x*228u;
 }else if(rotation==4||rotation==2){
  x=U((uint32_t)(uintptr_t)output);y=U((uint32_t)(uintptr_t)output+4u);size=U((uint32_t)(uintptr_t)output+8u);
  x=22u-x-size;y=8u-y-size;U((uint32_t)(uintptr_t)output)=x;U((uint32_t)(uintptr_t)output+4u)=y;
  U((uint32_t)(uintptr_t)output+16u)=x+y*228u;
 }
 return output;
}
uint8_t FUN_004def00(int32_t cell)
{
 int32_t relative=(int32_t)((uint32_t)cell-U(0xbca2d8u)),origin_x=relative%228,origin_y=relative/228,index,row,column,size;
 int32_t record[13];uint32_t x,y,offset,cell_origin,orientation;
 for(index=0;index<116;++index){
  orientation=U(0xbca2e0u);
  restore_city_geometry_004ded50(record,index,(int32_t)orientation,(int32_t)orientation);
  x=(uint32_t)record[0]+(uint32_t)origin_x;y=(uint32_t)record[1]+(uint32_t)origin_y;size=record[2];
  cell_origin=U(0xbca2d8u)+y*228u+x;U(0xbc9768u)=cell_origin;
  for(row=0;row<size;++row)for(column=0;column<size;++column){
   if((uint8_t)FUN_0046f9a0((int32_t)(U(0xbca2e8u)+(uint32_t)column+x),(int32_t)(U(0xbca2ecu)+(uint32_t)row+y),3))return 0;
   offset=U(0x5cc8c0u+((uint32_t)row*6u+(uint32_t)column)*8u);
   if(U(0xaf598cu+(offset+U(0xbc9768u))*4u)&0xfeffd76eu)return 0;
  }
 }
 return 1;
}
