#include "display_configuration.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#define I32(address) (*(int32_t *)(address##u))
#define U32(address) (*(uint32_t *)(address##u))
extern uint32_t FUN_004cff20(void);
static void require(int value) {if(!value) abort();}
static void check(int mode,int width,int height,int offset_x,int offset_y)
{
    I32(0x00d3b554)=I32(0x00d3b55c)=-123456;
    restore_display_configure((uint32_t)mode);
    require(U32(0x00e38e68)==(uint32_t)mode);
    require(I32(0x00e38db0)==width&&I32(0x00e38dac)==height);
    require(I32(0x00e38d9c)==width&&I32(0x00e38d98)==height&&I32(0x00e38d90)==height);
    require(I32(0x00e38da8)==offset_x&&I32(0x00e38da4)==offset_y);
    require(U32(0x00e38d94)==(uint32_t)width*2u&&U32(0x00e38d8c)==(uint32_t)width*2u*(uint32_t)height);
    require(I32(0x00d3b550)==height&&I32(0x00d3b558)==width&&I32(0x00d3b554)==0&&I32(0x00d3b55c)==0);
}
int main(void)
{
    uint32_t available;
    require(restore_data_open());
    *(unsigned char *)0x00e38db3u=0x7f;
    *(unsigned char *)0x00e38dafu=0x7e;
    check(1,640,480,0,0);
    check(2,800,600,80,60);
    check(3,1024,768,192,144);
    /* Unknown modes retain both dimensions, including full signed words. */
    I32(0x00e38db0)=639; I32(0x00e38dac)=477;
    check(0,639,477,0,-1);
    I32(0x00e38db0)=0x70000001; I32(0x00e38dac)=3;
    check(-1,0x70000001,3,939523776,-238);
    *(unsigned char *)0x00e39107u=0x93;
    U32(0x00e39128)=0x1234fedcu;
    available=FUN_004cff20();
    require(available==((MEMORYSTATUS *)0x00e39108u)->dwAvailPhys&&available>255);
    require(((MEMORYSTATUS *)0x00e39108u)->dwLength==32);
    require(*(unsigned char *)0x00e39107u==0x93&&U32(0x00e39128)==0x1234fedcu);
    restore_data_close();
    puts("Display/startup state: full-width modes, centering, invalid-mode retention, x86 wrapping and physical-memory return verified; no window used.");
    return 0;
}
