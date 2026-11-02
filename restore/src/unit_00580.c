#include "minimap.h"
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern int FUN_004ea9c0(int kind);
int32_t FUN_0043ee10(int32_t actor)
{
    while(actor) {
        uint32_t a=0xbcd600u+(uint32_t)actor*388u;uint8_t kind=B(a+2);
        if(kind>=11&&kind<=13) {
            int32_t formation=H(a+88);
            return H(0xbcba4au+(uint32_t)formation*144u)<0;
        }
        if((kind>=43&&kind<=57)||kind==99||(kind==41&&B(a+48)==11))return 2;
        if((uint8_t)FUN_004ea9c0(kind))return 3;
        actor=H(a);
    }
    return 0;
}
