#include "actor_mission_init.h"
#define U8(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U16(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
__declspec(noinline) int32_t restore_actor_group_allocate(uint16_t kind,int32_t mode,int32_t orientation,uint16_t x,uint16_t y)
{
 uint32_t group,slot,offset;
 for(group=10,slot=0xbcbf68u;slot<0xbcd5e8u;++group,slot+=144u){
  if(U8(slot)!=0)continue;
  if(U8(slot+105u)!=0){U8(slot)=1;continue;}
  offset=group*144u;
  U16(0xbcb9feu+offset)=x;U16(0xbcba00u+offset)=y;
  U8(0xbcb9c8u+offset)=1;U8(0xbcba0au+offset)=0;U8(0xbcb9c9u+offset)=0;
  U16(0xbcb9ccu+offset)=kind;U8(0xbcb9cau+offset)=(uint8_t)(group-10u);
  if(mode==10)mode=(orientation==0||orientation==4)?1:2;
  U16(0xbcb9f2u+offset)=(uint16_t)mode;U16(0xbcb9f4u+offset)=100;
  return (int32_t)group;
 }
 return 0;
}
__declspec(noinline) int32_t FUN_004b6ff0(uint16_t kind,int32_t mode,int32_t orientation,uint16_t x,uint16_t y)
{return restore_actor_group_allocate(kind,mode,orientation,x,y);}
