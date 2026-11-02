#ifndef RESTORE_SPRITE_CYCLE_NUMERIC_H
#define RESTORE_SPRITE_CYCLE_NUMERIC_H
#include "sprite_cycle.h"
#define CY_U(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define CY_W(a) (*(volatile uint16_t *)(uintptr_t)(a))
#define CY_B(a) (*(volatile uint8_t *)(uintptr_t)(a))
/* Tiny numeric boundaries retain native x87 precision, inherited PC/RC,
   unordered comparisons and the comparison BEFORE rounding to binary32.
   Exceptions must be masked; FPSW/trap equivalence is not claimed. */
static __inline uint32_t cycle_less(RestoreSpriteCycle *self,float limit_value)
{
    uint16_t status;
    __asm { mov eax,self }
    __asm { fld dword ptr[eax+48h] }
    __asm { fcomp limit_value }
    __asm { fnstsw status }
    return (status&0x100u)!=0;
}
static __inline uint32_t cycle_sum(RestoreSpriteCycle *self,uint32_t bits,int subtract,float limit_value)
{
    uint16_t status;
    if(subtract){
        __asm { mov eax,self }
        __asm { fld dword ptr[eax+48h] }
        __asm { fsub dword ptr bits }
    }else{
        __asm { mov eax,self }
        __asm { fld dword ptr bits }
        __asm { fadd dword ptr[eax+48h] }
    }
    __asm { fst dword ptr[eax+48h] }
    __asm { fcomp limit_value }
    __asm { fnstsw status }
    return (status&0x100u)!=0;
}
static __inline float cycle_length(RestoreSpriteCycle *self)
{return (float)CY_W(self->variant+2u);}
static __inline uint16_t cycle_repeat(RestoreSpriteCycle *self)
{return (uint16_t)(CY_B(self->descriptor+3u)<<((CY_B(self->descriptor)&7u)==4u));}
static __inline void cycle_integer_frame(RestoreSpriteCycle *self,int32_t frame)
{
    __asm { mov eax,self }
    __asm { fild frame }
    __asm { fstp dword ptr[eax+48h] }
}
static __inline void cycle_last_frame(RestoreSpriteCycle *self,float count_value)
{
    const float one=1.0f;
    __asm { mov eax,self }
    __asm { fld count_value }
    __asm { fsub one }
    __asm { fstp dword ptr[eax+48h] }
}
#endif
