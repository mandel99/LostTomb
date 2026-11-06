#include <stdint.h>
#define DAT_00bca2d0 (*(int32_t *)0x00bca2d0u)
#define DAT_00bca2d4 (*(int32_t *)0x00bca2d4u)
extern uint32_t FUN_00486810(uint32_t,uint32_t,uint32_t,uint32_t,
                            int32_t,int32_t,int32_t,int32_t);

/* Native 4867E0 forwards the complete BOOL result, consumed by TEST EAX
   at 4A3141/4ACB40. Keep the existing eight-argument C algorithm. */
uint32_t FUN_004867e0(uint32_t request_a,uint32_t request_b,
                     uint32_t reserved,uint32_t tile_mask)
{
    return FUN_00486810(request_a,request_b,reserved,tile_mask,
                        0,0,DAT_00bca2d0,DAT_00bca2d4);
}
