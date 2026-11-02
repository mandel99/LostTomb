#include "actor_callbacks_human.h"
/* Both original callers push x,y; the authenticated17-instruction callee reads
   only stack+4. Retain the caller contract around the accepted one-input leaf. */
uint8_t restore_actor_human_boundary(int32_t x,int32_t unused_y)
{(void)unused_y;return (uint8_t)FUN_0047a1c0(x);}
