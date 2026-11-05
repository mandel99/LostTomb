#include "crt_nls_mapping.h"
/* Adapt actual parameter-slot records; the C owner performs all decisions,
   writes and Win32 operations. Both original entries use cdecl RET0. */
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) int32_t __cdecl FUN_0056947e(LCID locale,DWORD flags,LPCSTR source,
    int32_t source_count,void *destination,int32_t destination_count,UINT codepage,int32_t strict)
{
    __asm {
        lea eax,[esp+4]
        push eax
        call restore_crt_map_multibyte_0056947e
        add esp,4
        ret
    }
}
__declspec(naked) int32_t __cdecl FUN_0056ac16(LCID locale,DWORD flags,LPCSTR first,
    int32_t first_count,LPCSTR second,int32_t second_count,UINT codepage)
{
    __asm {
        lea eax,[esp+4]
        push eax
        call restore_crt_compare_multibyte_0056ac16
        add esp,4
        ret
    }
}
#pragma warning(pop)
