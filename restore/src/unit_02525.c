#include "graphics_53f250.h"
#include "sprite_render.h"
#include <string.h>

#define U32(a) (*(uint32_t *)(uintptr_t)(a))
#define U16(a) (*(uint16_t *)(uintptr_t)(a))
#define U8(a) (*(uint8_t *)(uintptr_t)(a))

extern uint32_t FUN_004105c0(const void *pixels);
extern uint32_t FUN_00410660(const void *pixels);
extern void FUN_00410700(const void *pixels,uint16_t color);
extern void FUN_004109d0(const void *pixels,uint16_t color);
extern void FUN_00410a70(const void *pixels,uint16_t color);

static uint32_t upper_u32(const void *p)
{
    uint32_t value;memcpy(&value,p,sizeof(value));return value;
}

static uint16_t upper_u16(const void *p)
{
    uint16_t value;memcpy(&value,p,sizeof(value));return value;
}

/* Native 0053f250 draws the upper RLE portion of a terrain image. Image
   dimensions and the derived upper height are words; positions and pixels
   are dwords. Unknown widths deliberately leave the prior position/height.
   All 123 recovered native call sites bind ECX to the global manager. */
int restore_sprite_draw_upper(RestoreSpriteLibraries *self,int image,int x,int y)
{
    const uint8_t *record;
    uint32_t pixels,index;
    uint16_t width,height;
    unsigned int size;

    if(image<1||image>=25500)return 0;
    index=image<(int32_t)self->next_image?(uint32_t)image:0u;
    record=self->images[index].bytes;
    if(record[0x35]==0)return 0;

    pixels=upper_u32(record);
    U32(0x00d3b5b8)=pixels;
    if((int32_t)pixels<=0)return 0;
    pixels+=upper_u32(record+8);
    U32(0x00d3b5b8)=pixels;
    width=upper_u16(record+0x14);
    U16(0x00d3b5be)=width;

    switch(width) {
    case 358:size=6;break;
    case 298:size=5;break;
    case 238:size=4;break;
    case 178:size=3;break;
    case 118:size=2;break;
    case 58:size=1;break;
    default:size=0;break;
    }
    if(size) {
        height=upper_u16(record+0x16);
        /* Unsigned arithmetic preserves x86 wraparound at both widths. */
        U32(0x00d3b5e0)=(uint32_t)x-30u*(size-1u);
        U32(0x00d3b5dc)=(uint32_t)y-(uint32_t)(int32_t)(int16_t)height+30u*size;
        U16(0x00d3b5bc)=(uint16_t)(height-(15u*size+1u));
    }

    FUN_004cdad0();
    FUN_004cdb80();
    if(U8(0x00d3b530)!=5) {
        if(U8(0x00d3b531)==1)
            FUN_004105c0((const void *)(uintptr_t)U32(0x00d3b5b8));
        else if(U8(0x00d3b531)==2)
            FUN_00410660((const void *)(uintptr_t)U32(0x00d3b5b8));
        else
            FUN_00410230((const void *)(uintptr_t)U32(0x00d3b5b8));

        /* The original reloads the color, clipping flag and pixel pointer
           after the base pass. Raster consumers use the low color word. */
        if(U16(0x0070e120)!=0) {
            if(U8(0x00d3b531)==1)
                FUN_004109d0((const void *)(uintptr_t)U32(0x00d3b5b8),U16(0x0070e120));
            else if(U8(0x00d3b531)==2)
                FUN_00410a70((const void *)(uintptr_t)U32(0x00d3b5b8),U16(0x0070e120));
            else
                FUN_00410700((const void *)(uintptr_t)U32(0x00d3b5b8),U16(0x0070e120));
        }
    }
    return 1;
}

int FUN_0053f250(int image,int x,int y)
{
    return restore_sprite_draw_upper(RESTORE_SPRITE_LIBRARIES,image,x,y);
}
