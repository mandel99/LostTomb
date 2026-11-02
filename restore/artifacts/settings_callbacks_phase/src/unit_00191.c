#include "settings_callbacks.h"
void FUN_00416ba0(uint32_t selection)
{
    if(SSET_B(0xe38dda)==1){SSET_PUT32(0xd3b198,selection);FUN_00403320();}
}
