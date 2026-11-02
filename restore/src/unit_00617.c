#include "trade_updates.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Complete443080. Both switch tables were read from the pinned original PE.
   The high-water count includes the last building, and either signed WORD
   model field may match, including model-1 for an unmapped resource. */
int32_t FUN_00443080(uint32_t resource)
{
    static const int8_t models[33]={21,-1,-1,-1,-1,-1,22,13,8,5,7,24,9,19,1,16,
        10,-1,4,15,11,12,3,6,-1,-1,14,17,18,23,20,25,26};
    int32_t model=resource-2u<33u?models[resource-2u]:-1;
    int32_t count=(int32_t)U(0x78d790u),kind;
    uint32_t p=0x799af0u,total=0;
    if(count<1)return 0;
    do{
        if(B(p-16u)==1u&&H(p+48u)!=0){
            kind=H(p);
            if(kind<110||kind>114){
                switch(kind){
                case 32:case 47:case 51:case 53:case 74:case 177:case 178:
                case 187:case 188:case 189:case 194:case 203:case 204:case 205:
                case 226:case 231:case 232:case 233:break;
                default:goto next;
                }
            }
            if(H(p+2u)==model||H(p+190u)==model)++total;
        }
next:
        p+=264u;
    }while(--count);
    return (int32_t)total;
}
