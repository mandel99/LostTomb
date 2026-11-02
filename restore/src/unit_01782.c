#include "graphics_session.h"
#include <stdio.h>
#include <string.h>

int FUN_004d0040(void)
{
    uint32_t now=restore_graphics_platform()->ticks();
    uint32_t elapsed=now-GFX_U(0xe36c88);
    GFX_U(0xe36c88)=now;
    return (int32_t)elapsed;
}
