#include "quicktime_dispatch.h"
/* Native5602F0 supplies one contiguous8-byte block, not separate C locals.
   Return the actual child register result; the window caller discards it. */
uint32_t FUN_005602f0(uint32_t controller,RestoreQuickTimeEvent *event)
{
    RestoreQuickTimeComponentParameters parameters;
    parameters.flags=0;
    parameters.parameter_bytes=4;
    parameters.request=7;
    parameters.event=(uint32_t)(uintptr_t)event;
    return FUN_005605e0(controller,&parameters);
}
