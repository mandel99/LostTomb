#include "display_confirmation.h"
#include <mmsystem.h>
uint32_t restore_display_confirmation_clock(void)
{
    return (uint32_t)timeGetTime();
}
