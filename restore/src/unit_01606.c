#include "temple_upgrade.h"

/* Whole4C52D0: BYTE value3 means both upgrade bits are present.
   All callers consume AL; the desired argument is a full DWORD. */
uint8_t restore_temple_upgrade_present(int32_t building,uint32_t desired)
{
    uint8_t upgrades=*(volatile uint8_t *)(uintptr_t)(0x799a81u+(uint32_t)building*264u);
    return (uint8_t)(upgrades==3u||(uint32_t)upgrades==desired);
}
uint8_t FUN_004c52d0(int32_t building,uint32_t desired)
{return restore_temple_upgrade_present(building,desired);}
