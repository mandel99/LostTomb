#include "map_refresh_postpass5.h"
int32_t FUN_0051c620(int32_t value)
{
    /* Exact32-bit ADD; avoid signed-overflow undefined behavior before the
       explicit target conversion back to MSVC's signed32-bit return ABI. */
    return (int32_t)((uint32_t)value+21u);
}
