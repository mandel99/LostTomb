#include "restore_types.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))

uint32_t FUN_0047a1c0(int32_t x)
{
    int32_t half;
    uint32_t eax;
    /* Native callers consume AL. With the boundary disabled, the native
       high EAX bits are incoming scratch state; give those unused bits zero. */
    if (I(0x78d7b0) <= 0) return 0;
    half = (int32_t)(U(0x78d7ac) - U(0x78d7b0)) / 2;
    eax = (uint32_t)half;
    return (eax & 0xffffff00u) |
           (uint32_t)(half > (int32_t)(228u - (uint32_t)x));
}
