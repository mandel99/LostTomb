#include "ferry_preview.h"
/* Whole native43D010:23 bytes,9 instructions. Full DWORD camera, signed
   wrapped difference and truncation toward zero. */
int32_t restore_ferry_image_orientation(int32_t orientation)
{
 int32_t delta=FRY_I((uint32_t)orientation-FRY_U(0xbca2e0u));
 if(delta<0)delta=FRY_I((uint32_t)delta+8u);
 return delta/2;
}
int32_t FUN_0043d010(int32_t orientation){return restore_ferry_image_orientation(orientation);}
