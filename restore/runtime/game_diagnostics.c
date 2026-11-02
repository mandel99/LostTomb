#include "game_diagnostics.h"
#include <windows.h>
#include <dbghelp.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

static HANDLE journal=INVALID_HANDLE_VALUE;
static wchar_t dump_path[MAX_PATH];
static wchar_t hang_path[MAX_PATH];
static LONG reporting;
static volatile LONG progress_stage,progress_time,save_time,monitor_started,monitor_stopped;
static DWORD main_thread;
static unsigned int post_save_frames;
struct grid_event {uint32_t site;int32_t actor,tile,head,next;int inserting;};
static struct grid_event grid_events[32];
static uint32_t grid_event_count;
static int grid_failed;

void restore_game_progress(uint32_t stage)
{
    if(!InterlockedCompareExchange(&monitor_started,0,0))return;
    InterlockedExchange(&progress_stage,(LONG)stage);
    InterlockedExchange(&progress_time,(LONG)GetTickCount());
}

/* One bounded snapshot per process, only after a completed save. Windows
   exception filters cannot capture an infinite loop. DbgHelp runs on this
   separate thread so the stalled main thread's stack can be retained. */
static DWORD WINAPI post_save_monitor(void *unused)
{
    typedef BOOL (WINAPI *WriteDump)(HANDLE,DWORD,HANDLE,MINIDUMP_TYPE,
        PMINIDUMP_EXCEPTION_INFORMATION,PMINIDUMP_USER_STREAM_INFORMATION,
        PMINIDUMP_CALLBACK_INFORMATION);
    HMODULE library=LoadLibraryExW(L"dbghelp.dll",NULL,LOAD_LIBRARY_SEARCH_SYSTEM32);
    WriteDump write_dump=library?(WriteDump)GetProcAddress(library,"MiniDumpWriteDump"):NULL;
    (void)unused;
    if(!write_dump)return 0;
    while(!InterlockedCompareExchange(&monitor_stopped,0,0)){
        DWORD now,armed,last,stage;
        HANDLE file;
        MINIDUMP_USER_STREAM streams[2];
        MINIDUMP_USER_STREAM_INFORMATION info;
        Sleep(500);
        now=GetTickCount();
        armed=(DWORD)InterlockedCompareExchange(&save_time,0,0);
        last=(DWORD)InterlockedCompareExchange(&progress_time,0,0);
        stage=(DWORD)InterlockedCompareExchange(&progress_stage,0,0);
        if(!stage||now-armed>60000u||now-last<15000u)continue;
        if(InterlockedCompareExchange(&reporting,1,0))continue;
        restore_game_trace("post-save stall main_thread=%lu stage=%lu actor=%lu phase=%lu elapsed_ms=%lu\n",
            main_thread,stage,*(volatile uint32_t *)(uintptr_t)0xd056f8u,
            *(volatile uint32_t *)(uintptr_t)0x78d7ccu,now-last);
        file=CreateFileW(hang_path,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
        if(file!=INVALID_HANDLE_VALUE){
            BOOL ok;
            streams[0].Type=0x10000u;streams[0].BufferSize=228u*228u*2u;
            streams[0].Buffer=(void *)(uintptr_t)0xb350dcu;
            streams[1].Type=0x10001u;streams[1].BufferSize=2000u*388u;
            streams[1].Buffer=(void *)(uintptr_t)0xbcd5f8u;
            info.UserStreamCount=2;info.UserStreamArray=streams;
            ok=write_dump(GetCurrentProcess(),GetCurrentProcessId(),file,MiniDumpWithDataSegs,NULL,&info,NULL);
            restore_game_trace("post-save hang dump success=%d error=%lu\n",ok,ok?0:GetLastError());
            CloseHandle(file);
        }else restore_game_trace("post-save hang dump open error=%lu\n",GetLastError());
        InterlockedExchange(&reporting,0);
        return 0;
    }
    return 0;
}

void restore_game_actor_grid_event(uint32_t site,int32_t actor,int inserting)
{
    struct grid_event *event;
    uintptr_t record;
    if(journal==INVALID_HANDLE_VALUE)return;
    event=&grid_events[grid_event_count++%32u];
    event->site=site;event->actor=actor;event->inserting=inserting;
    event->tile=-1;event->head=-1;event->next=-1;
    if(actor<=0||actor>=2000)return;
    record=0xbcd5f8u+(uint32_t)actor*388u;
    event->tile=*(volatile int32_t *)(record+32u);
    event->next=*(volatile int16_t *)(record+8u);
    if(event->tile>=0&&event->tile<228*228)
        event->head=*(volatile int16_t *)(uintptr_t)(0xb350dcu+(uint32_t)event->tile*2u);
}

__declspec(noreturn) void restore_game_actor_grid_failure(uint32_t tile,int32_t actor)
{
    uint32_t i,count=grid_event_count<32u?grid_event_count:32u;
    int32_t id;
    grid_failed=1;
    restore_game_trace("actor grid corrupt tile=%lu actor=%ld; last %lu link/unlink calls:\n",tile,actor,count);
    for(i=0;i<count;++i){
        const struct grid_event *event=&grid_events[(grid_event_count-count+i)%32u];
        restore_game_trace("grid %s site=%08lx actor=%ld tile=%ld head=%ld next=%ld\n",
            event->inserting?"insert":"remove",event->site,event->actor,event->tile,event->head,event->next);
    }
    if(tile<228u*228u){
        id=*(volatile int16_t *)(uintptr_t)(0xb350dcu+tile*2u);
        for(i=0;i<64u&&id>0&&id<2000;++i){
            uintptr_t record=0xbcd5f8u+(uint32_t)id*388u;
            int32_t next=*(volatile int16_t *)(record+8u);
            restore_game_trace("grid node actor=%ld next=%ld kind=%u state=%u tile=%ld x=%d y=%d\n",
                id,next,(unsigned)*(volatile uint8_t *)(record+10u),(unsigned)*(volatile uint8_t *)(record+14u),
                *(volatile int32_t *)(record+32u),(int)*(volatile int16_t *)(record+20u),(int)*(volatile int16_t *)(record+22u));
            id=next;
        }
    }
    /* Preserve the corrupt state for diagnosis; do not silently drop actors
       or repair their links from inside the renderer. */
    RaiseException(0xe0475244u,EXCEPTION_NONCONTINUABLE,0,NULL);
    abort();
}

void restore_game_trace(const char *format,...)
{
    char line[512];
    DWORD written;
    int prefix,length;
    va_list args;
    if(journal==INVALID_HANDLE_VALUE)return;
    prefix=sprintf_s(line,sizeof(line),"%lu ",GetTickCount());
    if(prefix<0)return;
    va_start(args,format);
    length=_vsnprintf_s(line+prefix,sizeof(line)-(size_t)prefix,_TRUNCATE,format,args);
    va_end(args);
    if(length<0)length=(int)strlen(line+prefix);
    (void)WriteFile(journal,line,(DWORD)(prefix+length),&written,NULL);
}

static LONG WINAPI report_crash(EXCEPTION_POINTERS *exception)
{
    typedef BOOL (WINAPI *WriteDump)(HANDLE,DWORD,HANDLE,MINIDUMP_TYPE,
        PMINIDUMP_EXCEPTION_INFORMATION,PMINIDUMP_USER_STREAM_INFORMATION,
        PMINIDUMP_CALLBACK_INFORMATION);
    HMODULE library;
    HANDLE file;
    WriteDump write_dump;
    MINIDUMP_EXCEPTION_INFORMATION info;
    MINIDUMP_USER_STREAM streams[2];
    MINIDUMP_USER_STREAM_INFORMATION stream_info;
    if(InterlockedExchange(&reporting,1))return EXCEPTION_CONTINUE_SEARCH;
    restore_game_trace("exception code=%08lx address=%p EIP=%08lx ESP=%08lx\n",
        exception->ExceptionRecord->ExceptionCode,exception->ExceptionRecord->ExceptionAddress,
        exception->ContextRecord->Eip,exception->ContextRecord->Esp);
    library=LoadLibraryExW(L"dbghelp.dll",NULL,LOAD_LIBRARY_SEARCH_SYSTEM32);
    write_dump=library?(WriteDump)GetProcAddress(library,"MiniDumpWriteDump"):NULL;
    file=CreateFileW(dump_path,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    if(file!=INVALID_HANDLE_VALUE) {
        info.ThreadId=GetCurrentThreadId();info.ExceptionPointers=exception;info.ClientPointers=FALSE;
        /* These streams retain the actual map heads and actor records even
           when DbgHelp omits a virtual/uninitialized PE data section. */
        streams[0].Type=0x10000u;streams[0].BufferSize=228u*228u*2u;
        streams[0].Buffer=(void *)(uintptr_t)0xb350dcu;
        streams[1].Type=0x10001u;streams[1].BufferSize=2000u*388u;
        streams[1].Buffer=(void *)(uintptr_t)0xbcd5f8u;
        stream_info.UserStreamCount=2;stream_info.UserStreamArray=streams;
        if(write_dump) {
            BOOL ok=write_dump(GetCurrentProcess(),GetCurrentProcessId(),file,MiniDumpWithDataSegs,&info,
                grid_failed?&stream_info:NULL,NULL);
            restore_game_trace("minidump success=%d error=%lu\n",ok,ok?0:GetLastError());
        }
        CloseHandle(file);
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

static void normal_exit(void)
{
    InterlockedExchange(&monitor_stopped,1);
    restore_game_trace("normal process exit\n");
    if(journal!=INVALID_HANDLE_VALUE){CloseHandle(journal);journal=INVALID_HANDLE_VALUE;}
}

int restore_game_startup_exception_filter(EXCEPTION_POINTERS *exception)
{
    HWND window=*(HWND *)(uintptr_t)0xe39140u;
    /* Native scope56F4D0/filter414395 jumps to413EB0: minimize the actual
       game window, then CONTINUE_SEARCH. Record the first fault while the
       compiler-owned SEH chain is still intact; do not swallow it. */
    if(window)ShowWindow(window,SW_MINIMIZE);
    (void)report_crash(exception);
    return EXCEPTION_CONTINUE_SEARCH;
}

void restore_game_diagnostics_initialize(void)
{
    wchar_t path[MAX_PATH],*name;
    DWORD length=GetModuleFileNameW(NULL,path,MAX_PATH);
    IMAGE_DOS_HEADER *image=(IMAGE_DOS_HEADER *)GetModuleHandleW(NULL);
    IMAGE_NT_HEADERS *header=(IMAGE_NT_HEADERS *)((unsigned char *)image+image->e_lfanew);
    if(!length||length>=MAX_PATH)return;
    name=wcsrchr(path,L'\\');if(!name)return;
    if(wcscpy_s(name+1,MAX_PATH-(size_t)(name+1-path),L"PharaohRestoreC-run.log"))return;
    journal=CreateFileW(path,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,
                       FILE_ATTRIBUTE_NORMAL|FILE_FLAG_WRITE_THROUGH,NULL);
    if(journal==INVALID_HANDLE_VALUE)return;
    if(wcscpy_s(name+1,MAX_PATH-(size_t)(name+1-path),L"PharaohRestoreC-crash.dmp")||
       wcscpy_s(dump_path,MAX_PATH,path))return;
    if(wcscpy_s(name+1,MAX_PATH-(size_t)(name+1-path),L"PharaohRestoreC-hang.dmp")||
       wcscpy_s(hang_path,MAX_PATH,path))return;
    main_thread=GetCurrentThreadId();
    SetUnhandledExceptionFilter(report_crash);
    (void)atexit(normal_exit);
    restore_game_trace("build COFF=%08lx pid=%lu\n",header->FileHeader.TimeDateStamp,GetCurrentProcessId());
}

void restore_game_trace_save_complete(void)
{
    HANDLE thread;
    if(journal==INVALID_HANDLE_VALUE)return;
    post_save_frames=2;
    InterlockedExchange(&save_time,(LONG)GetTickCount());
    InterlockedExchange(&progress_time,(LONG)GetTickCount());
    if(InterlockedCompareExchange(&monitor_started,1,0))return;
    thread=CreateThread(NULL,0,post_save_monitor,NULL,0,NULL);
    if(thread)CloseHandle(thread);
    else{InterlockedExchange(&monitor_started,0);restore_game_trace("post-save monitor start error=%lu\n",GetLastError());}
}
int restore_game_trace_post_save_frame(void)
{if(!post_save_frames)return 0;--post_save_frames;return 1;}
