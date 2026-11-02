#include "mission_routes.h"
#include "world_route_geometry.h"
/* Original opaque save-loader interface, same324-byte route layout. */
uint8_t FUN_004d4720(RestoreMissionRoute *self)
{return restore_world_route_rebuild((RestoreWorldRoute *)self);}
