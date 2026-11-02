#include "viewport_controller.h"

void FUN_004dddc0(void)
{
    /* Native facade loads right-to-left before pushing the three dword args. */
    uint32_t camera_mode=viewport_read32(0x00bca2d8u);
    uint32_t camera_y=viewport_read32(0x00bca2c0u);
    uint32_t camera_x=viewport_read32(0x00bca2bcu);
    FUN_00430d30(camera_x,camera_y,camera_mode);
}
