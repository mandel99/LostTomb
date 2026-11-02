#include "camera_navigation.h"
uint32_t FUN_00430cc0(void)
{
    return (*(volatile uint32_t *)(uintptr_t)0x70e458u+2u)
        * *(volatile uint32_t *)(uintptr_t)0x70e464u;
}
