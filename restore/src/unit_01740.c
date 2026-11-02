#include "map_file_load.h"
/* Match the native paired BYTE comparison and its repeated metadata reads. */
static uint32_t extension_equal(uint32_t literal)
{
    uint32_t text=0xe38aec,a,b;
    for(;;){
        a=MFR(literal,1);b=MFR(text,1);if(a!=b)return 0;if(!a)return 1;
        a=MFR(literal+1u,1);b=MFR(text+1u,1);if(a!=b)return 0;
        literal+=2;text+=2;if(!a)return 1;
    }
}
uint32_t FUN_004ce100(char *path)
{
    uint32_t i,drive;
    MFE(0x4ce100,path,0,0);
    MFE(0x4ce330,path,0,0);FUN_004ce330(path);
    MFE(0x4cbec0,0xe38aec,0,0);FUN_004cbec0((unsigned char *)(uintptr_t)0xe38aec);
    if(!extension_equal(0x5e06e8)&&!extension_equal(0x5e06e4)&&!extension_equal(0x5e06e0))return 0;
    for(i=0;i<10;++i)MFW(0xe36bf8u+i*4u,4,0);
    if(extension_equal(0x5e06e8)||extension_equal(0x5e06e4)||extension_equal(0x5e06e0)){
        MFE(0x4cbea0,0x5e06dc,0xe36bf8,10);FUN_004cbea0(0x5e06dc,(char *)(uintptr_t)0xe36bf8,10);
    }
    drive=MFR(0xe38e62u,1);MFW(0xe36bf8u,1,drive);
    return map_file_chdir((char *)(uintptr_t)0xe36bf8)==0;
}
