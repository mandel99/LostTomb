#include "sprite_render.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))

/* Whole4381B0: signed WORD offsets in64-byte image records. Metadata alone
   falls back to record0; the original requested image still reaches drawing. */
int FUN_004381b0(int32_t image,int32_t x,int32_t y,int32_t dx,int32_t dy)
{
    uint32_t limit=U(0x126517cu),index=(int32_t)image<(int32_t)limit?(uint32_t)image:0;
    uint32_t px=(uint32_t)dx-(uint32_t)H(0x10d6a1au+index*64u);
    uint32_t py=(uint32_t)y-(uint32_t)H(0x10d6a1cu+index*64u);
    return FUN_0053e590(image,(int32_t)((uint32_t)x+px),(int32_t)(py+(uint32_t)dy));
}
