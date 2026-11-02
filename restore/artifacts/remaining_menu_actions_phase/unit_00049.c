#include "remaining_menu_actions.h"
void FUN_00406090(void)
{
    uint32_t height,width;
    if(RA_B(0xe38dda)!=1)return;
    if(RA_U(0x605a3c)!=1)return;
    FUN_00478420();FUN_00486e70();FUN_004dea50();
    height=RA_U(0xbca2d4)-1u;width=RA_U(0xbca2d0)-1u;
    FUN_004be160(RESTORE_CITY_WORLD,0,0,(int32_t)width,(int32_t)height);
    restore_remaining_refresh_489030((void *)(uintptr_t)0xbca350u);
}
