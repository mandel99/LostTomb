#include "placement_preview.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Whole4893C0: terrain/object rectangle query. Native AL is boolean, while
   the high24EAX bits retain the last tile or row count. Preserve both. */
uint32_t FUN_004893c0(int32_t x,int32_t y,int32_t width,int32_t height,uint32_t bits)
{
    uint32_t base=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
    uint32_t row,col,tile,pointer,result=(uint32_t)height;
    U(0xbc9768u)=base;
    for(row=0;I(row)<height;++row){
        for(col=0;I(col)<width;++col){
            tile=base+U(0x5cc8c0u+row*48u+col*8u);
            if(bits&0x10000u){pointer=U(0xd05720u+tile*4u);if(pointer&&B(pointer+24u)==0)return (tile&0xffffff00u)|1u;}
            if(U(0xaf598cu+tile*4u)&bits)return (tile&0xffffff00u)|1u;
        }
        result=row+1u;
    }
    return result&0xffffff00u;
}
