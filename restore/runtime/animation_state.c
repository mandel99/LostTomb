#include "animation_state.h"
/* Whole40F270/26 instructions:51 byte flags and51 DWORD time stamps.
 * Native signed comparison follows wrapped32-bit last+speed*20. */
void FUN_0040f270(void)
{
    uint32_t i,now=*(volatile uint32_t *)(uintptr_t)0xe92e0cu;
    for(i=0;i<51u;++i)*(volatile uint8_t *)(uintptr_t)(0x605b38u+i)=0;
    for(i=0;i<51u;++i){
        volatile uint32_t *last=(volatile uint32_t *)(uintptr_t)(0x605a6cu+i*4u);
        if((int32_t)now>=(int32_t)(*last+i*20u)){
            *(volatile uint8_t *)(uintptr_t)(0x605b38u+i)=1;*last=now;
        }
    }
}
void restore_animation_registry(uint32_t begin,uint32_t end)
{
    uint32_t slot;
    for(slot=begin;slot<end;slot+=16u)
        FUN_0041fdb0(*(void **)(uintptr_t)slot);
}
