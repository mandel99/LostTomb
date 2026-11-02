#include "terrain_diamond.h"
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
void FUN_004cdad0(void);
void FUN_004cdb80(void);

/* Packed diamond: thirty rows, widths2,6,...,58,58,...,6,2.
   The engine's half-cell edge paths crop exactly thirty left pixels or
   retain exactly twenty-eight right-edge pixels, not a general clip width.
   Vertical flag3 starts the lower half at destination row0; flag4 emits
   only the upper half. Source rows retain their original packed offsets. */
void restore_terrain_diamond(const void *pixels,int vertical_clip,int side,int masked)
{
    const uint8_t *source=pixels;
    uint32_t destination=U(0xe92e18)+U(0xd3b5e0)*2u+U(0xd3b5dc)*U(0xe38d94);
    uint32_t pitch=U(0xe38d94),output_row=0;
    uint16_t color=W(0x70e120);
    unsigned row;
    for(row=0;row<30;++row) {
        unsigned half=row<15?row:29u-row;
        unsigned width=half*4u+2u,left=(58u-width)/2u,start=left,end=left+width,column;
        if(vertical_clip==3&&row<15){source+=width*2u;continue;}
        if(vertical_clip==4&&row>=15)break;
        if(side==1&&start<30)start=30;
        if(side==2&&end>28)end=28;
        for(column=start;column<end;++column) {
            uint16_t value;
            uint32_t x=column-(side==1?30u:0u);
            unsigned source_column=column-left;
            /* Original left-edge assembly repeats one dword on row15
               (42a5f8/42af69), then preserves the prior high EAX word
               on row25 (42a9ae/42b3db). Preserve both visible quirks. */
            if(side==1&&((row==15&&(column==38||column==39))||(row==25&&column==37)))source_column-=2;
            memcpy(&value,source+source_column*2u,2);
            if(masked)value&=color;
            memcpy((void *)(uintptr_t)(destination+output_row*pitch+x*2u),&value,2);
        }
        source+=width*2u;++output_row;
    }
}

/* A sizeN footprint stores N*N packed diamonds in increasing diagonal,
   left-to-right order. Clipping may clamp draw positions; every following
   diamond is positioned from the original footprint origin. */
int restore_terrain_footprint(unsigned size)
{
    uint32_t x=U(0xd3b5e0),y=U(0xd3b5dc),tile=0;
    unsigned diagonal,column;
    W(0xd3b5be)=58;W(0xd3b5bc)=30;
    for(diagonal=0;diagonal<2u*size-1u;++diagonal) {
        unsigned count=diagonal<size?diagonal+1u:2u*size-1u-diagonal;
        uint32_t left=x-(count-1u)*30u;
        for(column=0;column<count;++column,++tile) {
            const void *pixels;
            U(0xd3b5e0)=left+column*60u;U(0xd3b5dc)=y+diagonal*15u;
            FUN_004cdad0();FUN_004cdb80();
            if(B(0xd3b530)==5)continue;
            pixels=(const void *)(uintptr_t)(U(0xd3b5b8)+U(0xe36c80)+tile*1800u);
            if(B(0xd3b531)==1)FUN_0042a160(pixels,B(0xd3b530));
            else if(B(0xd3b531)==2)FUN_0042b440(pixels,B(0xd3b530));
            else FUN_00427a20(pixels,B(0xd3b530));
        }
    }
    return 58;
}
