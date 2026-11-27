#include "pharaoh/runtime/crt_asin.h"
#include "pharaoh/runtime/crt_numeric_bits.h"
#include "pharaoh/runtime/crt_math_dispatch.h"
#include "pharaoh/runtime/crt_x87_math.h"

#if !defined(_MSC_VER) || !defined(_M_IX86)
#error These inherited-precision x87 entries require MSVC Win32.
#endif

/* Exact Ghidra bytes6050A0/6050AA/604960; never host long-double constants. */
static const uint8_t asin_indefinite80[10]=
    {0,0,0,0,0,0,0,0xc0,0xff,0xff};
static const uint8_t asin_half_pi80[10]=
    {0x35,0xc2,0x68,0x21,0xa2,0xda,0x0f,0xc9,0xff,0x3f};
static const char asin_name[]="asin";

/* Reused live primitives from current/old CrtAsinMath C. */
static __declspec(noinline) void replace_indefinite_live(void)
{
    __asm {
        fstp st(0)
        fld tbyte ptr asin_indefinite80
    }
}

static __declspec(noinline) void replace_half_pi_live(uint32_t negative)
{
    __asm {
        fstp st(0)
        fld tbyte ptr asin_half_pi80
    }
    if(negative) { __asm { fchs } }
}

static __declspec(noinline) void asin_interior_live(void)
{
    __asm {
        fld1
        fadd st(0),st(1)
        fld1
        fsub st(0),st(2)
        fmulp st(1),st(0)
        fsqrt
        fpatan
    }
}

/* Native5675FE is the second body range of562D1D. */
static __declspec(noinline) void restore_control_if_needed(
    const PharaohCrtX87ControlSlot *slot)
{
    if(slot->original!=0x27fu) {
        __asm {
            mov eax,slot
            fldcw word ptr[eax]
        }
    }
}

static void error_or_restore_live(int32_t type,uint64_t operand,
    const PharaohCrtX87ControlSlot *slot,
    const PharaohCrtMathEnvironment *environment)
{
    if(*environment->bypass_errors==0)
        pharaoh_crt_math_one_arg_error_live(type,asin_name,0xeu,operand,
            &slot->original,environment->hook_mode,environment->error_code);
    else restore_control_if_needed(slot);
}

/* Whole00562D1D. Its sole observed producer5675E8's CMP sets ZF exactly for
 * exponent7FF. Derive that condition from the real classification result;
 * unobserved flags and scratch upper bits are not extra C API inputs. */
__declspec(noinline) void pharaoh_crt_asin_core_live(uint32_t classification,
    uint64_t operand,const PharaohCrtMathEnvironment *environment)
{
    PharaohCrtX87ControlSlot slot;
    __asm {
        lea eax,slot
        fstcw word ptr[eax]
    }
    const uint32_t high=(uint32_t)(operand>>32),low=(uint32_t)operand;
    if((classification&0x7ff00000u)==0x7ff00000u) {
        int32_t type;
        if((classification&0xfffffu)!=0 || low!=0)
            type=(int32_t)pharaoh_crt_x87_quiet_nan_live(classification);
        else { replace_indefinite_live();type=1; }
        error_or_restore_live(type,operand,&slot,environment);
        return;
    }
    if(slot.original!=0x27fu)
        (void)pharaoh_crt_x87_normalize_control_live(&slot);
    if(classification<0x3ff00000u)asin_interior_live();
    else if(classification==0x3ff00000u && (high&0xfffffu)==0 && low==0)
        replace_half_pi_live(high&0x80000000u);
    else {
        replace_indefinite_live();
        error_or_restore_live(1,operand,&slot,environment);
        return;
    }
    if(*environment->bypass_errors==0)
        pharaoh_crt_math_finish_live(&slot.original,operand,0xeu,asin_name,
            environment->hook_mode,environment->error_code);
    else restore_control_if_needed(&slot);
}

/* Whole00562D00. FST snapshots binary64 while retaining actual extended ST0. */
__declspec(noinline) void pharaoh_crt_asin_live(
    const PharaohCrtMathEnvironment *environment)
{
    uint64_t operand;
    __asm { fst qword ptr operand }
    const uint32_t classification=pharaoh_crt_double_exponent_class(
        (uint32_t)(operand>>32));
    pharaoh_crt_asin_core_live(classification,operand,environment);
}
