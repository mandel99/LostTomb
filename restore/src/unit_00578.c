#include "minimap.h"
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern uint8_t FUN_004387d0(int32_t actor);
int32_t FUN_0043e790(int32_t actor)
{
    while(actor) {
        uint32_t a=0xbcd600u+(uint32_t)actor*388u;uint8_t kind;
        if(I(0xe94e0c)==51)return 0;
        if(FUN_004387d0(actor))return 1;
        if(!I(0xe94e0c)) {
            kind=B(a+2);
            if(kind==22||kind==23||kind==24||kind==107||kind==106||kind==109)return 1;
        }
        actor=H(a);
    }
    return 0;
}
