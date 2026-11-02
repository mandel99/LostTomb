#include "upper_raster.h"
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))

/* Upper images are RLE rows: FF,count skips transparent pixels; a literal
   control precedes that many packed16-bit source pixels. The center masked
   decoder has the same1..16/default15 dispatch as the original unmasked
   decoder. Edge decoders instead consume a general byte count (zero still
   executes one literal iteration in the native loop). */
void restore_upper_rle(const void *pixels,int side,int operation,uint16_t mask)
{
    int32_t width=H(0xd3b5be),rows=I(0xd3b5d4);
    uint32_t destination,pitch=U(0xe38d94);
    const uint8_t *source;
    if(!pixels||width<=0||rows<=0)return;
    source=(const uint8_t *)FUN_00410f20((char *)pixels);
    if(operation==2)mask=*(uint16_t *)(uintptr_t)0xd3b51cu;
    destination=U(0xe92e18)+U(0xd3b5e0)*2u+U(0xd3b5dc)*pitch;
    while(rows-->0) {
        int32_t remaining=width;
        uint32_t cutoff=side==1?U(0xd3b540)>>1:U(0xd3b544);
        uint32_t cursor=destination-(side==1?cutoff*2u:0u);
        while(remaining>0) {
            unsigned run=*source++,i;
            if(run==255) {
                run=*source++;cursor+=run*2u;cutoff-=run;
            }else{
                if(side==0){if(run<1||run>16)run=15;}
                else if(run==0)run=1;
                for(i=0;i<run;++i) {
                    if(side==0||(side==1?(int32_t)cutoff<=0:(int32_t)cutoff>0)) {
                        uint16_t value;
                        if(operation==2)value=mask;
                        else if(operation==1){memcpy(&value,(const void *)(uintptr_t)cursor,2);value&=mask;}
                        else memcpy(&value,source,2);
                        memcpy((void *)(uintptr_t)cursor,&value,2);
                    }
                    source+=2;cursor+=2;--cutoff;
                }
            }
            remaining-=(int32_t)run;
        }
        destination=side?destination+pitch:cursor+U(0xd3b53c);
    }
}
