#include "sprite_cycle_numeric.h"
#include "map_x87_conversion.h"

/* Whole55D4F0. Keep the extended intermediate across the clock conversion;
   MSVC long double is binary64 and cannot represent the native ST0/ST1 pair. */
static uint32_t scaled_step(RestoreSpriteCycle *self,uint32_t *output)
{
    uint8_t delta80[10],ratio80[10];
    uint64_t elapsed;
    int32_t divisor;
    uint32_t step;
    uint16_t status;
    __asm { mov eax,self }
    __asm { fld dword ptr[eax+50h] }
    __asm { fmul dword ptr[eax+4ch] }
    __asm { mov edx,5702ach }
    __asm { fmul dword ptr[edx] }
    __asm { fst dword ptr step }
    __asm { lea eax,delta80 }
    __asm { fstp tbyte ptr[eax] }
    if(CY_U(0x6044b0u)){
        elapsed=(uint64_t)CY_U(0x12651a0u);
        __asm { mov edx,5702a8h }
        __asm { fld dword ptr[edx] }
        __asm { mov edx,6044ach }
        __asm { fdiv dword ptr[edx] }
        __asm { lea eax,ratio80 }
        __asm { fstp tbyte ptr[eax] }
        /* Native FTOL returns signed64, but FIDIV consumes only signed EAX. */
        divisor=(int32_t)(uint32_t)restore_map_trunc80(ratio80);
        __asm { lea eax,delta80 }
        __asm { fld tbyte ptr[eax] }
        __asm { fild qword ptr elapsed }
        __asm { fidiv divisor }
        __asm { fmulp st(1),st(0) }
        __asm { fstp tbyte ptr[eax] }
    }
    __asm { lea eax,delta80 }
    __asm { fld tbyte ptr[eax] }
    __asm { fst dword ptr step }
    __asm { mov edx,56f480h }
    __asm { fcomp dword ptr[edx] }
    __asm { fnstsw status }
    *output=step;
    return (status&0x100u)!=0;
}
uint32_t restore_sprite_cycle_tick(RestoreSpriteCycle *self)
{
    uint32_t step,negative=scaled_step(self,&step);
    if(negative){
        if(restore_sprite_cycle_add_reverse(self,step))return restore_sprite_cycle_wrap_reverse(self);
    }else if(restore_sprite_cycle_add_forward(self,step))return restore_sprite_cycle_wrap_forward(self);
    return 0;
}
RESTORE_CYCLE_ENTRY0(FUN_0055d4f0,restore_sprite_cycle_tick)
