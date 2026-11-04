#include "town_menu.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* 44BB90 caches the signed count once and wraps address arithmetic.
   The fallback writes the exhausted index, then returns reserved record140;
   the identifier is not written at the returned address. */
__declspec(noinline) RestoreTownRecord *restore_town_next(RestoreTownList *self,const int16_t *previous,uint8_t create)
{
    uint32_t base=(uint32_t)(uintptr_t)self;
    uint32_t first=previous?(uint32_t)(int32_t)(int16_t)W((uintptr_t)previous)+1u:0u;
    uint32_t index=first,count,record;
    B(0x78a634u)=1;
    count=U(base);
    while((int32_t)index<(int32_t)count){
        record=base+4u+index*124u;
        if(B(record+2u)==2u){
            W(record)=(uint16_t)index;
            U(0x5d9a40u)=index;
            return (RestoreTownRecord *)(uintptr_t)record;
        }
        ++index;
    }
    if(create&&(int32_t)first<140){
        W(base+4u+index*124u)=(uint16_t)index;
        U(0x5d9a40u)=index;
        return (RestoreTownRecord *)(uintptr_t)(base+0x43d4u);
    }
    U(0x5d9a40u)=U(0x5d9a40u)-1u;
    return 0;
}
/* All 24 native callers select ECX=0x785d70. Keep the existing C facade. */
int *FUN_0044bb90(const int16_t *previous,char create)
{
    return (int *)restore_town_next(RESTORE_TOWNS,previous,(uint8_t)create);
}
