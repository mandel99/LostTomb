#include "mausoleum.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Whole4DF2C0: instant completion covers the contiguous 116-part monument,
   deriving its root from the selected part's signed WORD piece index. */
void FUN_004df2c0(int32_t building)
{
    uint32_t root=(uint32_t)building-(uint32_t)(int32_t)(int16_t)W(0x7999eau+(uint32_t)building*264u),part,offset;
    for(part=0;part<116u;++part){
        offset=(root+part)*264u;
        U(0x799a30u+offset)=(U(0x5e2ae4u+part*52u)&0xffu)<4u?0xffffu:2u;
        U(0x799a3cu+offset)=255;U(0x799a38u+offset)=3;B(0x799a4cu+offset)=1;W(0x7999eeu+offset)=0xffffu;
    }
    FUN_004e0c80(building);
}
