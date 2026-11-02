#include "actor_order_los.h"
#include "menu_full_input_numeric.h"
void restore_los_slopes80(int32_t delta_x,int32_t delta_y,uint8_t slope[10],uint8_t inverse[10])
{
    uint32_t zero=0;
    if(delta_x==0){
        __asm {
            mov eax,056f488h
            fld dword ptr[eax]
            fld dword ptr zero
        }
    }else if(delta_y==0){
        __asm {
            mov eax,056f480h
            fld dword ptr[eax]
            mov eax,056f488h
            fld dword ptr[eax]
        }
    }else{
        __asm {
            fild delta_y
            fild delta_x
            fdivp st(1),st(0)
            mov eax,056f484h
            fld dword ptr[eax]
            fdiv st(0),st(1)
        }
    }
    __asm {
        mov eax,inverse
        fstp tbyte ptr[eax]
        mov eax,slope
        fstp tbyte ptr[eax]
    }
}
void restore_los_intersection80(int32_t origin,int32_t delta,const uint8_t ratio[10],uint8_t result[10])
{
    __asm {
        mov eax,ratio
        fld tbyte ptr[eax]
        fild delta
        fmul st(0),st(1)
        fiadd origin
        mov eax,result
        fstp tbyte ptr[eax]
        fstp st(0)
    }
}
uint32_t restore_los_equal_max80(const uint8_t value[10])
{
    uint16_t status;
    __asm {
        mov eax,value
        fld tbyte ptr[eax]
        mov eax,056f488h
        fcom dword ptr[eax]
        fnstsw status
        fstp st(0)
    }
    return (status&0x4000u)!=0;
}
uint64_t restore_los_trunc80(const uint8_t value[10])
{
    return restore_menu_input_trunc80(value);
}
