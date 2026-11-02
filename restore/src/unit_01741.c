#include "map_file_load.h"
void FUN_004ce2d0(void)
{
    MFE(0x4ce2d0,0,0,0);
    if(map_file_chdir((char *)(uintptr_t)0xe36b94)!=0){
        (void)map_file_getcwd((char *)(uintptr_t)0xe36e84,100);
        FUN_004ce6e0((char *)(uintptr_t)0x5e06ec,0xe36e84,0);
    }
}
