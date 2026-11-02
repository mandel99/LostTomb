/* Transitional host: original x86 fallback plus independently verified C hooks. */
#include "pe_image.h"
#include <shellapi.h>
#include <stdint.h>
#include <string.h>
#include <share.h>
#include <stdlib.h>
#include "record_geometry.h"
#include "memory_strings.h"
#include "core_helpers.h"
#include "ring_queue.h"

extern unsigned int FUN_0041ba00(int);
extern unsigned int FUN_0041bad0(unsigned int);
extern unsigned int FUN_00451bc0(int);
extern unsigned int FUN_0045b490(int);
extern char FUN_0045bb30(int,int);
extern void FUN_004e5120(void *,int,int,int,int);
extern int restore_verify_city_history(FILE *);

static RestoreImage game;
static FILE *trace;
static char command_line[MAX_PATH+4];
#define RESTORE_HOOK_COUNT 23
static LONG calls[RESTORE_HOOK_COUNT];
static const DWORD hook_addresses[RESTORE_HOOK_COUNT]={0x41ba00,0x41bad0,0x451bc0,0x45b490,0x45bb30,0x4e5120,
    0x4e5140,0x4e5190,0x4e52b0,0x562240,0x5631b0,0x563240,0x563380,
    0x56233e,0x562348,0x5632a0,0x563400,
    0x4f6520,0x4f6560,0x4f6570,0x4f65b0,0x4f65f0,0x4f6630};
#include "hook_signatures.h"
#include "experimental_hooks.h"
typedef char RestoreHookSignatureCount[(sizeof(hook_original_prefix)/sizeof(hook_original_prefix[0])==RESTORE_HOOK_COUNT)?1:-1];
typedef char RestoreExperimentalHookSignatureCount[(sizeof(experimental_hook_prefix)/sizeof(experimental_hook_prefix[0])==RESTORE_EXPERIMENTAL_HOOK_COUNT)?1:-1];
static int experimental_hooks_installed;
#define RESTORE_DEFAULT_EXPERIMENTAL_HOOK_LIMIT 225
static unsigned int experimental_hook_limit=RESTORE_DEFAULT_EXPERIMENTAL_HOOK_LIMIT;

/* These recovered functions compile, but are not yet safe entry-point replacements.
   Keep their original implementations active without blocking later candidates. */
static const DWORD experimental_hook_quarantine[]={0x004d32d0,0x004d3d00};

static int experimental_hook_is_quarantined(DWORD address)
{
    unsigned int i;
    for (i=0;i<sizeof(experimental_hook_quarantine)/sizeof(experimental_hook_quarantine[0]);i++) {
        if (experimental_hook_quarantine[i]==address) return 1;
    }
    return 0;
}

static unsigned int experimental_hook_active_count(void)
{
    unsigned int i,count=0;
    for (i=0;i<experimental_hook_limit;i++) {
        if (!experimental_hook_is_quarantined(experimental_hook_addresses[i])) count++;
    }
    return count;
}

/* Apply the game's existing 16-bit compatibility requirement to this launch
   only. This changes neither registry layers nor desktop display settings. */
static int compatibility_child(UINT *exit_code)
{
    HDC display;
    int depth;
    char marker[8];
    wchar_t executable[MAX_PATH],command[32768];
    STARTUPINFOW startup={sizeof(startup)};
    PROCESS_INFORMATION process;
    const wchar_t *arguments=GetCommandLineW();
    if (wcsstr(arguments,L"--inspect") || wcsstr(arguments,L"--self-test") ||
        wcsstr(arguments,L"--hook-guard-test") || wcsstr(arguments,L"--experimental-hook-test") ||
        wcsstr(arguments,L"--no-compat")) return 0;
    if (GetEnvironmentVariableA("RESTORE_COMPAT_CHILD",marker,sizeof(marker))) return 0;
    display=GetDC(NULL);
    depth=display ? GetDeviceCaps(display,BITSPIXEL)*GetDeviceCaps(display,PLANES) : 32;
    if (display) ReleaseDC(NULL,display);
    if (depth==16) return 0;
    GetModuleFileNameW(NULL,executable,MAX_PATH);
    if (wcscpy_s(command,sizeof(command)/sizeof(command[0]),arguments)) { *exit_code=6; return 1; }
    SetEnvironmentVariableA("__COMPAT_LAYER","16BITCOLOR HIGHDPIAWARE DWM8And16BitMitigation");
    SetEnvironmentVariableA("RESTORE_COMPAT_CHILD","1");
    startup.dwFlags=STARTF_USESHOWWINDOW;
    startup.wShowWindow=SW_SHOWNORMAL;
    if (!CreateProcessW(executable,command,NULL,NULL,FALSE,0,NULL,NULL,&startup,&process)) {
        *exit_code=6;
        return 1;
    }
    CloseHandle(process.hThread);
    WaitForSingleObject(process.hProcess,INFINITE);
    GetExitCodeProcess(process.hProcess,exit_code);
    CloseHandle(process.hProcess);
    return 1;
}

static void record_call(int index)
{
    LONG count=InterlockedIncrement(calls+index);
    if (count==1) {
        fprintf(trace,"First original-to-C call: %08lx\n",hook_addresses[index]);
        fflush(trace);
    }
}

static unsigned int __stdcall c_0041ba00(int x) { record_call(0); return FUN_0041ba00(x); }
static unsigned int __stdcall c_0041bad0(unsigned int x) { record_call(1); return FUN_0041bad0(x); }
static unsigned int __cdecl c_00451bc0(int x) { record_call(2); return FUN_00451bc0(x); }
static unsigned int __cdecl c_0045b490(int x) { record_call(3); return FUN_0045b490(x); }
static unsigned int __cdecl c_0045bb30(int x,int y) { record_call(4); return (unsigned char)FUN_0045bb30(x,y); }
static void *__fastcall c_004e5120(void *object,void *unused,int left,int top,int right,int bottom)
{
    (void)unused;
    record_call(5);
    FUN_004e5120(object,left,top,right,bottom);
    return object;
}

static void *__fastcall c_004e5140(void *destination,void *unused,const void *source)
{
    (void)unused;
    record_call(6);
    return restore_004e5140(destination,source);
}

static uint32_t __fastcall c_004e5190(void *record,void *unused)
{
    uint32_t step=*(volatile uint32_t *)0x70e45c;
    uint32_t scale=*(volatile uint32_t *)0x70e44c;
    (void)unused;
    record_call(7);
    return restore_004e5190(record,scale,step);
}

static void __stdcall c_004e52b0(void *records,int count)
{
    record_call(8);
    restore_004e52b0(records,count,(volatile uint32_t *)0x70e44c,(volatile uint32_t *)0x70e45c);
}

static char *__cdecl c_00562240(char *destination,const char *source,size_t count)
{
    record_call(9);
    return restore_00562240(destination,source,count);
}

static int __cdecl c_005631b0(const char *left,const char *right)
{
    record_call(10);
    return restore_005631b0(left,right);
}

static void *__cdecl c_00563240(void *destination,int value,size_t count)
{
    record_call(11);
    return restore_00563240(destination,value,count);
}

static size_t __cdecl c_00563380(const char *text)
{
    record_call(12);
    return restore_00563380(text);
}

static uint32_t __cdecl c_0056233e(uint32_t seed)
{
    record_call(13);
    return restore_0056233e(seed);
}

static uint32_t __cdecl c_00562348(void)
{
    record_call(14);
    return restore_00562348();
}

static char *__cdecl c_005632a0(const char *text,const char *pattern)
{
    record_call(15);
    return restore_005632a0(text,pattern);
}

static void *__cdecl c_00563400(void *destination,const void *source,size_t count)
{
    record_call(16);
    return restore_00563400(destination,source,count);
}

static uint32_t __fastcall c_004f6520(void *queue,void *unused)
{
    (void)unused; record_call(17);
    return restore_004f6520(queue);
}

static uint32_t __fastcall c_004f6560(void *queue,void *unused)
{
    (void)unused; record_call(18);
    return restore_004f6560(queue);
}

static uint32_t __fastcall c_004f6570(const void *queue,void *unused,uint32_t value)
{
    (void)unused; record_call(19);
    return restore_004f6570(queue,value);
}

static uint32_t __fastcall c_004f65b0(void *queue,void *unused,const void *value)
{
    (void)unused; record_call(20);
    return restore_004f65b0(queue,value);
}

static uint32_t __fastcall c_004f65f0(const void *queue,void *unused,void *cursor)
{
    (void)unused; record_call(21);
    return restore_004f65f0(queue,cursor);
}

static uint32_t __fastcall c_004f6630(void *queue,void *unused)
{
    (void)unused; record_call(22);
    return restore_004f6630(queue);
}

static void print_counts(void)
{
    unsigned int i;
    fputs("C hook counts:",trace);
    for (i=0;i<RESTORE_HOOK_COUNT;i++) fprintf(trace," %08lx=%ld",hook_addresses[i],calls[i]);
    fputc('\n',trace);
    fflush(trace);
}

static HMODULE WINAPI game_GetModuleHandleA(LPCSTR name)
{
    if (!name || _stricmp(name,"Pharaoh.exe")==0) return (HMODULE)game.base;
    return GetModuleHandleA(name);
}

static DWORD WINAPI game_GetModuleFileNameA(HMODULE module,LPSTR destination,DWORD capacity)
{
    size_t length;
    if (module && module!=(HMODULE)game.base) return GetModuleFileNameA(module,destination,capacity);
    length=strlen(game.filename);
    if (!capacity) return 0;
    strncpy_s(destination,capacity,game.filename,_TRUNCATE);
    if (length>=capacity) { SetLastError(ERROR_INSUFFICIENT_BUFFER); return capacity; }
    return (DWORD)length;
}

static LPSTR WINAPI game_GetCommandLineA(void) { return command_line; }

static void WINAPI game_GetStartupInfoA(LPSTARTUPINFOA info)
{
    GetStartupInfoA(info);
    info->dwFlags |= STARTF_USESHOWWINDOW;
    info->wShowWindow=SW_SHOWNORMAL;
}

static void WINAPI game_ExitProcess(UINT code)
{
    fprintf(trace,"Original application requested ExitProcess(%u).\n",code);
    print_counts();
    ExitProcess(code);
}

static BOOL WINAPI game_TerminateProcess(HANDLE process,UINT code)
{
    fprintf(trace,"Original application requested TerminateProcess(%p, %u).\n",process,code);
    print_counts();
    return TerminateProcess(process,code);
}

static FARPROC import_hook(const char *module,const char *symbol,FARPROC original)
{
    (void)module;
#define HOOK(name) if (strcmp(symbol,#name)==0) return (FARPROC)game_##name
    HOOK(GetModuleHandleA);
    HOOK(GetModuleFileNameA);
    HOOK(GetCommandLineA);
    HOOK(GetStartupInfoA);
    HOOK(ExitProcess);
    HOOK(TerminateProcess);
#undef HOOK
    return original;
}

static LONG WINAPI report_exception(EXCEPTION_POINTERS *exception)
{
    fprintf(trace,"Unhandled exception %08lx at %p (original image offset %08lx).\n",
        exception->ExceptionRecord->ExceptionCode,exception->ExceptionRecord->ExceptionAddress,
        (DWORD)(uintptr_t)exception->ExceptionRecord->ExceptionAddress-0x400000);
    fprintf(trace,"EAX=%08lx EBX=%08lx ECX=%08lx EDX=%08lx ESI=%08lx EDI=%08lx EBP=%08lx ESP=%08lx\n",
        exception->ContextRecord->Eax,exception->ContextRecord->Ebx,exception->ContextRecord->Ecx,
        exception->ContextRecord->Edx,exception->ContextRecord->Esi,exception->ContextRecord->Edi,
        exception->ContextRecord->Ebp,exception->ContextRecord->Esp);
    print_counts();
    return EXCEPTION_EXECUTE_HANDLER;
}

static LONG WINAPI report_vectored_exception(EXCEPTION_POINTERS *exception)
{
    DWORD code=exception->ExceptionRecord->ExceptionCode;
    if (code==EXCEPTION_ACCESS_VIOLATION || code==EXCEPTION_ILLEGAL_INSTRUCTION ||
       code==EXCEPTION_STACK_OVERFLOW || code==EXCEPTION_INT_DIVIDE_BY_ZERO ||
       code==EXCEPTION_FLT_DIVIDE_BY_ZERO) {
        fprintf(trace,"First-chance exception %08lx at %p (original image offset %08lx).\n",
            code,exception->ExceptionRecord->ExceptionAddress,
            (DWORD)(uintptr_t)exception->ExceptionRecord->ExceptionAddress-0x400000);
        fprintf(trace,"EAX=%08lx EBX=%08lx ECX=%08lx EDX=%08lx ESI=%08lx EDI=%08lx EBP=%08lx ESP=%08lx\n",
            exception->ContextRecord->Eax,exception->ContextRecord->Ebx,exception->ContextRecord->Ecx,
            exception->ContextRecord->Edx,exception->ContextRecord->Esi,exception->ContextRecord->Edi,
            exception->ContextRecord->Ebp,exception->ContextRecord->Esp);
        print_counts();
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

static DWORD WINAPI heartbeat(LPVOID unused)
{
    (void)unused;
    for (;;) { Sleep(10000); print_counts(); }
}

static int patch_functions(void)
{
    const void *functions[RESTORE_HOOK_COUNT]={c_0041ba00,c_0041bad0,c_00451bc0,c_0045b490,c_0045bb30,c_004e5120,
        c_004e5140,c_004e5190,c_004e52b0,c_00562240,c_005631b0,c_00563240,c_00563380,
        c_0056233e,c_00562348,c_005632a0,c_00563400,
        c_004f6520,c_004f6560,c_004f6570,c_004f65b0,c_004f65f0,c_004f6630};
    unsigned int i;
    /* Validate every patch site before changing any original entry. */
    for (i=0;i<RESTORE_HOOK_COUNT;i++) {
        if (memcmp((void *)(uintptr_t)hook_addresses[i],hook_original_prefix[i],5)) {
            fprintf(trace,"Hook prologue mismatch at %08lx; no new hooks installed.\n",hook_addresses[i]);
            return 0;
        }
    }
    for (i=0;i<RESTORE_HOOK_COUNT;i++) {
        if (!restore_image_jump(&game,hook_addresses[i],functions[i])) return 0;
        fprintf(trace,"Installed C hook: %08lx -> %p\n",hook_addresses[i],functions[i]);
    }
    fflush(trace);
    return 1;
}

static int patch_experimental_functions(void)
{
    unsigned int i,installed=0;
    for (i=0;i<experimental_hook_limit;i++) {
        if (experimental_hook_is_quarantined(experimental_hook_addresses[i])) continue;
        if (memcmp((void *)(uintptr_t)experimental_hook_addresses[i],experimental_hook_prefix[i],5)) {
            fprintf(trace,"Experimental hook prologue mismatch at %08lx; no experimental hooks installed.\n",
                experimental_hook_addresses[i]);
            return 0;
        }
    }
    for (i=0;i<experimental_hook_limit;i++) {
        if (experimental_hook_is_quarantined(experimental_hook_addresses[i])) continue;
        if (!restore_image_jump(&game,experimental_hook_addresses[i],experimental_hook_functions[i])) return 0;
        installed++;
    }
    experimental_hooks_installed=1;
    fprintf(trace,"Installed experimental C hook batch: %u active from first %u of %u entries (%u quarantined).\n",
        installed,experimental_hook_limit,RESTORE_EXPERIMENTAL_HOOK_COUNT,experimental_hook_limit-installed);
    fflush(trace);
    return 1;
}

static int verify_experimental_hook_install(void)
{
    unsigned int i;
    if (!patch_experimental_functions()) return 0;
    for (i=0;i<experimental_hook_limit;i++) {
        if (experimental_hook_is_quarantined(experimental_hook_addresses[i])) {
            if (memcmp((void *)(uintptr_t)experimental_hook_addresses[i],experimental_hook_prefix[i],5)) return 0;
            continue;
        }
        if (*(BYTE *)(uintptr_t)experimental_hook_addresses[i]!=0xe9) return 0;
    }
    fprintf(trace,"PASS: installed %u experimental batch hooks; %u selected entries quarantined.\n",
        experimental_hook_active_count(),experimental_hook_limit-experimental_hook_active_count());
    return 1;
}

static int verify_patch_guard(void)
{
    unsigned char saved[RESTORE_HOOK_COUNT][5];
    unsigned int i;
    DWORD previous,ignored;
    BYTE *last=(BYTE *)(uintptr_t)hook_addresses[RESTORE_HOOK_COUNT-1];
    int ok;
    for (i=0;i<RESTORE_HOOK_COUNT;i++) memcpy(saved[i],(void *)(uintptr_t)hook_addresses[i],5);
    if (!VirtualProtect(last,5,PAGE_EXECUTE_READWRITE,&previous)) return 0;
    last[0]^=1;
    VirtualProtect(last,5,previous,&ignored);
    ok=!patch_functions();
    for (i=0;i<RESTORE_HOOK_COUNT-1;i++) {
        if (memcmp(saved[i],(void *)(uintptr_t)hook_addresses[i],5)) ok=0;
    }
    for (i=0;i<RESTORE_HOOK_COUNT;i++) {
        void *entry=(void *)(uintptr_t)hook_addresses[i];
        if (!VirtualProtect(entry,5,PAGE_EXECUTE_READWRITE,&previous)) return 0;
        memcpy(entry,saved[i],5);
        FlushInstructionCache(GetCurrentProcess(),entry,5);
        if (!VirtualProtect(entry,5,previous,&ignored)) return 0;
    }
    if (!ok || !patch_functions()) return 0;
    for (i=0;i<RESTORE_HOOK_COUNT;i++) {
        if (*(BYTE *)(uintptr_t)hook_addresses[i]!=0xe9) return 0;
    }
    fprintf(trace,"PASS: changed last patch site refused before any hook; restored image accepts all %u hooks.\n",RESTORE_HOOK_COUNT);
    return 1;
}

static int compare_memory_transform_void0(DWORD original_address,void *restored_function,
    void *memory,SIZE_T size,const char *name)
{
    BYTE *before=(BYTE *)malloc(size),*expected=(BYTE *)malloc(size);
    int equal=0;
    if (!before || !expected) goto done;
    memcpy(before,memory,size);
    ((void (__cdecl *)(void))(uintptr_t)original_address)();
    memcpy(expected,memory,size);
    memcpy(memory,before,size);
    ((void (__cdecl *)(void))restored_function)();
    equal=!memcmp(memory,expected,size);
    memcpy(memory,before,size);
done:
    free(expected);
    free(before);
    if (!equal) fprintf(trace,"FAIL: reconstructed address calculation differs for %s.\n",name);
    return equal;
}

static int compare_memory_transform_void1(DWORD original_address,void *restored_function,
    int argument,void *memory,SIZE_T size,const char *name)
{
    BYTE *before=(BYTE *)malloc(size),*expected=(BYTE *)malloc(size);
    int equal=0;
    if (!before || !expected) goto done;
    memcpy(before,memory,size);
    ((void (__cdecl *)(int))(uintptr_t)original_address)(argument);
    memcpy(expected,memory,size);
    memcpy(memory,before,size);
    ((void (__cdecl *)(int))restored_function)(argument);
    equal=!memcmp(memory,expected,size);
    memcpy(memory,before,size);
done:
    free(expected);
    free(before);
    if (!equal) fprintf(trace,"FAIL: reconstructed address calculation differs for %s.\n",name);
    return equal;
}

static int compare_memory_transform_void2(DWORD original_address,void *restored_function,
    int argument1,int argument2,void *memory,SIZE_T size,const char *name)
{
    BYTE *before=(BYTE *)malloc(size),*expected=(BYTE *)malloc(size);
    int equal=0;
    if (!before || !expected) goto done;
    memcpy(before,memory,size);
    ((void (__cdecl *)(int,int))(uintptr_t)original_address)(argument1,argument2);
    memcpy(expected,memory,size);
    memcpy(memory,before,size);
    ((void (__cdecl *)(int,int))restored_function)(argument1,argument2);
    equal=!memcmp(memory,expected,size);
    memcpy(memory,before,size);
done:
    free(expected);
    free(before);
    if (!equal) fprintf(trace,"FAIL: reconstructed address calculation differs for %s.\n",name);
    return equal;
}

static int compare_memory_transform_int1(DWORD original_address,void *restored_function,
    int argument,void *memory,SIZE_T size,const char *name)
{
    BYTE *before=(BYTE *)malloc(size),*expected=(BYTE *)malloc(size);
    int original_result=0,restored_result=0,equal=0;
    if (!before || !expected) goto done;
    memcpy(before,memory,size);
    original_result=((int (__cdecl *)(int))(uintptr_t)original_address)(argument);
    memcpy(expected,memory,size);
    memcpy(memory,before,size);
    restored_result=((int (__cdecl *)(int))restored_function)(argument);
    equal=original_result==restored_result && !memcmp(memory,expected,size);
    memcpy(memory,before,size);
done:
    free(expected);
    free(before);
    if (!equal) fprintf(trace,"FAIL: reconstructed address calculation differs for %s.\n",name);
    return equal;
}

static int compare_memory_transform_five_ints(DWORD original_address,void *restored_function,
    int a,int b,int c,int d,int e,void *memory,SIZE_T size,const char *name)
{
    BYTE *before=(BYTE *)malloc(size),*expected=(BYTE *)malloc(size);
    int original_result=0,restored_result=0,equal=0;
    if (!before || !expected) goto done;
    memcpy(before,memory,size);
    original_result=((int (__cdecl *)(int,int,int,int,int))(uintptr_t)original_address)(a,b,c,d,e);
    memcpy(expected,memory,size);
    memcpy(memory,before,size);
    restored_result=((int (__cdecl *)(int,int,int,int,int))restored_function)(a,b,c,d,e);
    equal=original_result==restored_result && !memcmp(memory,expected,size);
    memcpy(memory,before,size);
done:
    free(expected);
    free(before);
    if (!equal) fprintf(trace,"FAIL: reconstructed address calculation differs for %s.\n",name);
    return equal;
}

static int verify_corrected_experimental_candidates(void)
{
    typedef unsigned int (__cdecl *ThreeIntOut)(int,int *,int *);
    typedef int *(__cdecl *CopyTransform)(int *,int,int,int);
    typedef void (__cdecl *OneInt)(int);
    typedef int (__cdecl *TwoInt)(int,int);
    typedef void (__cdecl *ThreeInt)(int,int,int);
    typedef int (__cdecl *OneIntResult)(int);
    typedef int (__cdecl *VoidIntResult)(void);
    typedef int (__cdecl *MaskDelta)(unsigned int,unsigned int,int);
    typedef unsigned char (__cdecl *MaskCount)(unsigned int,int);
    typedef unsigned char (__cdecl *MaskValid)(int,unsigned int,char);
    typedef unsigned char (__cdecl *IndexMask)(int,unsigned int);
    typedef int (__cdecl *TwoIntResult)(int,unsigned int);
    typedef unsigned char (__cdecl *OneIntByte)(int);
    typedef int *(__cdecl *FourIntPointer)(int,int,int,int);
    typedef void (__cdecl *IntPointer)(int,int *);
    unsigned int saved_selector=*(unsigned int *)(uintptr_t)0x00eaac28u;
    BYTE *city=(BYTE *)(uintptr_t)0x00ea0000u;
    BYTE *city_saved=NULL,*fixture=NULL,*expected=NULL;
    int original_out[13],restored_out[13],a,b,c,d,return_a,return_b;
    short *bins=(short *)(uintptr_t)0x00ea3e34u;
    BYTE *links=(BYTE *)(uintptr_t)0x00799a10u;
    BYTE saved_links[0x212],saved_records[0x1c20],saved_ids[0x20];
    int saved_slots[3];
    int list_nodes[2][7];
    BYTE rectangles_a[0x38],rectangles_b[0x38];
    int *pointer_a,*pointer_b;
    int *saved_head;
    int i,ok=0;

    memcpy(saved_links,links,sizeof(saved_links));
    memset(links,0,sizeof(saved_links));
    *(short *)(links+0x000)=1;
    *(short *)(links+0x108)=2;
    *(short *)(links+0x210)=0;
    a=((OneIntResult)(uintptr_t)0x00475b50u)(0);
    b=((OneIntResult)FUN_00475b50)(0);
    memcpy(links,saved_links,sizeof(saved_links));
    if (a!=b || a!=2) { fputs("FAIL: FUN_00475b50 differs from original.\n",trace); goto done; }

    for (i=0;i<4;i++) {
        const int indexes[]={0,1,17,227};
        const unsigned int masks[]={0xffffffffu,0x55555555u,0xaaaaaaaau,0x00010001u};
        a=((MaskDelta)(uintptr_t)0x004775f0u)(masks[i],masks[(i+1)&3],indexes[i]);
        b=((MaskDelta)FUN_004775f0)(masks[i],masks[(i+1)&3],indexes[i]);
        if (a!=b) { fputs("FAIL: FUN_004775f0 differs from original.\n",trace); goto done; }
        a=((MaskCount)(uintptr_t)0x00477690u)(masks[i],indexes[i]);
        b=((MaskCount)FUN_00477690)(masks[i],indexes[i]);
        if (a!=b) { fputs("FAIL: FUN_00477690 differs from original.\n",trace); goto done; }
        a=((MaskValid)(uintptr_t)0x00477eb0u)(indexes[i],masks[i],(char)(i&1));
        b=((MaskValid)FUN_00477eb0)(indexes[i],masks[i],(char)(i&1));
        if (a!=b) { fputs("FAIL: FUN_00477eb0 differs from original.\n",trace); goto done; }
        a=((OneIntResult)(uintptr_t)0x0047e090u)(indexes[i]);
        b=((OneIntResult)FUN_0047e090)(indexes[i]);
        if (a!=b) { fputs("FAIL: FUN_0047e090 differs from original.\n",trace); goto done; }
        a=((IndexMask)(uintptr_t)0x0047ec80u)(indexes[i],masks[i]);
        b=((IndexMask)FUN_0047ec80)(indexes[i],masks[i]);
        if (a!=b) { fputs("FAIL: FUN_0047ec80 differs from original.\n",trace); goto done; }
        a=((IndexMask)(uintptr_t)0x0047ecc0u)(indexes[i],masks[i]);
        b=((IndexMask)FUN_0047ecc0)(indexes[i],masks[i]);
        if (a!=b) { fputs("FAIL: FUN_0047ecc0 differs from original.\n",trace); goto done; }
        a=((IndexMask)(uintptr_t)0x0047ed00u)(indexes[i],masks[i]);
        b=((IndexMask)FUN_0047ed00)(indexes[i],masks[i]);
        if (a!=b) { fputs("FAIL: FUN_0047ed00 differs from original.\n",trace); goto done; }
        a=((IndexMask)(uintptr_t)0x0047ed30u)(indexes[i],masks[i]);
        b=((IndexMask)FUN_0047ed30)(indexes[i],masks[i]);
        if (a!=b) { fputs("FAIL: FUN_0047ed30 differs from original.\n",trace); goto done; }
        a=((OneIntResult)(uintptr_t)0x0047ed60u)(indexes[i]);
        b=((OneIntResult)FUN_0047ed60)(indexes[i]);
        if (a!=b) { fputs("FAIL: FUN_0047ed60 differs from original.\n",trace); goto done; }
        a=((OneIntResult)(uintptr_t)0x004b5640u)(indexes[i]);
        b=((OneIntResult)FUN_004b5640)(indexes[i]);
        if (a!=b) { fputs("FAIL: FUN_004b5640 differs from original.\n",trace); goto done; }
        a=((OneIntResult)(uintptr_t)0x004b56d0u)(indexes[i]);
        b=((OneIntResult)FUN_004b56d0)(indexes[i]);
        if (a!=b) { fputs("FAIL: FUN_004b56d0 differs from original.\n",trace); goto done; }
        a=((OneIntResult)(uintptr_t)0x004b5770u)(indexes[i]);
        b=((OneIntResult)FUN_004b5770)(indexes[i]);
        if (a!=b) { fputs("FAIL: FUN_004b5770 differs from original.\n",trace); goto done; }
        a=((TwoIntResult)(uintptr_t)0x004b57e0u)(indexes[i],(unsigned int)(0x29+i));
        b=((TwoIntResult)FUN_004b57e0)(indexes[i],(unsigned int)(0x29+i));
        if (a!=b) { fputs("FAIL: FUN_004b57e0 differs from original.\n",trace); goto done; }
    }

    for (i=0;i<4;i++) {
        const int indexes[]={0,1,17,799};
        a=((OneIntResult)(uintptr_t)0x004b63d0u)(indexes[i]);
        b=((OneIntResult)FUN_004b63d0)(indexes[i]);
        if (a!=b) { fputs("FAIL: FUN_004b63d0 differs from original.\n",trace); goto done; }
        a=((OneIntResult)(uintptr_t)0x004b8330u)(indexes[i]);
        b=((OneIntResult)FUN_004b8330)(indexes[i]);
        if (a!=b) { fputs("FAIL: FUN_004b8330 differs from original.\n",trace); goto done; }
    }

    memcpy(saved_records,(void *)(uintptr_t)0x00bcb9c8u,sizeof(saved_records));
    memset((void *)(uintptr_t)0x00bcb9c8u,0,sizeof(saved_records));
    if (!compare_memory_transform_five_ints(0x004b6ff0u,(void *)FUN_004b6ff0,
        17,3,0,21,22,(void *)(uintptr_t)0x00bcb9c8u,sizeof(saved_records),"FUN_004b6ff0")) {
        memcpy((void *)(uintptr_t)0x00bcb9c8u,saved_records,sizeof(saved_records));
        goto done;
    }
    memcpy((void *)(uintptr_t)0x00bcb9c8u,saved_records,sizeof(saved_records));

    if (!compare_memory_transform_void2(0x004b7b50u,(void *)FUN_004b7b50,
        1,7,(void *)(uintptr_t)0x00bcba58u,0x70u,"FUN_004b7b50")) goto done;
    if (!compare_memory_transform_void0(0x004b8130u,(void *)FUN_004b8130,
        (void *)(uintptr_t)0x00bcb9c8u,0x1c4au,"FUN_004b8130")) goto done;
    if (!compare_memory_transform_void0(0x004b8aa0u,(void *)FUN_004b8aa0,
        (void *)(uintptr_t)(0x00ea5ed4u+(unsigned int)saved_selector*0x49d8u),6u,"FUN_004b8aa0")) goto done;

    memcpy(saved_ids,(void *)(uintptr_t)0x00bcb9d0u,sizeof(saved_ids));
    memset((void *)(uintptr_t)0x00bcb9d0u,0,sizeof(saved_ids));
    *(short *)(uintptr_t)0x00bcb9d0u=1;
    if (!compare_memory_transform_void1(0x004ba040u,(void *)FUN_004ba040,
        0,(void *)(uintptr_t)(0x00bcd62eu+0x184u),6u,"FUN_004ba040")) {
        memcpy((void *)(uintptr_t)0x00bcb9d0u,saved_ids,sizeof(saved_ids));
        goto done;
    }
    memcpy((void *)(uintptr_t)0x00bcb9d0u,saved_ids,sizeof(saved_ids));
    if (!compare_memory_transform_void0(0x004bca90u,(void *)FUN_004bca90,
        (void *)(uintptr_t)(0x00ea6210u+(unsigned int)saved_selector*0x49d8u),4u,"FUN_004bca90")) goto done;
    if (!compare_memory_transform_void1(0x004bd250u,(void *)FUN_004bd250,
        1,(void *)(uintptr_t)(0x00bcd606u+0x184u),1u,"FUN_004bd250")) goto done;
    a=((int (__cdecl *)(void))(uintptr_t)0x004c5190u)();
    b=((int (__cdecl *)(void))FUN_004c5190)();
    if (a!=b) { fputs("FAIL: FUN_004c5190 differs from original.\n",trace); goto done; }
    if (!compare_memory_transform_void0(0x004c51d0u,(void *)FUN_004c51d0,
        (void *)(uintptr_t)(0x00ea5e3cu+(unsigned int)saved_selector*0x49d8u),0x3c0u,"FUN_004c51d0")) goto done;

    for (i=0;i<2;i++) {
        *(unsigned int *)(uintptr_t)0x00eaac28u=(unsigned int)i;
        a=((OneIntResult)(uintptr_t)0x004e94d0u)(i);
        b=((OneIntResult)FUN_004e94d0)(i);
        if (a!=b) { fputs("FAIL: FUN_004e94d0 differs from original.\n",trace); goto done; }
        if (!compare_memory_transform_void0(0x004e9500u,(void *)FUN_004e9500,
            (void *)(uintptr_t)(0x00ea0000u+(unsigned int)i*0x49d8u),0x49d8u,"FUN_004e9500")) goto done;
        if (!compare_memory_transform_void0(0x004e9700u,(void *)FUN_004e9700,
            (void *)(uintptr_t)(0x00ea0000u+(unsigned int)i*0x49d8u),0x49d8u,"FUN_004e9700")) goto done;
        a=((TwoIntResult)(uintptr_t)0x004e9760u)(0,0);
        b=((TwoIntResult)FUN_004e9760)(0,0);
        if (a!=b) { fputs("FAIL: FUN_004e9760 differs from original.\n",trace); goto done; }
    }
    *(unsigned int *)(uintptr_t)0x00eaac28u=saved_selector;
    if (!compare_memory_transform_void0(0x004e9ee0u,(void *)FUN_004e9ee0,
        (void *)(uintptr_t)0x00eaa5d8u,0x5fau,"FUN_004e9ee0")) goto done;
    if (!compare_memory_transform_void1(0x004f1cb0u,(void *)FUN_004f1cb0,
        1,(void *)(uintptr_t)(0x00bcd602u+0x184u),0x118u,"FUN_004f1cb0")) goto done;
    a=((OneIntResult)(uintptr_t)0x004f2190u)(0);
    b=((OneIntResult)FUN_004f2190)(0);
    if (a!=b) { fputs("FAIL: FUN_004f2190 differs from original.\n",trace); goto done; }
    a=((TwoIntResult)(uintptr_t)0x004f26a0u)(1,0);
    b=((TwoIntResult)FUN_004f26a0)(1,0);
    if (a!=b) { fputs("FAIL: FUN_004f26a0 differs from original.\n",trace); goto done; }
    memcpy(saved_slots,(void *)(uintptr_t)(0x00ea5facu+(unsigned int)saved_selector*0x49d8u),sizeof(saved_slots));
    ((int *)(uintptr_t)(0x00ea5facu+(unsigned int)saved_selector*0x49d8u))[0]=11;
    ((int *)(uintptr_t)(0x00ea5facu+(unsigned int)saved_selector*0x49d8u))[1]=22;
    ((int *)(uintptr_t)(0x00ea5facu+(unsigned int)saved_selector*0x49d8u))[2]=33;
    if (!compare_memory_transform_void2(0x004f4a60u,(void *)FUN_004f4a60,
        44,1,(void *)(uintptr_t)(0x00ea5facu+(unsigned int)saved_selector*0x49d8u),sizeof(saved_slots),"FUN_004f4a60")) {
        memcpy((void *)(uintptr_t)(0x00ea5facu+(unsigned int)saved_selector*0x49d8u),saved_slots,sizeof(saved_slots));
        goto done;
    }
    if (!compare_memory_transform_void2(0x004f4a80u,(void *)FUN_004f4a80,
        0,1,(void *)(uintptr_t)(0x00ea5facu+(unsigned int)saved_selector*0x49d8u),sizeof(saved_slots),"FUN_004f4a80")) {
        memcpy((void *)(uintptr_t)(0x00ea5facu+(unsigned int)saved_selector*0x49d8u),saved_slots,sizeof(saved_slots));
        goto done;
    }
    a=((OneIntResult)(uintptr_t)0x004f4aa0u)(11);
    b=((OneIntResult)FUN_004f4aa0)(11);
    memcpy((void *)(uintptr_t)(0x00ea5facu+(unsigned int)saved_selector*0x49d8u),saved_slots,sizeof(saved_slots));
    if (a!=b) { fputs("FAIL: FUN_004f4aa0 differs from original.\n",trace); goto done; }
    a=((TwoIntResult)(uintptr_t)0x004f56a0u)(0,0);
    b=((TwoIntResult)FUN_004f56a0)(0,0);
    if (a!=b) { fputs("FAIL: FUN_004f56a0 differs from original.\n",trace); goto done; }
    a=((TwoIntResult)(uintptr_t)0x004f5770u)(0,0);
    b=((TwoIntResult)FUN_004f5770)(0,0);
    if (a!=b) { fputs("FAIL: FUN_004f5770 differs from original.\n",trace); goto done; }
    a=((OneIntResult)(uintptr_t)0x004f6000u)(0);
    b=((OneIntResult)FUN_004f6000)(0);
    if (a!=b) { fputs("FAIL: FUN_004f6000 differs from original.\n",trace); goto done; }
    a=((OneIntByte)(uintptr_t)0x0050fc70u)(1);
    b=((OneIntByte)FUN_0050fc70)(1);
    if (a!=b) { fputs("FAIL: FUN_0050fc70 differs from original.\n",trace); goto done; }
    a=((OneIntResult)(uintptr_t)0x0051afa0u)(0);
    b=((OneIntResult)FUN_0051afa0)(0);
    if (a!=b) { fputs("FAIL: FUN_0051afa0 differs from original.\n",trace); goto done; }
    a=((OneIntResult)(uintptr_t)0x0051aff0u)(0);
    b=((OneIntResult)FUN_0051aff0)(0);
    if (a!=b) { fputs("FAIL: FUN_0051aff0 differs from original.\n",trace); goto done; }
    if (!compare_memory_transform_int1(0x00542110u,(void *)FUN_00542110,
        0,(void *)(uintptr_t)0x00799a34u,4u,"FUN_00542110")) goto done;
    return_a=((ThreeIntOut)(uintptr_t)0x00542f40u)(0,&a,&b);
    return_b=((ThreeIntOut)FUN_00542f40)(0,&c,&d);
    if ((unsigned char)return_a!=(unsigned char)return_b || a!=c || b!=d) {
        fputs("FAIL: FUN_00542f40 differs from original.\n",trace); goto done;
    }

    if (!compare_memory_transform_void0(0x004d3200u,(void *)FUN_004d3200,
        (void *)(uintptr_t)0x007900b8u,0x9920u,"FUN_004d3200")) goto done;
    if (!compare_memory_transform_void0(0x004d3d80u,(void *)FUN_004d3d80,
        (void *)(uintptr_t)0x00785ddeu,0x4830u,"FUN_004d3d80")) goto done;
    if (!compare_memory_transform_void0(0x004d3f40u,(void *)FUN_004d3f40,
        (void *)(uintptr_t)0x00bcbac0u,0x1b04u,"FUN_004d3f40")) goto done;
    if (!compare_memory_transform_void0(0x004d3f60u,(void *)FUN_004d3f60,
        (void *)(uintptr_t)0x00799af0u,0x101bacu,"FUN_004d3f60")) goto done;
    *(unsigned int *)(uintptr_t)0x00eaac28u=0;
    if (!compare_memory_transform_void0(0x004d4040u,(void *)FUN_004d4040,
        (void *)(uintptr_t)0x00ea41fcu,0x2du,"FUN_004d4040")) goto done;
    if (!compare_memory_transform_void0(0x004d4080u,(void *)FUN_004d4080,
        (void *)(uintptr_t)0x00799a5eu,0x101bf9u,"FUN_004d4080")) goto done;
    if (!compare_memory_transform_void0(0x004d40a0u,(void *)FUN_004d40a0,
        (void *)(uintptr_t)0x00ea5d38u,0x49e0u,"FUN_004d40a0")) goto done;
    if (!compare_memory_transform_void0(0x004d4180u,(void *)FUN_004d4180,
        (void *)(uintptr_t)0x00799ae0u,0x101b9bu,"FUN_004d4180")) goto done;

    memset(list_nodes,0,sizeof(list_nodes));
    list_nodes[0][0]=11;
    list_nodes[0][1]=22;
    list_nodes[0][6]=(int)(uintptr_t)&list_nodes[1][0];
    list_nodes[1][0]=33;
    list_nodes[1][1]=44;
    saved_head=*(int **)(uintptr_t)0x00e94618u;
    *(int **)(uintptr_t)0x00e94618u=&list_nodes[0][0];
    pointer_a=((FourIntPointer)(uintptr_t)0x004d5fe0u)(1,0,33,44);
    pointer_b=((FourIntPointer)FUN_004d5fe0)(1,0,33,44);
    *(int **)(uintptr_t)0x00e94618u=saved_head;
    if (pointer_a!=pointer_b || pointer_a!=&list_nodes[1][0]) {
        fputs("FAIL: FUN_004d5fe0 differs from original.\n",trace); goto done;
    }

    list_nodes[0][0]=1;
    list_nodes[0][2]=10;
    list_nodes[0][3]=20;
    list_nodes[0][4]=30;
    list_nodes[0][5]=40;
    list_nodes[0][6]=(int)(uintptr_t)&list_nodes[1][0];
    list_nodes[1][0]=2;
    list_nodes[1][2]=50;
    list_nodes[1][3]=60;
    list_nodes[1][4]=70;
    list_nodes[1][5]=80;
    memset(rectangles_a,0xcc,sizeof(rectangles_a));
    memset(rectangles_b,0xcc,sizeof(rectangles_b));
    ((IntPointer)(uintptr_t)0x004d66f0u)((int)(uintptr_t)rectangles_a,&list_nodes[0][0]);
    ((IntPointer)FUN_004d66f0)((int)(uintptr_t)rectangles_b,&list_nodes[0][0]);
    if (memcmp(rectangles_a,rectangles_b,sizeof(rectangles_a))) {
        fputs("FAIL: FUN_004d66f0 differs from original.\n",trace); goto done;
    }
    if (!compare_memory_transform_void0(0x004de140u,(void *)FUN_004de140,
        (void *)(uintptr_t)0x00e94ddcu,0x20u,"FUN_004de140")) goto done;
    if (!compare_memory_transform_void0(0x004e1850u,(void *)FUN_004e1850,
        (void *)(uintptr_t)0x00e95124u,0x50u,"FUN_004e1850")) goto done;
    *(unsigned int *)(uintptr_t)0x00eaac28u=0;
    if (!compare_memory_transform_void0(0x004e7db0u,(void *)FUN_004e7db0,
        (void *)(uintptr_t)0x00ea5cc0u,0x130u,"FUN_004e7db0")) goto done;
    if (!compare_memory_transform_void0(0x004e7e10u,(void *)FUN_004e7e10,
        (void *)(uintptr_t)0x00ea1820u,4u,"FUN_004e7e10")) goto done;
    if (!compare_memory_transform_void0(0x004e7e20u,(void *)FUN_004e7e20,
        (void *)(uintptr_t)0x00ea1820u,4u,"FUN_004e7e20")) goto done;
    if (!compare_memory_transform_void1(0x004e8e80u,(void *)FUN_004e8e80,
        -15,(void *)(uintptr_t)0x00799ae0u,0x101bb3u,"FUN_004e8e80")) goto done;

    for (i=0;i<4;i++) {
        const int transform[][2]={{0,0},{4,0},{2,4},{6,1}};
        memset(original_out,0xcc,sizeof(original_out));
        memset(restored_out,0xcc,sizeof(restored_out));
        pointer_a=((CopyTransform)(uintptr_t)0x004ded50u)(original_out,1,transform[i][0],transform[i][1]);
        pointer_b=((CopyTransform)FUN_004ded50)(restored_out,1,transform[i][0],transform[i][1]);
        if (pointer_a!=original_out || pointer_b!=restored_out ||
            memcmp(original_out,restored_out,sizeof(original_out))) {
            fputs("FAIL: FUN_004ded50 differs from original.\n",trace); goto done;
        }
    }

    for (i=0;i<6;i++) {
        const int values[]={-1,0,49,50,51,1000};
        return_a=((OneIntResult)(uintptr_t)0x004e1600u)(values[i]);
        return_b=((OneIntResult)FUN_004e1600)(values[i]);
        if (return_a!=return_b) { fputs("FAIL: FUN_004e1600 differs from original.\n",trace); goto done; }
    }
    return_a=((VoidIntResult)(uintptr_t)0x004e1750u)();
    return_b=((VoidIntResult)FUN_004e1750)();
    if (return_a!=return_b) { fputs("FAIL: FUN_004e1750 differs from original.\n",trace); goto done; }

    for (i=0;i<4;i++) {
        const int indexes[]={0,1,127,3999};
        return_a=((ThreeIntOut)(uintptr_t)0x004e0c50u)(indexes[i],&a,&b);
        return_b=((ThreeIntOut)FUN_004e0c50)(indexes[i],&c,&d);
        if (return_a!=return_b || a!=c || b!=d) {
            fputs("FAIL: FUN_004e0c50 differs from original.\n",trace); goto done;
        }
        ((void (__cdecl *)(int,int *,int *))(uintptr_t)0x004e0cc0u)(indexes[i],&a,&b);
        ((void (__cdecl *)(int,int *,int *))FUN_004e0cc0)(indexes[i],&c,&d);
        if (a!=c || b!=d) { fputs("FAIL: FUN_004e0cc0 differs from original.\n",trace); goto done; }
    }

    for (i=0;i<3;i++) {
        const int deltas[]={-200,31,200};
        int *value=(int *)(uintptr_t)0x00ea1888u;
        int saved=*value;
        *value=77;
        ((OneInt)(uintptr_t)0x004e85d0u)(deltas[i]);
        a=*value;
        *value=77;
        ((OneInt)FUN_004e85d0)(deltas[i]);
        b=*value;
        *value=saved;
        if (a!=b) { fputs("FAIL: FUN_004e85d0 differs from original.\n",trace); goto done; }
    }

    *(unsigned int *)(uintptr_t)0x00eaac28u=0;
    for (i=0;i<3;i++) {
        const int deltas[]={-200,31,200};
        int *value=(int *)(uintptr_t)0x00ea5b48u;
        int saved=*value;
        *value=77;
        if (!compare_memory_transform_void1(0x004e96c0u,(void *)FUN_004e96c0,
            deltas[i],value,sizeof(*value),"FUN_004e96c0")) {
            *value=saved;
            goto done;
        }
        *value=saved;
    }

    city_saved=(BYTE *)malloc(0x5f10u);
    fixture=(BYTE *)malloc(0x5f10u);
    expected=(BYTE *)malloc(0x5f10u);
    if (!city_saved || !fixture || !expected) goto done;
    memcpy(city_saved,city,0x5f10u);
    memset(city,0,0x5f10u);
    for (i=0;i<100;i++) bins[i]=(short)(i%7-2);
    memcpy(fixture,city,0x5f10u);
    ((OneInt)(uintptr_t)0x004e87a0u)(0);
    memcpy(expected,city,0x5f10u);
    memcpy(city,fixture,0x5f10u);
    ((OneInt)FUN_004e87a0)(0);
    if (memcmp(city,expected,0x5f10u)) {
        fputs("FAIL: FUN_004e87a0 differs from original.\n",trace); goto done;
    }
    memcpy(city,city_saved,0x5f10u);

    for (i=0;i<10;i++) bins[i]=(short)(i-3);
    a=((TwoInt)(uintptr_t)0x004e92c0u)(0,0);
    b=((TwoInt)FUN_004e92c0)(0,0);
    if (a!=b) { fputs("FAIL: FUN_004e92c0 differs from original.\n",trace); goto done; }
    a=((OneIntResult)(uintptr_t)0x004e94a0u)(0);
    b=((OneIntResult)FUN_004e94a0)(0);
    if (a!=b) { fputs("FAIL: FUN_004e94a0 differs from original.\n",trace); goto done; }
    memcpy(fixture,bins,20);
    ((ThreeInt)(uintptr_t)0x004e92f0u)(0,0,7);
    memcpy(expected,bins,20);
    memcpy(bins,fixture,20);
    ((ThreeInt)FUN_004e92f0)(0,0,7);
    if (memcmp(bins,expected,20)) {
        fputs("FAIL: FUN_004e92f0 differs from original.\n",trace); goto done;
    }
    ok=1;
    fputs("PASS: seventy reconstructed functions match original x86 behavior.\n",trace);
done:
    memcpy(links,saved_links,sizeof(saved_links));
    if (city_saved) memcpy(city,city_saved,0x5f10u);
    *(unsigned int *)(uintptr_t)0x00eaac28u=saved_selector;
    free(expected);
    free(fixture);
    free(city_saved);
    return ok;
}

/* ABI and side-effect checks against the real mapped image, before replacement. */
static int verify_image(void)
{
    const RestoreQueueFunctions queue_c={c_004f6520,c_004f6560,c_004f6570,c_004f65b0,c_004f65f0,c_004f6630};
    const RestoreQueueFunctions queue_patched={(RestoreQueueOne)0x4f6520,(RestoreQueueOne)0x4f6560,
        (RestoreQueueFind)0x4f6570,(RestoreQueuePush)0x4f65b0,(RestoreQueueNext)0x4f65f0,(RestoreQueueOne)0x4f6630};
    const int inputs[]={(-2147483647-1),-1,0,1,8,9,24,25,199,200,299,300,399,400,401,2147483647};
    unsigned int i,checks=0;
    typedef unsigned int (__stdcall *StdOne)(int);
    typedef unsigned int (__cdecl *CdeclOne)(int);
    typedef unsigned char (__cdecl *CdeclTwo)(int,int);
    typedef void *(__fastcall *Constructor)(void *,void *,int,int,int,int);
    if (!verify_corrected_experimental_candidates()) return 0;
    if (!restore_verify_city_history(trace)) return 0;
    for (i=0;i<sizeof(inputs)/sizeof(inputs[0]);i++) {
        int x=inputs[i];
        int original_rect[6]={101,102,103,104,105,106};
        int restored_rect[6]={101,102,103,104,105,106};
        void *returned;
        if (((StdOne)0x41ba00)(x)!=FUN_0041ba00(x)) return 0;
        if (((StdOne)0x41bad0)(x)!=FUN_0041bad0((unsigned int)x)) return 0;
        if (((CdeclOne)0x451bc0)(x)!=FUN_00451bc0(x)) return 0;
        if (((CdeclOne)0x45b490)(x)!=FUN_0045b490(x)) return 0;
        if (((CdeclTwo)0x45bb30)(x,0)!=(unsigned char)FUN_0045bb30(x,0)) return 0;
        returned=((Constructor)0x4e5120)(original_rect+1,NULL,x,2,3,4);
        FUN_004e5120(restored_rect+1,x,2,3,4);
        if (returned!=original_rect+1 || memcmp(original_rect,restored_rect,sizeof(original_rect))) return 0;
        checks+=6;
    }
    if (!restore_verify_geometry(trace,c_004e5140,c_004e5190,c_004e52b0,"C adapters before patch")) return 0;
    if (!restore_verify_memory_strings(trace,c_00562240,c_005631b0,c_00563240,c_00563380,"C adapters before patch")) return 0;
    if (!restore_verify_core(trace,c_0056233e,c_00562348,c_005632a0,c_00563400,"C adapters before patch")) return 0;
    if (!restore_verify_queue(trace,&queue_c,"C adapters before patch")) return 0;
    if (!patch_functions()) return 0;
    if (!restore_verify_geometry(trace,(RestoreCopyRecord)0x4e5140,(RestoreUpdateRecord)0x4e5190,
        (RestoreUpdateRecords)0x4e52b0,"patched game entries")) return 0;
    restore_geometry_fixtures_close();
    if (!restore_verify_memory_strings(trace,(RestoreBoundedStringCopy)0x562240,(RestoreStringCompare)0x5631b0,
        (RestoreMemoryFill)0x563240,(RestoreStringLength)0x563380,"patched game entries")) return 0;
    restore_memory_string_fixtures_close();
    if (!restore_verify_core(trace,(RestoreSeed)0x56233e,(RestoreRandom)0x562348,
        (RestoreFind)0x5632a0,(RestoreMove)0x563400,"patched game entries")) return 0;
    restore_core_fixtures_close();
    if (!restore_verify_queue(trace,&queue_patched,"patched game entries")) return 0;
    restore_queue_fixtures_close();
    for (i=0;i<sizeof(inputs)/sizeof(inputs[0]);i++) {
        int x=inputs[i],a[4];
        if (((StdOne)0x41ba00)(x)!=FUN_0041ba00(x)) return 0;
        if (((StdOne)0x41bad0)(x)!=FUN_0041bad0((unsigned int)x)) return 0;
        if (((CdeclOne)0x451bc0)(x)!=FUN_00451bc0(x)) return 0;
        if (((CdeclOne)0x45b490)(x)!=FUN_0045b490(x)) return 0;
        if (((CdeclTwo)0x45bb30)(x,1)!=(unsigned char)FUN_0045bb30(x,1)) return 0;
        if (((Constructor)0x4e5120)(a,NULL,x,2,3,4)!=a || a[0]!=x || a[3]!=4) return 0;
        checks+=6;
    }
    fprintf(trace,"PASS: %u mapped-image and patched-ABI checks across six C routines.\n",checks);
    print_counts();
    return 1;
}

static int runtime_main(void)
{
    char directory[MAX_PATH]="C:\\Program Files (x86)\\GOG.com\\Pharaoh";
    char filename[MAX_PATH],logfile[MAX_PATH],*separator;
    LPWSTR *arguments;
    int count,i,inspect=0,self_test=0,no_hooks=0,no_experimental_hooks=0;
    int compatibility_test=0,guard_test=0,experimental_test=0;
    GetModuleFileNameA(NULL,logfile,sizeof(logfile));
    separator=strrchr(logfile,'\\');
    if (!separator) return 2;
    strcpy_s(separator+1,sizeof(logfile)-(separator+1-logfile),"PharaohRestore.log");
    trace=_fsopen(logfile,"w",_SH_DENYNO);
    if (!trace) return 2;
    setvbuf(trace,NULL,_IONBF,0);
    fputs("PharaohRestore hybrid runtime: compiled C hooks with original native fallback.\n",trace);
    arguments=CommandLineToArgvW(GetCommandLineW(),&count);
    for (i=1;i<count;i++) {
        if (wcscmp(arguments[i],L"--game-dir")==0 && i+1<count) {
            if (!WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,arguments[++i],-1,directory,sizeof(directory),NULL,NULL)) return 2;
        } else if (wcscmp(arguments[i],L"--inspect")==0) inspect=1;
        else if (wcscmp(arguments[i],L"--self-test")==0) self_test=1;
        else if (wcscmp(arguments[i],L"--compat-self-test")==0) { self_test=1; compatibility_test=1; }
        else if (wcscmp(arguments[i],L"--hook-guard-test")==0) guard_test=1;
        else if (wcscmp(arguments[i],L"--experimental-hook-test")==0) experimental_test=1;
        else if (wcscmp(arguments[i],L"--no-c-hooks")==0) no_hooks=1;
        else if (wcscmp(arguments[i],L"--no-experimental-hooks")==0) no_experimental_hooks=1;
        else if (wcscmp(arguments[i],L"--experimental-limit")==0 && i+1<count) {
            wchar_t *end;
            unsigned long value=wcstoul(arguments[++i],&end,10);
            if (*end || value>RESTORE_EXPERIMENTAL_HOOK_COUNT) {
                fprintf(trace,"Experimental hook limit must be between 0 and %u.\n",RESTORE_EXPERIMENTAL_HOOK_COUNT);
                return 2;
            }
            experimental_hook_limit=(unsigned int)value;
        }
        else if (wcscmp(arguments[i],L"--no-compat")==0) { /* Preflight-only diagnostic switch. */ }
        else { fprintf(trace,"Unknown argument. Supported: --game-dir PATH --inspect --self-test --compat-self-test --hook-guard-test --experimental-hook-test --no-c-hooks --no-experimental-hooks --experimental-limit N --no-compat\n"); return 2; }
    }
    LocalFree(arguments);
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
    SetUnhandledExceptionFilter(report_exception);
    AddVectoredExceptionHandler(1,report_vectored_exception);
    fprintf(trace,"Process-local compatibility: %s\n",getenv("__COMPAT_LAYER") ? getenv("__COMPAT_LAYER") : "native");
    if (compatibility_test) {
        HDC display=GetDC(NULL);
        int depth=display ? GetDeviceCaps(display,BITSPIXEL)*GetDeviceCaps(display,PLANES) : 0;
        if (display) ReleaseDC(NULL,display);
        fprintf(trace,"Compatibility startup reports %d-bit color.\n",depth);
        if (depth!=16) return 7;
    }
    if (!SetCurrentDirectoryA(directory) || !SetDllDirectoryA(directory)) {
        fprintf(trace,"Cannot use game directory: %s, error %lu.\n",directory,GetLastError()); return 2;
    }
    if (sprintf_s(filename,sizeof(filename),"%s\\Pharaoh.exe",directory)<0) return 2;
    sprintf_s(command_line,sizeof(command_line),"\"%s\"",filename);
    fprintf(trace,"Installation: %s\n",directory);
    if (!restore_image_adopt(&game,filename,import_hook,trace)) return 3;
    if (inspect) { fputs("PASS: image validation and import resolution; no game entry executed.\n",trace); restore_image_close(&game); return 0; }
    if (guard_test) {
        int result=verify_patch_guard();
        restore_image_close(&game);
        fclose(trace);
        return result ? 0 : 8;
    }
    if (experimental_test) {
        int result=verify_experimental_hook_install();
        restore_image_close(&game);
        fclose(trace);
        return result ? 0 : 9;
    }
    if (self_test) {
        int result=verify_image();
        restore_image_close(&game);
        fclose(trace);
        return result ? 0 : 4;
    }
    if (!no_hooks && !patch_functions()) return 5;
    if (!no_hooks && !no_experimental_hooks && experimental_hook_limit && !patch_experimental_functions()) return 9;
    fprintf(trace,"Starting original entry point %08lx with %u C replacements.\n",0x400000+game.entry_rva,
        no_hooks?0:RESTORE_HOOK_COUNT+(no_experimental_hooks?0:experimental_hook_active_count()));
    CloseHandle(CreateThread(NULL,0,heartbeat,NULL,0,NULL));
    ((void (__cdecl *)(void))(game.base+game.entry_rva))();
    fputs("Original entry returned.\n",trace);
    print_counts();
    restore_image_close(&game);
    fclose(trace);
    return 0;
}

/* Invoked by the executable entry trampoline after the loader lock is released. */
void WINAPI RestoreStart(void)
{
    UINT exit_code;
    if (compatibility_child(&exit_code)) ExitProcess(exit_code);
    ExitProcess((UINT)runtime_main());
}

BOOL WINAPI DllMain(HINSTANCE module,DWORD reason,LPVOID reserved)
{
    (void)reserved;
    if (reason==DLL_PROCESS_ATTACH) DisableThreadLibraryCalls(module);
    return TRUE;
}
