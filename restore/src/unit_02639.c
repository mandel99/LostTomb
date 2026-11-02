#include "compression.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern int FUN_005664d5(uint32_t file,const void *input,uint32_t size);
/* Whole55BC50. The encoded size and raw-block sentinel are DWORDs.
   File write results are intentionally discarded as in the original. */
uint32_t FUN_0055bc50(uint32_t file,const void *input,int32_t size)
{
    uint32_t output=U(0xe92e1cu);
    U(0x1265188u)=output;
    if(size>1200000)return 0;
    U(0x1265190u)=(uint32_t)size;
    if(FUN_0055bf10((uint32_t *)(uintptr_t)0x1265194u,
            (uint32_t *)(uintptr_t)0x126518cu,input,(void *)(uintptr_t)output,(uint32_t)size)){
        (void)FUN_005664d5(file,(const void *)(uintptr_t)0x126518cu,4);
        (void)FUN_005664d5(file,(const void *)(uintptr_t)U(0x1265188u),U(0x126518cu));
    }else{
        U(0x126518cu)=0x80000000u;
        (void)FUN_005664d5(file,(const void *)(uintptr_t)0x126518cu,4);
        (void)FUN_005664d5(file,input,(uint32_t)size);
    }
    return 1;
}
