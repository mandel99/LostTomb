#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))

static void mask_pixel(uint32_t source,uint32_t destination,uint16_t mask)
{
    if(W(source)!=0xf81fu)W(destination)=(uint16_t)(W(destination)&mask);
}

/* Whole4100F0. Pointer, pitch, coordinates, row count and destination skip
   are DWORDs; only the per-row width and mask are WORDs. Native callers clip
   first. Preserve the initial four/scalar pixels, forward access order and
   live row width/skip, including the native first iteration for zero counts. */
void FUN_004100f0(const void *pixels,uint16_t mask)
{
    uint32_t source=(uint32_t)(uintptr_t)pixels;
    uint32_t destination=U(0xe92e18u),rows,remaining,i;
    destination+=U(0xd3b5e0u)*2u;
    destination+=U(0xd3b5dcu)*U(0xe38d94u);
    rows=U(0xd3b5d4u);
    do{
        remaining=(uint32_t)(int32_t)(int16_t)W(0xd3b5beu);
        if(!(remaining&3u)){
            for(i=0;i<4u;++i){
                mask_pixel(source,destination,mask);
                source+=2u;destination+=2u;
            }
            remaining-=4u;
        }else{
            mask_pixel(source,destination,mask);
            source+=2u;destination+=2u;--remaining;
        }
        while((int32_t)remaining>0){
            mask_pixel(source,destination,mask);
            source+=2u;destination+=2u;--remaining;
        }
        destination+=U(0xd3b53cu);
        --rows;
    }while((int32_t)rows>0);
}
