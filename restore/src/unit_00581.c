#include "minimap.h"
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
int32_t FUN_0043eeb0(int32_t actor)
{
    while(actor) {
        uint32_t a=0xbcd600u+(uint32_t)actor*388u;uint8_t kind=B(a+2);
        if(kind==78||kind==77||kind==25||kind==76||kind==20)return 1;
        if(kind==93||kind==92||kind==100||kind==101)return 2;
        actor=H(a);
    }
    return 0;
}
