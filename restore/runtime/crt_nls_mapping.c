#include "crt_nls_mapping.h"
#include <malloc.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
/* Existing complete bounded-byte-count algorithm. It also probes the byte at
   the exhausted limit, as native56AE93 does; do not silently use strnlen. */
extern int32_t FUN_0056ae93(const char *text,int32_t limit);

int32_t restore_crt_map_multibyte_0056947e(volatile struct RestoreCrtMapArguments *a)
{
    uint32_t backend=U(0x12666e0u);
    if(backend==0){
        if(LCMapStringW(0,0x100,(LPCWSTR)(uintptr_t)0x570b98u,1,0,0))backend=1;
        else if(LCMapStringA(0,0x100,(LPCSTR)(uintptr_t)0x570b94u,1,0,0))backend=2;
        else return 0;
        U(0x12666e0u)=backend;
    }
    if(a->source_count>0)a->source_count=FUN_0056ae93(a->source,a->source_count);
    backend=U(0x12666e0u);
    if(backend==2)
        return LCMapStringA(a->locale,a->flags,a->source,a->source_count,
                            (LPSTR)a->destination,a->destination_count);
    if(backend==1){
        int32_t source_wide_count,mapped_count,converted;
        uint32_t allocation;WCHAR *source_wide,*mapped_wide;
        if(a->codepage==0)a->codepage=U(0x126649cu);
        source_wide_count=MultiByteToWideChar(a->codepage,a->strict!=0?9u:1u,
                                             a->source,a->source_count,0,0);
        if(source_wide_count==0)return 0;
        allocation=((uint32_t)source_wide_count*2u+3u)&0xfffffffcu;
        /* Native scope0 contains only this allocation. Filter56958E returns1;
           handler569592 restores the prior ESP and clears the buffer pointer. */
        __try{source_wide=(WCHAR *)_alloca(allocation);}
        __except(EXCEPTION_EXECUTE_HANDLER){source_wide=0;}
        if(source_wide==0)return 0;
        converted=MultiByteToWideChar(a->codepage,1,a->source,a->source_count,
                                     source_wide,source_wide_count);
        if(converted==0)return 0;
        /* Native retains the first required count, even if the second API
           reports a different nonzero count. No zero-fill is added. */
        mapped_count=LCMapStringW(a->locale,a->flags,source_wide,source_wide_count,0,0);
        if(mapped_count==0)return 0;
        if(a->flags&0x400u){
            if(a->destination_count==0)return mapped_count;
            if(mapped_count>a->destination_count)return 0;
            if(!LCMapStringW(a->locale,a->flags,source_wide,source_wide_count,
                             (LPWSTR)a->destination,a->destination_count))return 0;
            return mapped_count;
        }
        allocation=((uint32_t)mapped_count*2u+3u)&0xfffffffcu;
        /* Native scope1 is independent. Its failure retains source_wide. */
        __try{mapped_wide=(WCHAR *)_alloca(allocation);}
        __except(EXCEPTION_EXECUTE_HANDLER){mapped_wide=0;}
        if(mapped_wide==0)return 0;
        if(!LCMapStringW(a->locale,a->flags,source_wide,source_wide_count,
                         mapped_wide,mapped_count))return 0;
        return WideCharToMultiByte(a->codepage,0x220,mapped_wide,mapped_count,
            a->destination_count==0?0:(LPSTR)a->destination,a->destination_count==0?0:a->destination_count,0,0);
    }
    return 0;
}

/* CPINFO's native UINT MaxCharSize and unsigned BYTE lead ranges retain their
   separate widths. Win32 supplies terminated ranges; malformed unterminated
   provider data retains its native unbounded access rather than a new fallback. */
static int is_lead_byte(const CPINFO *info,LPCSTR text)
{
    const BYTE *range=info->LeadByte;
    if(info->MaxCharSize<2u)return 0;
    while(range[0]!=0&&range[1]!=0){
        BYTE value=*(const volatile BYTE *)(const void *)text;
        if(value>=range[0]&&value<=range[1])return 1;
        range+=2;
    }
    return 0;
}

int32_t restore_crt_compare_multibyte_0056ac16(volatile struct RestoreCrtCompareArguments *a)
{
    uint32_t backend=U(0x126670cu);int32_t first_count;
    if(backend==0){
        if(CompareStringW(0,0,(LPCWSTR)(uintptr_t)0x570b98u,1,
                            (LPCWSTR)(uintptr_t)0x570b98u,1))backend=1;
        else if(CompareStringA(0,0,(LPCSTR)(uintptr_t)0x570b94u,1,
                                 (LPCSTR)(uintptr_t)0x570b94u,1))backend=2;
        else return 0;
        U(0x126670cu)=backend;
    }
    first_count=a->first_count;
    if(first_count>0){first_count=FUN_0056ae93(a->first,first_count);a->first_count=first_count;}
    if(a->second_count>0)a->second_count=FUN_0056ae93(a->second,a->second_count);
    backend=U(0x126670cu);
    if(backend==2)
        return CompareStringA(a->locale,a->flags,a->first,first_count,a->second,a->second_count);
    if(backend==1){
        int32_t first_wide_count,second_wide_count;uint32_t allocation;
        WCHAR *first_wide,*second_wide;
        if(a->codepage==0)a->codepage=U(0x126649cu);
        if(first_count==0||a->second_count==0){
            CPINFO info;
            if(first_count==a->second_count)return 2;
            if(a->second_count>1)return 1;
            if(first_count>1)return 3;
            if(!GetCPInfo(a->codepage,&info))return 0;
            if(first_count>0)return is_lead_byte(&info,a->first)?2:3;
            if(a->second_count>0)return is_lead_byte(&info,a->second)?2:1;
            /* A zero/-1 pair reaches conversion, rather than inventing a
               generic empty-string answer that native does not return. */
        }
        first_wide_count=MultiByteToWideChar(a->codepage,9,a->first,first_count,0,0);
        if(first_wide_count==0)return 0;
        allocation=((uint32_t)first_wide_count*2u+3u)&0xfffffffcu;
        __try{first_wide=(WCHAR *)_alloca(allocation);}
        __except(EXCEPTION_EXECUTE_HANDLER){first_wide=0;}
        if(first_wide==0)return 0;
        if(!MultiByteToWideChar(a->codepage,1,a->first,first_count,
                                first_wide,first_wide_count))return 0;
        second_wide_count=MultiByteToWideChar(a->codepage,9,a->second,a->second_count,0,0);
        if(second_wide_count==0)return 0;
        allocation=((uint32_t)second_wide_count*2u+3u)&0xfffffffcu;
        __try{second_wide=(WCHAR *)_alloca(allocation);}
        __except(EXCEPTION_EXECUTE_HANDLER){second_wide=0;}
        if(second_wide==0)return 0;
        if(!MultiByteToWideChar(a->codepage,1,a->second,a->second_count,
                                second_wide,second_wide_count))return 0;
        return CompareStringW(a->locale,a->flags,first_wide,first_wide_count,
                               second_wide,second_wide_count);
    }
    return 0;
}
