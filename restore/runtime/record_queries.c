#include "record_place.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
uint32_t FUN_00443040(uint32_t record_index,uint32_t kind)
{
    uint32_t i,base=0x77f77cu+record_index*98u;
    for(i=0;i<8u;++i)if(B(base+i)==kind)return 1;
    return 0;
}
uint32_t FUN_00444950(int32_t kind)
{
    if(kind<0||kind>=50)return 0;
    return B(0x7770f0u+(uint32_t)kind*324u)==2u;
}
uint32_t FUN_00444a00(uint32_t record_index,uint32_t kind)
{
    uint32_t r=0x77f748u+record_index*98u,v;
    if(B(r)!=1u||!B(r+24u))return 0;
    v=B(r+kind+62u);return v<=40u?v:0u;
}
