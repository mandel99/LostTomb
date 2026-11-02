#include "mirrored_raster.h"
#define MR_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define MR_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define MR_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

void FUN_0040fc40(const void *pixels)
{
    uint32_t source=(uint32_t)(uintptr_t)FUN_00410f20((char *)pixels);
    uint32_t pitch=MR_U(0xe38d94),frame=MR_U(0xe92e18),y=MR_U(0xd3b5dc);
    uint32_t width_bytes=(uint32_t)(int32_t)(int16_t)MR_W(0xd3b5be)*2u;
    uint32_t destination=frame+pitch*y+MR_U(0xd3b5e0)*2u;
    uint32_t rows=MR_U(0xd3b5d4),remaining,run;
    uint16_t pixel;
    if((int32_t)rows>0){
        do{
            remaining=(uint32_t)(int32_t)(int16_t)MR_W(0xd3b5be);
            destination+=width_bytes;
            while((int32_t)remaining>0){
                run=MR_B(source);++source;
                if(run==255){
                    run=MR_B(source);++source;destination-=run*2u;remaining-=run;
                }else{
                    while(run!=0){
                        if(remaining==0)goto row_done;
                        pixel=MR_W(source);MR_W(destination)=pixel;
#ifdef RESTORE_MIRRORED_RASTER_TRACE
                        restore_mirrored_raster_pixel(destination,pixel);
#endif
                        source+=2u;destination-=2u;--remaining;--run;
                    }
                }
            }
row_done:
            destination+=MR_U(0xd3b53c)+width_bytes;
            --rows;
        }while(rows!=0);
    }
#ifdef RESTORE_MIRRORED_RASTER_TRACE
    restore_mirrored_raster_finish(source);
#endif
}
