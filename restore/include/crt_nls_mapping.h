#ifndef RESTORE_CRT_NLS_MAPPING_H
#define RESTORE_CRT_NLS_MAPPING_H
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <stdint.h>
#include <stddef.h>

/* These records are the actual DWORD parameter slots. Native length trimming
   and default-codepage selection write them; they are not invented registers
   or padding inputs. Named C callers may supply an ordinary explicit record. */
struct RestoreCrtMapArguments {
    LCID locale;
    DWORD flags;
    LPCSTR source;
    int32_t source_count;
    void *destination;
    int32_t destination_count;
    UINT codepage;
    int32_t strict;
};
struct RestoreCrtCompareArguments {
    LCID locale;
    DWORD flags;
    LPCSTR first;
    int32_t first_count;
    LPCSTR second;
    int32_t second_count;
    UINT codepage;
};
typedef char RestoreCrtMapLayout[(sizeof(struct RestoreCrtMapArguments)==32&&
    offsetof(struct RestoreCrtMapArguments,source_count)==12&&
    offsetof(struct RestoreCrtMapArguments,codepage)==24)?1:-1];
typedef char RestoreCrtCompareLayout[(sizeof(struct RestoreCrtCompareArguments)==28&&
    offsetof(struct RestoreCrtCompareArguments,first_count)==12&&
    offsetof(struct RestoreCrtCompareArguments,second_count)==20&&
    offsetof(struct RestoreCrtCompareArguments,codepage)==24)?1:-1];

int32_t restore_crt_map_multibyte_0056947e(volatile struct RestoreCrtMapArguments *arguments);
int32_t restore_crt_compare_multibyte_0056ac16(volatile struct RestoreCrtCompareArguments *arguments);
int32_t __cdecl FUN_0056947e(LCID locale,DWORD flags,LPCSTR source,int32_t source_count,
                           void *destination,int32_t destination_count,UINT codepage,int32_t strict);
int32_t __cdecl FUN_0056ac16(LCID locale,DWORD flags,LPCSTR first,int32_t first_count,
                           LPCSTR second,int32_t second_count,UINT codepage);
#endif
