#include "mirrored_clip.h"

void FUN_0040fdd0(const void *pixels)
{
    uint32_t source=(uint32_t)(uintptr_t)FUN_00410f20((char *)pixels);
    uint32_t pitch=MC_U(0xe38d94),frame=MC_U(0xe92e18),y=MC_U(0xd3b5dc);
    uint32_t width_bytes=(uint32_t)(int32_t)(int16_t)MC_W(0xd3b5be)*2u;
    uint32_t row_base=frame+pitch*y+MC_U(0xd3b5e0)*2u;
    uint32_t rows=MC_U(0xd3b5d4),remaining,cutoff,clip_count,clip_position,destination,run;
    uint16_t pixel;
    if((int32_t)rows>0){
        do{
            remaining=(uint32_t)(int32_t)(int16_t)MC_W(0xd3b5be);
            clip_count=MC_U(0xd3b540);clip_position=MC_U(0xd3b540);
            /* Native SAR rounds negative odd byte counts downward. */
            clip_count=(clip_count>>1)|(clip_count&0x80000000u);
            cutoff=remaining-clip_count;destination=row_base+width_bytes-clip_position;
            while((int32_t)remaining>0){
                run=MC_B(source);
                if(run==255){
                    run=MC_B(source+1u);source+=2u;cutoff-=run;remaining-=run;destination-=run*2u;
                }else{
                    ++source;
                    if(run!=0){
                        remaining-=run;
                        do{
                            if((int32_t)cutoff>0){
                                pixel=MC_W(source);MC_W(destination)=pixel;
#ifdef RESTORE_MIRRORED_CLIP_TRACE
                                restore_mirrored_clip_pixel(destination,pixel);
#endif
                            }
                            destination-=2u;source+=2u;--cutoff;--run;
                        }while(run!=0);
                    }
                }
            }
            row_base+=MC_U(0xe38d94);--rows;
        }while(rows!=0);
    }
#ifdef RESTORE_MIRRORED_CLIP_TRACE
    restore_mirrored_clip_finish(source);
#endif
}
