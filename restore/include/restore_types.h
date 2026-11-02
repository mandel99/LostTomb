#ifndef RESTORE_TYPES_H
#define RESTORE_TYPES_H
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned int undefined3;
typedef unsigned int uint3;
typedef int int3;
typedef unsigned __int64 undefined8;
typedef unsigned __int64 undefined6;
typedef unsigned __int64 ulonglong;
typedef __int64 longlong;
typedef long double float10;
typedef unsigned int pointer32;
typedef unsigned char bool;
typedef int code();
extern int restore_interrupt3();
extern unsigned int restore_unresolved_return(unsigned int address);
extern unsigned int restore_pointer32_checked(const void *value,unsigned int origin);
typedef struct { unsigned int value:24; } restore_u24;
#define RESTORE_SLICE1(v,o) (*(unsigned char *)((unsigned char *)&(v)+(o)))
#define RESTORE_SLICE2(v,o) (*(unsigned short *)((unsigned char *)&(v)+(o)))
#define RESTORE_SLICE3(v,o) (((restore_u24 *)((unsigned char *)&(v)+(o)))->value)
#define RESTORE_SLICE4(v,o) (*(unsigned int *)((unsigned char *)&(v)+(o)))
#define RESTORE_SLICE8(v,o) (*(unsigned __int64 *)((unsigned char *)&(v)+(o)))
#define true 1
#define false 0

#include <windows.h>
#include <ddraw.h>
#include <mmsystem.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
/* Original x86 FS:[0] refers to the current thread's exception chain. */
#define ExceptionList (((NT_TIB *)NtCurrentTeb())->ExceptionList)
#include "media_bindings.h"
typedef unsigned char undefined;
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned short word;
typedef unsigned int dword;
typedef unsigned short wchar16;
typedef unsigned char CRect;
typedef unsigned char CAtlWinModule;
typedef RECT tagRECT;
typedef MSG tagMSG;
typedef POINT tagPOINT;
typedef SIZE tagSIZE;
typedef struct _WIN32_FIND_DATAA _WIN32_FIND_DATAA;
typedef OSVERSIONINFOA _OSVERSIONINFOA;
typedef TIME_ZONE_INFORMATION _TIME_ZONE_INFORMATION;
typedef SYSTEMTIME _SYSTEMTIME;
typedef STARTUPINFOA _STARTUPINFOA;
typedef SECURITY_ATTRIBUTES _SECURITY_ATTRIBUTES;
typedef EXCEPTION_POINTERS _EXCEPTION_POINTERS;
typedef CPINFO _cpinfo;
/* Original pre-UCRT stream layout; keep it separate from the host CRT. */
typedef struct {
    char *_ptr;
    int _cnt;
    char *_base;
    int _flag;
    int _file;
    int _charbuf;
    int _bufsiz;
    char *_tmpfname;
} restore_legacy_FILE;

#include "restore_intrinsics.h"
#endif
