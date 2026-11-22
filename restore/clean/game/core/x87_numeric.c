#include "pharaoh/core/x87_numeric.h"

#if !defined(_MSC_VER) || !defined(_M_IX86)
#error These inherited-precision x87 owners require the configured MSVC Win32 target.
#endif

/* Exact binary64/32 constants read from pinned Ghidra memory. */
const double pharaoh_library_progress_step=0.047619047619047616;
const double pharaoh_abu_simbel_progress_step=0.017857142857142856;
static const double percent_scale=100.0;
static const float chamber_scale=4.0f;
static const float chamber_percent=100.0f;
static const double completed_threshold=99.900000000000006;

/* Complete typed reuse of CampaignScoreNumeric's conversion. Native0056133C
 * uses FSTCW/WAIT, RC=toward zero, FISTP qword, then restores the actual CW.
 * No binary64 intermediate or emulated register frame is introduced. */
__declspec(noinline) int64_t pharaoh_x87_truncate80(const PharaohFloat80 *value)
{
    uint16_t original,truncate;
    int64_t result;
    __asm {
        mov eax,value
        fld tbyte ptr[eax]
        fstcw original
        fwait
        mov ax,original
        or ah,0ch
        mov truncate,ax
        fldcw truncate
        fistp qword ptr result
        fldcw original
    }
    return result;
}

__declspec(noinline) int32_t pharaoh_x87_scaled_progress(
    int32_t stage, const double *step)
{
    PharaohFloat80 value;
    __asm {
        fild stage
        mov eax,step
        fmul qword ptr[eax]
        fmul qword ptr percent_scale
        fstp tbyte ptr value
    }
    return (int32_t)(uint32_t)pharaoh_x87_truncate80(&value);
}

/* Exact existing MonumentProgressPercent sequence; the native 99.9 comparison
 * precedes truncation. In particular, it is not an integer division formula. */
__declspec(noinline) int32_t pharaoh_x87_tomb_percent(int32_t sum,int32_t count)
{
    PharaohFloat80 value;
    uint16_t status;
    __asm {
        fild count
        fmul dword ptr chamber_scale
        fdivr dword ptr chamber_percent
        fimul sum
        fcom qword ptr completed_threshold
        fnstsw status
    }
    if (!(status&0x4100u)) {
        __asm {fstp st(0)}
        return 100;
    }
    __asm {fstp tbyte ptr value}
    return (int32_t)(uint32_t)pharaoh_x87_truncate80(&value);
}

/* Native004683FF..00468410: integer inputs remain extended until __ftol.
 * A zero divisor keeps the original masked x87 invalid/divide behavior. */
__declspec(noinline) int32_t pharaoh_x87_integer_ratio(
    int32_t numerator, int32_t denominator)
{
    PharaohFloat80 value;
    __asm {
        fild numerator
        fidiv denominator
        fstp tbyte ptr value
    }
    return (int32_t)(uint32_t)pharaoh_x87_truncate80(&value);
}

static const double fertility_scale=12.0,fertility_divisor=96.0;

/* Native00476B0B..00476B38: retain FCOM's condition codes as well as __ftol. */
__declspec(noinline) int32_t pharaoh_x87_floodplain_fertility(uint32_t fertility)
{
    PharaohFloat80 value;
    uint16_t status;
    __asm {
        fild fertility
        fcom qword ptr percent_scale
        fnstsw status
    }
    if (!(status&0x4100u)) {
        __asm {fstp tbyte ptr value}
    } else {
        __asm {
            fld qword ptr fertility_scale
            fdiv qword ptr fertility_divisor
            fmul st(0),st(1)
            fstp tbyte ptr value
            fstp st(0)
        }
    }
    return (int32_t)(uint32_t)pharaoh_x87_truncate80(&value);
}

__declspec(noinline) int32_t pharaoh_x87_i32_multiply_pair(
    int32_t input,const double *first,const double *second)
{
    PharaohFloat80 value;
    __asm {
        fild input
        mov eax,first
        fmul qword ptr[eax]
        mov eax,second
        fmul qword ptr[eax]
        fstp tbyte ptr value
    }
    return (int32_t)(uint32_t)pharaoh_x87_truncate80(&value);
}

__declspec(noinline) int32_t pharaoh_x87_i32_divide_multiply(
    int32_t input,const double *divisor,const double *scale)
{
    PharaohFloat80 value;
    __asm {
        fild input
        mov eax,divisor
        fdiv qword ptr[eax]
        mov eax,scale
        fmul qword ptr[eax]
        fstp tbyte ptr value
    }
    return (int32_t)(uint32_t)pharaoh_x87_truncate80(&value);
}

/* Native486EFC..486F0A: FIDIV keeps the integer denominator's exact width. */
__declspec(noinline) int32_t pharaoh_x87_scaled_integer_ratio(
    int32_t numerator,int32_t denominator,const double *scale)
{
    PharaohFloat80 value;
    __asm {
        fild numerator
        fidiv denominator
        mov eax,scale
        fmul qword ptr[eax]
        fstp tbyte ptr value
    }
    return (int32_t)(uint32_t)pharaoh_x87_truncate80(&value);
}
