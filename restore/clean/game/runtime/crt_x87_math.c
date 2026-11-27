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
