#include "minimap.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
void FUN_0040fbe0(const void *pixels)
{
    uint32_t target=U(0xe92e18)+U(0xd3b5e0)*2u+U(0xd3b5dc)*U(0xe38d94);
    U(target)=*(const uint32_t *)pixels;
}
