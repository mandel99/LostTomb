#include "map_commit_state.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
void FUN_00485200(void)
{
 uint32_t cursor=U(0xbc9728u);
 U(0xbc968cu)=1;U(0xbc9688u)=500;U(0xbc9684u)=cursor;B(0xe38dcau)=1;
}
void FUN_00485830(void)
{
 uint32_t height=U(0xbca2d4u),row,first,last,cell,count,image;
 if((int32_t)height<=0)return;
 for(row=0;(int32_t)row<(int32_t)height;++row){
  first=U(0xbc9f24u+row*4u);last=U(0xbc9b94u+row*4u);cell=U(0xbc9804u+row*4u);
  if((int32_t)first<(int32_t)last){
   count=last-first;
   do{
    if(!H(0xb4e6fcu+cell*2u)){image=U(0xa1126cu+cell*4u);U(0xb7482cu+cell*4u)=image;}
    ++cell;--count;
   }while(count);
  }
 }
}
void FUN_004e9680(int32_t player,int32_t amount)
{
 uint32_t offset=(uint32_t)player*0x49d8u,value;
 if(B(0xea1872u+offset)==1&&U(0x605a3cu)!=1){
  value=U(0xea187cu+offset);U(0xea187cu+offset)=value-(uint32_t)amount;
  value=U(0xea44fcu+offset);U(0xea44fcu+offset)=value+(uint32_t)amount;
 }
}
