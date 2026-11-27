#include "pharaoh/runtime/crt_math_dispatch.h"
#include "pharaoh/runtime/crt_ieee_raise.h"
#include "pharaoh/runtime/crt_x87_math.h"
#include "pharaoh/runtime/crt_error_translation.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohCrtMathExceptionRecord)==32 &&
    offsetof(PharaohCrtMathExceptionRecord,arg1)==8 &&
    offsetof(PharaohCrtMathExceptionRecord,arg2)==16 &&
    offsetof(PharaohCrtMathExceptionRecord,result)==24,"Native math record");

/* Whole0056971F, reused from current/old CrtAsinMath C. */
void pharaoh_crt_math_dispatch(uint32_t operation,
    PharaohCrtMathExceptionRecord *record, const uint16_t *saved_control,
    const volatile uint32_t *math_hook_mode, int32_t *error_code)
{
    uint32_t control=*saved_control,exceptions=0;
    switch(record->type) {
    case 1:case 5:exceptions=8;break;
    case 2:exceptions=4;break;
    case 3:exceptions=0x11;break;
    case 4:exceptions=0x12;break;
    case 7:record->type=1;break;
    case 8:exceptions=0x10;break;
    default:break;
    }
    if(exceptions && !pharaoh_crt_x87_handle_masked_error(
            exceptions,&record->result,control)) {
        PharaohCrtIeeeRecord ieee;
        pharaoh_crt_ieee_prepare_second_operand(&ieee,operation,&record->arg2);
        pharaoh_crt_ieee_raise(&ieee,&control,exceptions,operation,
            &record->arg1,&record->result);
    }
    (void)pharaoh_crt_x87_control_word(control,0xffffu);
    if(record->type!=8 && *math_hook_mode==0 &&
            pharaoh_crt_math_hook_disabled()!=0)return;
    pharaoh_crt_set_math_error(record->type,error_code);
}


/* Whole00567517. Native writes type, pops live result, then name/operand.
 * Unary arg2 remains the caller-owned record's unspecified bytes. */
__declspec(noinline) void pharaoh_crt_math_one_arg_error_live(int32_t type, const char *name,
    uint32_t operation, uint64_t operand, const uint16_t *saved_control,
    const volatile uint32_t *math_hook_mode, int32_t *error_code)
{
    PharaohCrtMathExceptionRecord record;
    uint64_t *result=&record.result;
    record.type=type;
    __asm {
        mov eax,result
        fstp qword ptr[eax]
    }
    record.name=name;
    record.arg1=operand;
    pharaoh_crt_math_dispatch(operation,&record,saved_control,
        math_hook_mode,error_code);
    __asm {
        mov eax,result
        fld qword ptr[eax]
    }
    if(*saved_control!=0x27fu) {
        __asm {
            mov eax,saved_control
            fldcw word ptr[eax]
        }
    }
}

/* Whole0056760B logical FP/CW tail. Native shared-stack POP/RET is folded
 * into its typed C callers; no scratch high bits or GPR result are invented. */
__declspec(noinline) void pharaoh_crt_math_finish_live(const uint16_t *saved_control,
    uint64_t operand, uint32_t operation, const char *name,
    const volatile uint32_t *math_hook_mode, int32_t *error_code)
{
    const uint16_t control=*saved_control;
    if(control==0x27fu)return;
    if((control&0x20u)!=0 &&
            ((uint32_t)pharaoh_crt_x87_status_wait()&0x20u)!=0) {
        pharaoh_crt_math_one_arg_error_live(8,name,operation,operand,
            saved_control,math_hook_mode,error_code);
        return;
    }
    __asm {
        mov eax,saved_control
        fldcw word ptr[eax]
    }
}
