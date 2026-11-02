#include "menu_record_queries.h"
#define LAYOUT_U(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define LAYOUT_W(a) (*(volatile uint16_t *)(uintptr_t)(a))
void FUN_004e2ab0(void)
{
 uint32_t source=0x5e44a8u,destination=0xe94e2cu;
 int32_t x=(int32_t)(LAYOUT_U(0xe38db0u)-180u)/2;
 LAYOUT_U(0xf53f34u)=0;LAYOUT_U(0xf53f38u)=0;LAYOUT_U(0xf53f3cu)=0;
 LAYOUT_U(0xf53f40u)=0;LAYOUT_U(0xf53f44u)=0;
 do {
  LAYOUT_U(source)=(uint32_t)x;
  LAYOUT_W(destination-2u)=(uint16_t)x;
  LAYOUT_W(destination)=LAYOUT_W(source+4u);
  LAYOUT_W(destination+2u)=0;
  source+=8u;destination+=0x78u;
 }while(source<0x5e44d0u);
}
