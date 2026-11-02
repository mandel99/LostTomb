#include "minimap_input.h"
#include "minimap.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
uint32_t FUN_0043ef10(int32_t x,int32_t y,int32_t width,int32_t height)
{
 uint32_t row,outer,end=(uint32_t)height+4u,pixel_y=(uint32_t)y-4u;
 uint32_t toggle=0,col,pixel_x,inner,value;
 FUN_0043e670(width,height);
 U(0x70e3d0u)=(uint32_t)y;row=U(0x70e3d8u)-4u;
 U(0x70e3d4u)=(uint32_t)x;
 U(0x70e3ccu)=(uint32_t)x+(uint32_t)width*2u;
 U(0x70e3c8u)=(uint32_t)y+(uint32_t)height;
 U(0x70e47cu)=row;
 outer=0xfffffffcu;
 if((int32_t)end<=-4)return 0;
 do{
  pixel_x=(uint32_t)x-(toggle?9u:8u);toggle^=1u;
  col=U(0x70e3dcu)-4u;U(0x70e480u)=col;
  if(width>-4){
   inner=0xfffffffcu;
   do{
    if((int32_t)col>=0&&(int32_t)col<231&&(int32_t)row>=0&&(int32_t)row<457&&U(0xe38e30u)==pixel_y){
     uint32_t mouse_x=U(0xe38e34u);
     if(mouse_x==pixel_x||mouse_x==pixel_x+1u){
      value=U(0x70e494u+(row*231u+col)*4u);U(0x70e488u)=value;
      return (int32_t)value<0x0fff0000?value:0;
     }
    }
    pixel_x+=2u;++col;++inner;U(0x70e480u)=col;
   }while((int32_t)inner<width);
  }
  ++pixel_y;++outer;++row;U(0x70e47cu)=row;
 }while((int32_t)outer<(int32_t)end);
 return 0;
}
