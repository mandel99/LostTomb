#include "crt_runtime_tail.h"

uint32_t FUN_00565f66(uint32_t character,uint32_t mask)
{
    /* Native writes its WORD classification into the upper half of this
       original argument slot only on the multibyte/NLS route. */
    return restore_crt_classify_character(character,&mask,(uint16_t *)(void *)((uint8_t *)&character+2));
}
BOOL FUN_00569335(DWORD type,LPCSTR text,int32_t count,LPWORD output,
                  UINT codepage,LCID locale,int32_t strict)
{return restore_crt_get_string_type(type,text,count,output,&codepage,locale,strict);}

__declspec(naked) void FUN_00567500(void)
{
    __asm {
        push ebp
        mov ebp,esp
        push ebx
        lea ebx,[ebp+8]
        push ebx
        push dword ptr[ebp+28]
        push dword ptr[ebp+24]
        push dword ptr[ebp+20]
        push dword ptr[ebp+16]
        push edx
        push ecx
        push eax
        call restore_crt_two_arg_error_live
        add esp,32
        pop ebx
        mov esp,ebp
        pop ebp
        ret
    }
}

/* Only physical stack/register restoration is assembly. The C owner computes
   addresses and performs every page/final probe, including duplicate final
   probes for exact page multiples. No probed stack data is promoted into a C
   logical input/result merely to reproduce unused TEST flags. */
__declspec(naked) void FUN_00561f30(void)
{
    __asm {
        push ecx
        push edx
        push ebp
        mov ebp,esp
        lea ecx,[ebp+16]
        push eax
        push ecx
        call restore_crt_probe_stack
        add esp,8
        mov edx,[ebp+12]
        mov ecx,[ebp+8]
        mov esp,eax
        mov eax,edx
        mov edx,[ebp+4]
        mov ebp,[ebp]
        push eax
        ret
    }
}
__declspec(naked) void FUN_00563dd6(void)
{
    __asm {
        push ebx
        push edi
        push esi
        push ebp
        call restore_crt_catch_cleanup
        add esp,16
        ret
    }
}
uint32_t __cdecl FUN_005630e0(int32_t error){return restore_crt_fatal_error(error);}
void FUN_00567dcc(void){restore_crt_error_banner();}
void FUN_00567e05(uint32_t error){restore_crt_report_error(error,&error);}
int32_t FUN_00569c11(const char *text,const char *caption,uint32_t flags)
{return restore_crt_message_box(text,caption,flags);}
