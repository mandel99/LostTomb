#include "map_file_load.h"
/* Native result is full EAX 0/1, not the old BYTE bool typedef. */
uint32_t FUN_004ce3f0(char *path)
{
    int32_t descriptor;uint32_t found=0;
    MFE(0x4ce3f0,path,0,0);
    descriptor=map_file_open(path,0x8000);
    if(descriptor>=0){(void)map_file_close(descriptor);return 1;}
    (void)FUN_004ce100(path);
    descriptor=map_file_open(path,0x8000);
    if(descriptor>=0){found=1;(void)map_file_close(descriptor);}
    FUN_004ce2d0();return found;
}
