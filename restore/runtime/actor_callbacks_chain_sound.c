#include "actor_callbacks_chain.h"
/* Current accepted sound entry is a no-argument no-op. Keep the native event
   boundary explicit without fabricating a new shared sound ABI. */
extern uint32_t FUN_0047d350(void);
void restore_actor_chain_sound(int32_t event){(void)event;(void)FUN_0047d350();}
