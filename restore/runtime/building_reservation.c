#include "building_reservation.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define S(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0040f8f0(uint32_t *,uint32_t,uint8_t);
void FUN_00485080(void)
{
 int32_t count=(int32_t)U(0xbc95f0u),i;
 for(i=0;i<count;++i){
  int32_t id=S(0xba746cu+(uint32_t)i*2u);
  if(id){uint32_t offset=(uint32_t)id*264u;
   if(B(0x7999d8u+offset)==6)B(0x7999d8u+offset)=1;
   B(0x799a97u+offset)=0;
  }
 }
 U(0xbc95f0u)=0;
 FUN_0040f8f0((uint32_t *)(uintptr_t)0xba7888u,0x21d68,0);
 FUN_0040f8f0((uint32_t *)(uintptr_t)0xba746cu,0x41a,0);
}
uint32_t FUN_004851b0(int32_t building)
{
 uint32_t p;
 if(building<=0||U(0xbc968cu)!=1||U(0xbc9694u)!=1||(int32_t)U(0xbc95f0u)<=0)return 0;
 /* The positive count is a gate: native scans all525 WORD slots, not count. */
 for(p=0xba746cu;p<0xba7886u;p+=2u)if((int32_t)S(p)==building)return 1;
 return 0;
}
