#include "raw_tile_raster.h"
#define RT_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void FUN_00410fb0(const void *pixels)
{
    uint32_t base=RT_U(0xe92e18),x=RT_U(0xd3b5e0),y=RT_U(0xd3b5dc);
    uint32_t pitch=RT_U(0xe38d94),destination=base+x*2u+y*pitch;
    uint32_t source=(uint32_t)(uintptr_t)pixels,row,column;
    pitch=RT_U(0xe38d94);
    /* Preserve the native interleaved DWORD loads/stores, including overlapping
       inputs. A bulk copy can change the semantics of this raster. */
    for(row=0;row<16;++row){
        for(column=0;column<8;++column){
            uint32_t value=RT_U(source);
            RT_U(destination+column*4u)=value;
            source+=4u;
        }
        destination+=pitch;
    }
}
