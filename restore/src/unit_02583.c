#include "monument_footprint.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
/* Whole native547A60:102 bytes,36 instructions. Includes the final index;
   the related492FC0 loop deliberately excludes it. Reload count per call. */
void restore_monument_refresh_all_tiles(void)
{
 uint32_t id,self;
 for(id=1;(int32_t)id<=(int32_t)U(0x78d790u);++id){
  self=restore_monument_receiver(W(0x7999e8u+id*264u));
  if(self)restore_monument_commit_footprint(self,(int32_t)id);
 }
}
void FUN_00547a60(void){restore_monument_refresh_all_tiles();}
