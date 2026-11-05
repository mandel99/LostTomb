#include "random_building_patrol.h"
#include "actor_callbacks_human.h"
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))

/* 4E7C90: AL is Boolean; rejected kinds retain the building offset above
   AL. An accepted kind instead retains the cell query's full upper EAX. */
uint32_t restore_building_patrol_eligibility_result(int32_t building)
{
 uint32_t offset=(uint32_t)building*264u,eax=offset;
 int32_t kind=H(0x7999e8u+offset),x,y;
 if((kind>=0x3c&&kind<=0x40)||kind==0x2f||kind==0xb7||kind==0xde||
    kind==0xe5||kind==0xea||kind==0xeb||kind==0xec){
  y=H(0x799a00u+offset);x=H(0x7999feu+offset);
  eax=FUN_0047fe60(x,y,1,6);
  if(eax)return(eax&0xffffff00u)|1u;
 }
 return eax&0xffffff00u;
}
