#include "sprite_shutdown.h"
void FUN_004e63b0(void)
{
 void *object=*(void **)(uintptr_t)0xea136cu;
 if(object){restore_sprite_shutdown_delete(object,1);*(void **)(uintptr_t)0xea136cu=NULL;}
 object=*(void **)(uintptr_t)0xea1370u;
 if(object){restore_sprite_shutdown_delete(object,1);*(void **)(uintptr_t)0xea1370u=NULL;}
 restore_sprite_shutdown_slot((int *)(uintptr_t)0xea1368u);(void)FUN_0053db60(RESTORE_SPRITE_LIBRARIES);
}
