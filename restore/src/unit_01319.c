#include "monument_footprint.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_004ea930(void);
/* Whole native492FC0:198 bytes,70 instructions. Consume one available
   eligible material with the greatest positive cost; earliest ties win. */
void restore_monument_consume_resource(void)
{
 uint32_t i,best=0,self;int32_t best_cost=0,cost;
 for(i=0;i<36u;++i){
  switch(i){case 1:case 8:case 10:case 13:case 15:case 17:case 18:case 19:
  case 20:case 23:case 24:case 25:case 26:case 28:case 30:break;default:continue;}
  if(U(0x784e78u+i*4u)){
   cost=(int32_t)U(0x5d2c48u+i*8u);
   if(cost>best_cost){best=i;best_cost=cost;}
  }
 }
 if(best)U(0x784e78u+best*4u)-=1u;
 if((uint8_t)FUN_004ea930())return;
 for(i=1;(int32_t)i<(int32_t)U(0x78d790u);++i){
  self=restore_monument_receiver(W(0x7999e8u+i*264u));
  if(self)restore_monument_commit_footprint(self,(int32_t)i);
 }
}
