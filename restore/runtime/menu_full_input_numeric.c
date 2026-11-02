#include "menu_full_input_numeric.h"

uint32_t restore_menu_input_length32(int32_t delta_x,int32_t delta_y)
{
    uint32_t result;
    /* Preserve the original extended multiply/add/sqrt sequence and its one
       binary32 spill. Native PC/RC are taken from the current hardware CW. */
    __asm {
        fild delta_y
        fild delta_x
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fsqrt
        fstp dword ptr result
        fstp st(0)
        fstp st(0)
    }
    return result;
}

uint32_t restore_menu_input_ratio32(int32_t delta,uint32_t length_bits)
{
    uint32_t result;
    __asm {
        fild delta
        fdiv dword ptr length_bits
        fstp dword ptr result
    }
    return result;
}

void restore_menu_input_coordinate80(int32_t origin,uint32_t ratio_bits,int32_t distance,uint8_t raw80[10])
{
    /* Actual route origins are signed WORD coordinates. FILD(origin) is exact,
       as is the native Y-origin FILD then FSTP32/FLD32 for that domain. */
    __asm {
        fild origin
        fild distance
        fld dword ptr ratio_bits
        fmul st(0),st(1)
        fadd st(0),st(2)
        mov eax,raw80
        fstp tbyte ptr[eax]
        fstp st(0)
        fstp st(0)
    }
}

uint32_t restore_menu_input_compare32(int32_t integer,uint32_t float_bits)
{
    uint16_t status;
    __asm {
        fild integer
        fcom dword ptr float_bits
        fnstsw status
        fstp st(0)
    }
    return status&0x4500u;
}

uint64_t restore_menu_input_trunc80(const uint8_t raw80[10])
{
    uint64_t mantissa=0,magnitude;
    uint32_t sign=raw80[9]>>7,exponent=(uint32_t)raw80[8]|((uint32_t)(raw80[9]&127u)<<8),i;
    int32_t shift;
    const uint64_t indefinite=UINT64_C(0x8000000000000000);
    for(i=0;i<8;++i)mantissa|=(uint64_t)raw80[i]<<(i*8u);
    if(exponent==0)return 0;
    if(exponent==32767u||!(mantissa&indefinite))return indefinite;
    shift=(int32_t)exponent-16383;
    if(shift<0)return 0;
    if(shift>=63)return indefinite; /* Includes the identical valid INT64_MIN. */
    magnitude=mantissa>>(63u-(uint32_t)shift);
    return sign?UINT64_C(0)-magnitude:magnitude;
}
