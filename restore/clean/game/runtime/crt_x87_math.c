#include "pharaoh/runtime/crt_x87_math.h"

#if !defined(_MSC_VER) || !defined(_M_IX86)
#error These live x87 owners require the configured MSVC Win32 target.
#endif

/* Exact constants read from pinned Ghidra memory5707B0/6052E8/6052F4. */
static const uint64_t binary64_one = UINT64_C(0x3ff0000000000000);
static const uint8_t exception_huge80[10] =
    {0,0,0,0,0,0,0,0x80,0x10,0x44};
static const uint8_t exception_tiny80[10] =
    {1,0,0,0,0,0,0,0x80,0x00,0x30};

/* Whole0056758C. Reuse the old CrtAsinMath C algorithm and its live ST0. */
__declspec(noinline) uint32_t pharaoh_crt_x87_quiet_nan_live(
    uint32_t classification)
{
    if (classification & 0x80000u) return 7;
    __asm { fadd qword ptr binary64_one }
    return 1;
}

/* Whole00568448. Native snapshots the source top WORD before FLD/FSTP;
 * a signaling NaN can change that word during the round trip. */
__declspec(noinline) void pharaoh_crt_x87_set_exponent_live(
    uint64_t value, int32_t exponent)
{
    const uint16_t original_top = (uint16_t)(value >> 48) & 0x800fu;
    const uint16_t result_top = (uint16_t)(
        (((uint32_t)exponent + 0x3feu) << 4) | original_top);
    uint64_t rounded;
    __asm {
        fld qword ptr value
        fstp qword ptr rounded
    }
    rounded = (rounded & UINT64_C(0x0000ffffffffffff)) |
        ((uint64_t)result_top << 48);
    __asm { fld qword ptr rounded }
}

/* Whole00568540. FNSTSW, FNCLEX and signed WORD result, without a WAIT. */
__declspec(noinline) int32_t pharaoh_crt_x87_clear_status(void)
{
    uint16_t status;
    __asm {
        fnstsw status
        fnclex
    }
    return (int32_t)(int16_t)status;
}

/* Whole00568572. Reuse the real x87 exception operations in Native order;
 * the caller's control word decides whether they trap or set sticky flags. */
__declspec(noinline) void pharaoh_crt_x87_generate_exceptions(
    uint32_t exceptions)
{
    uint64_t spill;
    int32_t integer;
    if (exceptions & 1u) {
        __asm {
            fld tbyte ptr exception_huge80
            fistp integer
            fwait
        }
    }
    if (exceptions & 8u) {
        __asm {
            fstsw ax
            fld tbyte ptr exception_huge80
            fstp qword ptr spill
            fwait
            fstsw ax
        }
    }
    if (exceptions & 0x10u) {
        __asm {
            fld tbyte ptr exception_tiny80
            fstp qword ptr spill
            fwait
        }
    }
    if (exceptions & 4u) {
        __asm {
            fldz
            fld1
            fdivrp st(1),st(0)
            fstp st(0)
            fwait
        }
    }
    if (exceptions & 0x20u) {
        __asm {
            fldpi
            fstp qword ptr spill
            fwait
        }
    }
}


/* Shared live primitives reused from the confirmed CrtAsinMath C. */
static const uint64_t binary64_zero = 0;
static const uint64_t overflow_infinity = UINT64_C(0x7ff0000000000000);
static const uint64_t overflow_maximum = UINT64_C(0x7fefffffffffffff);

static __declspec(noinline) uint64_t x87_roundtrip64(uint64_t value)
{
    __asm {
        fld qword ptr value
        fstp qword ptr value
    }
    return value;
}

static __declspec(noinline) uint16_t x87_compare_zero64(uint64_t value)
{
    uint16_t status;
    __asm {
        fld qword ptr value
        fcomp qword ptr binary64_zero
        fnstsw status
    }
    return status;
}

static __declspec(noinline) void x87_load64(uint64_t value)
{
    __asm { fld qword ptr value }
}

static __declspec(noinline) uint64_t x87_pop64(void)
{
    uint64_t value;
    __asm { fstp qword ptr value }
    return value;
}

/* Native result stores are FSTP qword directly to the caller's buffer. */
static __declspec(noinline) void x87_store64(uint64_t value, uint64_t *destination)
{
    __asm {
        fld qword ptr value
        mov eax,destination
        fstp qword ptr[eax]
    }
}

/* Whole00568471. Preserve unordered/zero and subnormal branches. */
__declspec(noinline) void pharaoh_crt_x87_frexp_live(uint64_t value,int32_t *exponent)
{
    uint64_t fraction;int32_t scale;uint16_t status=x87_compare_zero64(value);
    if(status&0x4000u){ /* Native JZ includes unordered. */
        fraction=0;scale=0;
    }else if((value&UINT64_C(0x7ff0000000000000))==0&&
             (value&UINT64_C(0x000fffffffffffff))!=0){
        uint32_t negative=(x87_compare_zero64(value)&0x100u)!=0;
        scale=-0x3fd;
        while((value&UINT64_C(0x0010000000000000))==0){value<<=1;--scale;}
        value&=~UINT64_C(0x0010000000000000);
        if(negative)value|=UINT64_C(0x8000000000000000);
        /* Native caller materializes this binary64 argument with FLD/FSTP
           before the set-exponent helper's own binary64 round trip. */
        value=x87_roundtrip64(value);
        pharaoh_crt_x87_set_exponent_live(value,0);
        fraction=x87_pop64();
    }else{
        scale=(int32_t)((value>>52)&0x7ffu)-0x3fe;
        value=x87_roundtrip64(value);
        pharaoh_crt_x87_set_exponent_live(value,0);
        fraction=x87_pop64();
    }
    /* Native loads ST0 before publishing the exponent. */
    x87_load64(fraction);
    *exponent=scale;
}

/* Whole0056820B. Native mask priority, saturation, underflow and inexact. */
__declspec(noinline) uint32_t pharaoh_crt_x87_handle_masked_error(uint32_t exceptions,uint64_t *result,uint32_t control)
{
    uint32_t remaining=exceptions&0x1fu;
    if((exceptions&8u)&&(control&1u)){
        pharaoh_crt_x87_generate_exceptions(1);remaining&=~8u;
    }else if((exceptions&4u)&&(control&4u)){
        pharaoh_crt_x87_generate_exceptions(4);remaining&=~4u;
    }else if((exceptions&1u)&&(control&8u)){
        uint32_t rounding=control&0xc00u;
        uint16_t status;
        uint64_t replacement;
        pharaoh_crt_x87_generate_exceptions(8);
        status=x87_compare_zero64(*result);
        /* JA: positive, ordered. NaNs use the non-positive branch. */
        if(rounding==0x400u)replacement=(status&0x4100u)?overflow_infinity:overflow_maximum;
        else if(rounding==0x800u)replacement=(status&0x4100u)?overflow_maximum:overflow_infinity;
        else if(rounding==0xc00u)replacement=overflow_maximum;
        else replacement=overflow_infinity;
        if(status&0x4100u)replacement|=UINT64_C(0x8000000000000000);
        /* Native FLD/FSTP local followed by FLD/FSTP result. */
        replacement=x87_roundtrip64(replacement);
        x87_store64(replacement,result);
        remaining&=~1u;
    }else if((exceptions&2u)&&(control&0x10u)){
        uint32_t inexact=(exceptions&0x10u)!=0;
        uint64_t value=x87_roundtrip64(*result);
        if(x87_compare_zero64(value)&0x4000u)inexact=1;
        else {
            int32_t scale;uint32_t negative;
            value=x87_roundtrip64(value); /* Native FLD/FSTP call argument. */
            pharaoh_crt_x87_frexp_live(value,&scale);
            value=x87_pop64();scale-=0x600;
            if(scale< -0x432){value=0;inexact=1;}
            else {
                negative=(x87_compare_zero64(value)&0x100u)!=0;
                value=(value&UINT64_C(0x000fffffffffffff))|UINT64_C(0x0010000000000000);
                if(scale< -0x3fd){
                    int32_t shifts=-0x3fd-scale;
                    do {if(value&1u)inexact=1;value>>=1;}while(--shifts);
                }
                if(negative){
                    x87_load64(value);
                    __asm { fchs }
                    value=x87_pop64();
                }
            }
            x87_store64(value,result);
        }
        if(inexact)pharaoh_crt_x87_generate_exceptions(0x10u);
        remaining&=~2u;
    }
    if((exceptions&0x10u)&&(control&0x20u)){
        pharaoh_crt_x87_generate_exceptions(0x20u);remaining&=~0x10u;
    }
    return remaining==0;
}
