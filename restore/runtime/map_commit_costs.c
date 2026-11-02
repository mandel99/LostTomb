#include "map_commit_costs.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0040f8f0(uint32_t *,uint32_t,uint8_t);
void FUN_005173a0(int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
 uint32_t cell,tail,raw,flags,record;int32_t width,height,x,y;uint16_t type;uint8_t value;
 if(x1<0)x1=0;if(y1<0)y1=0;
 width=(int32_t)U(0xbca2d0u);if(x2>=width)x2=(int32_t)((uint32_t)width-1u);
 height=(int32_t)U(0xbca2d4u);if(y2>=height)y2=(int32_t)((uint32_t)height-1u);
 tail=(uint32_t)width-(uint32_t)x2+(uint32_t)x1-1u;
 cell=U(0xbca2d8u)+(uint32_t)y1*228u+(uint32_t)x1;
 for(y=y1;y<=y2;++y){
  for(x=x1;x<=x2;++x,++cell){
   raw=U(0xaf598cu+cell*4u);flags=raw;
   if((flags&0x80000u)||(flags&0x10004u)==0x10004u)value=0xff;
   else{
    flags&=0xeefed77fu;
    if(flags&0x8000u)value=4;
    else if((flags&0x40u)||(flags&0x40000000u))value=0;
    else if(flags&0x20u)value=0;
    else if(raw&0x400u)value=2;
    else if(flags&8u){
     record=(uint32_t)(int32_t)H(0xb4e6fcu+cell*2u)*264u;
     type=(uint16_t)H(0x7999e8u+record);
     if(type==0x48)value=1;
     else if(type==0xca)value=4;
     else if(type==0x63||type==0x58||type==0x59||type==0x5d||type==0xb7||type==0xd2||type==0xcf||type==0xd0||type==0xde||type==0xd5||type==0xe1||type==0xe6||type==0xe3||type==0xe5||type==0xea||type==0xeb||type==0xec||type==0xe4||type==0xd7)value=0xff;
     else if(type==0x39)value=5;
     else if(type==0x36)value=0;
     else if(type==0x88)value=0xff;
     else value=1;
    }
    else if(flags&0x100u)value=0;
    else if(flags&0x4000u)value=3;
    else if(flags&0x12040011u)value=0;
    else if(flags&0x1000u)value=0;
    else if(!flags)value=0;
    else value=0xff;
   }
   B(0xec7630u+cell)=value;
  }
  cell+=U(0xbca2dcu)+tail;
 }
}
void FUN_00517660(void)
{
 int32_t last_x,last_y;
 FUN_0040f8f0((uint32_t *)0xec7630u,0xcb10u,0xff);
 last_y=(int32_t)(U(0xbca2d4u)-1u);last_x=(int32_t)(U(0xbca2d0u)-1u);
 FUN_005173a0(0,0,last_x,last_y);
}
void FUN_005178e0(void)
{
 uint32_t cell=U(0xbca2d8u),rows,width,padding,direction,flags,count,x;uint8_t value;
 FUN_0040f8f0((uint32_t *)0xebab20u,0xcb10u,0xff);
 rows=U(0xbca2d4u);if((int32_t)rows<=0)return;
 width=U(0xbca2d0u);direction=U(0xe94e04u);
 do{
  if((int32_t)width>0){
   x=width;
   do{
    flags=U(0xaf598cu+cell*4u)&0xeefed77fu;value=0xff;
    if(flags&0x4000u){
     count=0;
     if(direction==0){
      if(U(0xaf598cu+(cell+228u)*4u)&0xc000u)count=1;
      if(U(0xaf598cu+(cell+229u)*4u)&0xc000u)++count;
      if(U(0xaf598cu+(cell+1u)*4u)&0xc000u)++count;
     }else if(direction==2){
      if(U(0xaf598cu+(cell+228u)*4u)&0xc000u)count=1;
      if(U(0xaf598cu+(cell+227u)*4u)&0xc000u)++count;
      if(U(0xaf598cu+(cell-1u)*4u)&0xc000u)++count;
     }else if(direction==4){
      if(U(0xaf598cu+(cell-228u)*4u)&0xc000u)count=1;
      if(U(0xaf598cu+(cell-229u)*4u)&0xc000u)++count;
      if(U(0xaf598cu+(cell-1u)*4u)&0xc000u)++count;
     }else if(direction==6){
      if(U(0xaf598cu+(cell-228u)*4u)&0xc000u)count=1;
      if(U(0xaf598cu+(cell-227u)*4u)&0xc000u)++count;
      if(U(0xaf598cu+(cell+1u)*4u)&0xc000u)++count;
     }
     if(count==3)value=0;
    }else if(flags&0x8000u)value=0;
    B(0xebab20u+cell)=value;
    ++cell;--x;
   }while(x);
  }
  padding=U(0xbca2dcu);cell+=padding;--rows;
 }while(rows);
}
