#include "world_route_geometry.h"
#include "menu_full_input_routes.h"
/* Whole4DD900: native receiver plus one BYTE argument, RET4. */
void FUN_004dd900(volatile RestoreWorldRoute *self,uint8_t enabled)
{restore_menu_route_mark((uint32_t)(uintptr_t)self,enabled);}
