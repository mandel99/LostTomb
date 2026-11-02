#include "sprite_render.h"
#include <string.h>
/* Whole40F9A0, the reverse of40F9F0: restore the cached preview into the
 * actual16-bit framebuffer. Geometry/pitch/pointers are DWORDs, the counts
 * are signed WORDs. A zero row count still copies the native first row. */
void FUN_0040f9a0(const uint32_t *pixels,int32_t destination_skip,int32_t source_skip)
{
    uint32_t source=(uint32_t)(uintptr_t)pixels;
    uint32_t destination=*(volatile uint32_t *)(uintptr_t)0xe92e18u;
    int32_t rows=*(volatile int16_t *)(uintptr_t)0xd3b5bcu;
    destination+=*(volatile uint32_t *)(uintptr_t)0xd3b5e0u*2u;
    destination+=*(volatile uint32_t *)(uintptr_t)0xd3b5dcu**(volatile uint32_t *)(uintptr_t)0xe38d94u;
    do{
        uint32_t words=(uint32_t)(int32_t)*(volatile int16_t *)(uintptr_t)0xd3b5beu*2u;
        while(words){
            uint32_t value=*(volatile uint32_t *)(uintptr_t)source;
            *(volatile uint32_t *)(uintptr_t)destination=value;
            source+=4u;destination+=4u;--words;
        }
        destination+=(uint32_t)destination_skip;source+=(uint32_t)source_skip;
        rows=(int32_t)((uint32_t)rows-1u);
    }while(rows>0);
}
/* Whole40F9F0: native framebuffer coordinates/pitch/pointer are DWORDs;
 * row count and the count of four-pixel groups are signed WORDs. Preserve
 * forward DWORD copy order and the caller's separate row increments. */
void FUN_0040f9f0(uint32_t *destination,int32_t source_skip,int32_t destination_skip)
{
    uint32_t source=*(volatile uint32_t *)(uintptr_t)0xe92e18u;
    uint32_t output=(uint32_t)(uintptr_t)destination,words,value;
    int32_t rows=*(volatile int16_t *)(uintptr_t)0xd3b5bcu;
    source+=*(volatile uint32_t *)(uintptr_t)0xd3b5e0u*2u;
    source+=*(volatile uint32_t *)(uintptr_t)0xd3b5dcu**(volatile uint32_t *)(uintptr_t)0xe38d94u;
    do{
        words=(uint32_t)(int32_t)*(volatile int16_t *)(uintptr_t)0xd3b5beu*2u;
        while(words){
            value=*(volatile uint32_t *)(uintptr_t)source;
            *(volatile uint32_t *)(uintptr_t)output=value;
            source+=4u;output+=4u;--words;
        }
        source+=(uint32_t)source_skip;output+=(uint32_t)destination_skip;
        rows=(int32_t)((uint32_t)rows-1u);
    }while(rows>0);
}
char *FUN_00410f20(char *pixels);
void restore_sprite_rle(const void *pixels,int solid)
{
    int width=*(int16_t *)0x00d3b5beu;
    int32_t rows=*(int32_t *)0x00d3b5d4u;
    uint16_t color=*(uint16_t *)0x00d3b51cu;
    uint8_t *destination;const uint8_t *source;
    if(!pixels||width<=0||rows<=0)return;
    source=(const uint8_t *)FUN_00410f20((char *)pixels);
    destination=(uint8_t *)(uintptr_t)(*(uint32_t *)0x00e92e18u+
        *(uint32_t *)0x00d3b5e0u*2u+*(uint32_t *)0x00d3b5dcu**(uint32_t *)0x00e38d94u);
    while(rows-->0) {
        int remaining=width;
        while(remaining>0) {
            unsigned int run=*source++,i;
            if(run==255) {run=*source++;destination+=run*2u;}
            else {
                if(run<1||run>16)run=15;
                if(solid)for(i=0;i<run;++i)memcpy(destination+i*2u,&color,2);
                else memcpy(destination,source,run*2u);
                source+=run*2u;destination+=run*2u;
            }
            remaining-=(int)run;
        }
        destination=(uint8_t *)((uintptr_t)destination+*(uint32_t *)0x00d3b53cu);
    }
}
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define S(a) (*(int16_t *)(uintptr_t)(a))
#define U(a) (*(uint32_t *)(uintptr_t)(a))
void restore_sprite_copy(const void *pixels,int color_key,int clipped)
{
    const uint8_t *source=pixels;
    uint8_t *destination;
    int32_t width=(color_key&&!clipped)?S(0xd3b5be):I(0xd3b544),height=I(0xd3b5d4),row;
    int32_t source_skip=clipped?I(0xd3b540):0;
    if(width<=0||height<=0)return;
    destination=(uint8_t *)(uintptr_t)(U(0xe92e18)+U(0xd3b5e0)*2u+U(0xd3b5dc)*U(0xe38d94));
    for(row=0;row<height;++row) {
        if(!color_key)memcpy(destination,source,(size_t)width*2);
        else {
            int32_t column;
            for(column=0;column<width;++column) {
                uint16_t value;memcpy(&value,source+(size_t)column*2,2);
                if(value!=0xf81f)memcpy(destination+(size_t)column*2,&value,2);
            }
        }
        source+=(uint32_t)width*2u+(uint32_t)source_skip;
        destination+=(uint32_t)width*2u+U(0xd3b53c);
    }
}
