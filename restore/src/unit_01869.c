#include "world_route_geometry.h"
#include "menu_full_input_routes.h"
/* Whole4DD8D0: full EAX; an empty route returns-1. */
int32_t FUN_004dd8d0(const volatile RestoreWorldRoute *self)
{return restore_menu_route_last((uint32_t)(uintptr_t)self);}
