#include "crt_asin_math.h"

/* Entry562D00 performs the physical ST0->binary64 argument bridge. All
   classification/decisions and arithmetic live in the C owner/primitives. */
__declspec(naked) void FUN_00562d00(void)
{
    __asm {
        sub esp,8
        fst qword ptr[esp]
        call FUN_005675e8
        call FUN_00562d1d
        add esp,8
        ret
    }
}
__declspec(naked) void FUN_00562d1d(void)
{
    __asm {
        push ebp
        mov ebp,esp
        pushfd
        push edx
        push eax
        push dword ptr[ebp-8]
        push dword ptr[ebp+12]
        push dword ptr[ebp+8]
        push dword ptr[ebp-4]
        push dword ptr[ebp-12]
        call restore_crt_asin_core_live
        add esp,20
        mov edx,eax
        mov esp,ebp
        pop ebp
        ret
    }
}
__declspec(naked) void FUN_00567517(void)
{
    __asm {
        push ebp
        mov ebp,esp
        push ebx
        lea ebx,[ebp+8]
        push ebx
        push dword ptr[ebp+20]
        push dword ptr[ebp+16]
        push edx
        push ecx
        push eax
        call restore_crt_one_arg_error_live
        add esp,24
        pop ebx
        mov esp,ebp
        pop ebp
        ret
    }
}
__declspec(naked) void FUN_00567575(void)
{
    __asm {
        push eax
        push ecx
        lea eax,[esp+12]
        push eax
        call restore_crt_normalize_math_live
        add esp,4
        mov edx,eax
        test edx,edx
        pop ecx
        pop eax
        ret
    }
}
__declspec(naked) void FUN_0056758c(void)
{
    __asm {
        push ecx
        push edx
        push eax
        push eax
        call restore_crt_quiet_nan_live
        add esp,4
        test dword ptr[esp],80000h
        lea esp,[esp+4]
        pop edx
        pop ecx
        ret
    }
}
__declspec(naked) void FUN_005675e8(void)
{
    __asm {
        push ecx
        push edx
        push dword ptr[esp+16]
        call restore_crt_classify_high
        add esp,4
        mov ecx,eax
        and ecx,7ff00000h
        cmp ecx,7ff00000h
        pop edx
        pop ecx
        ret
    }
}
/* Tail56760B enters without a return PC above its saved-CW scratch DWORD.
   Preserve that entire DWORD for native POP EDX, including its upper WORD. */
__declspec(naked) void FUN_0056760b(void)
{
    __asm {
        push ebp
        mov ebp,esp
        push ecx
        push edx
        push dword ptr[ebp+16]
        push dword ptr[ebp+12]
        movzx eax,word ptr[ebp+4]
        push eax
        call restore_crt_math_finish_live
        add esp,20
        mov edx,[ebp+4]
        mov esp,ebp
        pop ebp
        add esp,4
        ret
    }
}

void FUN_0056971f(uint32_t op,struct restore_crt_math_exception *record,const uint16_t *control)
{restore_crt_math_dispatch(op,record,control);}
void FUN_00567f58(struct restore_crt_ieee_record *record,uint32_t *control,uint32_t exceptions,
                  uint32_t operation,const uint64_t *operand,uint64_t *result)
{restore_crt_raise_ieee(record,control,exceptions,operation,operand,result);}
uint32_t FUN_0056820b(uint32_t exceptions,uint64_t *result,uint32_t control)
{return restore_crt_handle_masked_error(exceptions,result,control);}
void FUN_00568448(uint64_t value,int32_t exponent)
{restore_crt_set_exponent_live(value,exponent);}
void FUN_00568471(uint64_t value,int32_t *exponent)
{restore_crt_frexp_live(value,exponent);}
int32_t FUN_00568532(void){return restore_crt_status_wait();}
int32_t FUN_00568540(void){return restore_crt_clear_status();}
/* Native56854F's PUSH ECX supplies the upper WORD of its FSTCW scratch.
   Retain that ACTUAL live input when reproducing its caller-mask-slot write. */
#pragma warning(suppress:4100)
__declspec(naked) int32_t FUN_0056854f(uint32_t value,uint32_t mask)
{
    __asm {
        push ebp
        mov ebp,esp
        push edx
        lea edx,[ebp+12]
        push edx
        push ecx
        push dword ptr[ebp+12]
        push dword ptr[ebp+8]
        call restore_crt_control_word_slot
        add esp,16
        mov ecx,[ebp+12]
        test ecx,ecx
        pop edx
        mov esp,ebp
        pop ebp
        ret
    }
}
void FUN_00568572(uint32_t exceptions){restore_crt_generate_exceptions(exceptions);}
