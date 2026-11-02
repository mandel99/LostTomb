/* Bounded debugger for a newly owned x86 child; never attaches to user processes. */
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <dbghelp.h>
#include <stdlib.h>
#include <string.h>

typedef struct {DWORD address;BYTE original;int armed;} RestoreBreakpoint;
static RestoreBreakpoint breakpoints[8];
static unsigned int breakpoint_count;
static void snapshot(HANDLE process,HANDLE thread,DWORD thread_id,DWORD elapsed)
{
    CONTEXT context={0};BYTE code[24]={0};DWORD stack[128]={0};
    SIZE_T read=0;unsigned int i;
    if(SuspendThread(thread)==(DWORD)-1) {
        printf("SNAPSHOT unavailable thread=%lu error=%lu\n",thread_id,GetLastError());return;
    }
    context.ContextFlags=CONTEXT_FULL;
    if(GetThreadContext(thread,&context)) {
        printf("PERIODIC SNAPSHOT elapsed=%lu thread=%lu EIP=%08lx ESP=%08lx EBP=%08lx EAX=%08lx EBX=%08lx ECX=%08lx EDX=%08lx ESI=%08lx EDI=%08lx\n",
            elapsed,thread_id,context.Eip,context.Esp,context.Ebp,context.Eax,context.Ebx,context.Ecx,context.Edx,context.Esi,context.Edi);
        if(ReadProcessMemory(process,(void *)(uintptr_t)context.Eip,code,sizeof(code),&read)) {
            printf("SNAPSHOT CODE:");for(i=0;i<read;++i)printf(" %02x",code[i]);puts("");
        }
        read=0;
        if(ReadProcessMemory(process,(void *)(uintptr_t)context.Esp,stack,sizeof(stack),&read)) {
            for(i=0;i<read/sizeof(stack[0]);++i) {
                if(!(i%8))printf("SNAPSHOT STACK %08lx:",context.Esp+i*4u);
                printf(" %08lx",stack[i]);if(i%8==7)puts("");
            }
            if(i%8)puts("");
        }
    } else printf("SNAPSHOT context unavailable thread=%lu error=%lu\n",thread_id,GetLastError());
    /* Always undo only our own extra suspension, including read failures. */
    if(ResumeThread(thread)==(DWORD)-1)printf("SNAPSHOT resume failed thread=%lu error=%lu\n",thread_id,GetLastError());
}
static void dump_watched_words(HANDLE process)
{
    char text[256],*cursor,*end;DWORD length;
    unsigned int count=0;
    length=GetEnvironmentVariableA("RESTORE_DEBUG_READ_AT",text,sizeof(text));
    if(!length)return;
    if(length>=sizeof(text)) {puts("WATCH list too long.");return;}
    cursor=text;
    while(*cursor&&count++<8) {
        DWORD address=strtoul(cursor,&end,16),value=0;SIZE_T read=0;
        if(end==cursor||(*end&&*end!=',')) {puts("Invalid WATCH address.");return;}
        if(ReadProcessMemory(process,(void *)(uintptr_t)address,&value,sizeof(value),&read)&&read==sizeof(value))
            printf("WATCH %08lx=%08lx\n",address,value);
        else printf("WATCH %08lx unavailable\n",address);
        cursor=*end?end+1:end;
    }
}
static int child_compatibility(void)
{
    char enabled[8],layers[2048];
    DWORD length=GetEnvironmentVariableA("RESTORE_DEBUG_COMPAT16",enabled,sizeof(enabled));
    if(!length)return 1;
    if(length>=sizeof(enabled)||strcmp(enabled,"1"))return 0;
    length=GetEnvironmentVariableA("__COMPAT_LAYER",layers,sizeof(layers));
    if(length>=sizeof(layers)-80)return 0;
    if(!length)layers[0]=0;
    strcat_s(layers,sizeof(layers)," 16BITCOLOR HIGHDPIAWARE DWM8And16BitMitigation");
    if(!SetEnvironmentVariableA("__COMPAT_LAYER",layers))return 0;
    printf("CHILD COMPATIBILITY:%s\n",layers);
    return 1;
}
static int parse_breakpoints(void)
{
    char text[256],*cursor,*end;
    DWORD length=GetEnvironmentVariableA("RESTORE_DEBUG_BREAK_AT",text,sizeof(text));
    if(!length)return 1;
    if(length>=sizeof(text))return 0;
    cursor=text;
    while(*cursor) {
        unsigned long value=strtoul(cursor,&end,16);
        unsigned int i;
        if(end==cursor||!value||(*end&&*end!=',')||breakpoint_count==8)return 0;
        for(i=0;i<breakpoint_count;++i)if(breakpoints[i].address==value)return 0;
        breakpoints[breakpoint_count++].address=value;
        cursor=*end?end+1:end;
        if(*end&&!*cursor)return 0;
    }
    return 1;
}
static int code_byte(HANDLE process,DWORD address,BYTE value)
{
    DWORD old,ignored;SIZE_T written=0;
    int okay;
    if(!VirtualProtectEx(process,(void *)(uintptr_t)address,1,PAGE_EXECUTE_READWRITE,&old))return 0;
    okay=WriteProcessMemory(process,(void *)(uintptr_t)address,&value,1,&written)&&written==1;
    if(!VirtualProtectEx(process,(void *)(uintptr_t)address,1,old,&ignored))okay=0;
    if(!FlushInstructionCache(process,(void *)(uintptr_t)address,1))okay=0;
    return okay;
}
static int arm_breakpoints(HANDLE process,void *image)
{
    unsigned int i;
    for(i=0;i<breakpoint_count;++i) {
        MEMORY_BASIC_INFORMATION memory;SIZE_T read=0;
        RestoreBreakpoint *point=&breakpoints[i];
        if(!VirtualQueryEx(process,(void *)(uintptr_t)point->address,&memory,sizeof(memory))||
           memory.AllocationBase!=image||memory.Type!=MEM_IMAGE||
           !(memory.Protect&(PAGE_EXECUTE|PAGE_EXECUTE_READ|PAGE_EXECUTE_READWRITE|PAGE_EXECUTE_WRITECOPY))||
           !ReadProcessMemory(process,(void *)(uintptr_t)point->address,&point->original,1,&read)||read!=1||
           !code_byte(process,point->address,0xcc))return 0;
        point->armed=1;printf("ONE-SHOT BREAKPOINT %08lx\n",point->address);
    }
    return 1;
}

static void trace(HANDLE process,HANDLE thread,CONTEXT original,const char *executable)
{
    STACKFRAME64 frame={0};
    CONTEXT context=original;
    char directory[32768],*separator;
    unsigned int i;
    strcpy_s(directory,sizeof(directory),executable);
    separator=strrchr(directory,'\\');if(!separator)separator=strrchr(directory,'/');
    if(separator)*separator=0;
    SymSetOptions(SYMOPT_LOAD_LINES|SYMOPT_UNDNAME|SYMOPT_NO_PROMPTS|SYMOPT_FAIL_CRITICAL_ERRORS);
    if(!SymInitialize(process,directory,TRUE)) {printf("SYMBOLS unavailable %lu\n",GetLastError());return;}
    frame.AddrPC.Offset=context.Eip;frame.AddrPC.Mode=AddrModeFlat;
    frame.AddrStack.Offset=context.Esp;frame.AddrStack.Mode=AddrModeFlat;
    frame.AddrFrame.Offset=context.Ebp;frame.AddrFrame.Mode=AddrModeFlat;
    for(i=0;i<24;++i) {
        unsigned char storage[sizeof(SYMBOL_INFO)+MAX_SYM_NAME];
        SYMBOL_INFO *symbol=(SYMBOL_INFO *)storage;
        IMAGEHLP_LINE64 line={sizeof(line)};
        DWORD64 displacement=0;DWORD line_displacement=0;
        memset(storage,0,sizeof(storage));symbol->SizeOfStruct=sizeof(*symbol);symbol->MaxNameLen=MAX_SYM_NAME;
        printf("FRAME %u %08llx",i,frame.AddrPC.Offset);
        if(SymFromAddr(process,frame.AddrPC.Offset,&displacement,symbol))printf(" %s+%llu",symbol->Name,displacement);
        if(SymGetLineFromAddr64(process,frame.AddrPC.Offset,&line_displacement,&line))printf(" %s:%lu",line.FileName,line.LineNumber);
        puts("");
        if(!StackWalk64(IMAGE_FILE_MACHINE_I386,process,thread,&frame,&context,NULL,SymFunctionTableAccess64,SymGetModuleBase64,NULL)||!frame.AddrPC.Offset)break;
    }
    SymCleanup(process);
}

int main(int argc,char **argv)
{
    STARTUPINFOA startup={sizeof(startup)};
    PROCESS_INFORMATION process;
    DEBUG_EVENT event;
    char command[32768];
    DWORD started=GetTickCount();
    DWORD timeout=15000,exit_code=0,snapshot_interval=0,last_snapshot=0;
    char timeout_text[24];
    int initial_breakpoint=0,failed=0;
    int i;
    if (argc<2) return 2;
    if(!parse_breakpoints()) {fputs("Invalid RESTORE_DEBUG_BREAK_AT: use up to eight distinct executable addresses in hex.\n",stderr);return 2;}
    sprintf_s(command,sizeof(command),"\"%s\"",argv[1]);
    for (i=2;i<argc;i++) { strcat_s(command,sizeof(command)," \""); strcat_s(command,sizeof(command),argv[i]); strcat_s(command,sizeof(command),"\""); }
    setvbuf(stdout,NULL,_IONBF,0);
    if(!child_compatibility()) {fputs("Invalid child compatibility configuration.\n",stderr);return 2;}
    if(GetEnvironmentVariableA("RESTORE_DEBUG_TIMEOUT_MS",timeout_text,sizeof(timeout_text))) {
        unsigned long requested=strtoul(timeout_text,NULL,10);
        if(requested>=1000&&requested<=300000)timeout=requested;
    }
    if(GetEnvironmentVariableA("RESTORE_DEBUG_SNAPSHOT_MS",timeout_text,sizeof(timeout_text))) {
        unsigned long requested=strtoul(timeout_text,NULL,10);
        if(requested>=1000&&requested<=60000)snapshot_interval=requested;
        else {fputs("Invalid snapshot interval: use1000..60000 milliseconds.\n",stderr);return 2;}
    }
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);
    startup.dwFlags=STARTF_USESTDHANDLES;
    startup.hStdInput=GetStdHandle(STD_INPUT_HANDLE);startup.hStdOutput=GetStdHandle(STD_OUTPUT_HANDLE);startup.hStdError=GetStdHandle(STD_ERROR_HANDLE);
    SetHandleInformation(startup.hStdInput,HANDLE_FLAG_INHERIT,HANDLE_FLAG_INHERIT);
    SetHandleInformation(startup.hStdOutput,HANDLE_FLAG_INHERIT,HANDLE_FLAG_INHERIT);
    SetHandleInformation(startup.hStdError,HANDLE_FLAG_INHERIT,HANDLE_FLAG_INHERIT);
    if (!CreateProcessA(argv[1],command,NULL,NULL,TRUE,DEBUG_ONLY_THIS_PROCESS|CREATE_NO_WINDOW,NULL,NULL,&startup,&process)) {
        printf("CreateProcess failed %lu\n",GetLastError()); return 2;
    }
    for (;;) {
        DWORD disposition=DBG_CONTINUE;
        if (GetTickCount()-started>timeout) {
            CONTEXT context={0};
            puts("DEBUG TIMEOUT: snapshot and termination of only the owned child.");
            if(SuspendThread(process.hThread)!=(DWORD)-1) {
                context.ContextFlags=CONTEXT_FULL;
                if(GetThreadContext(process.hThread,&context))trace(process.hProcess,process.hThread,context,argv[1]);
                dump_watched_words(process.hProcess);
                ResumeThread(process.hThread);
            }
            TerminateProcess(process.hProcess,124);exit_code=124;failed=1;break;
        }
        if (!WaitForDebugEvent(&event,1000)) {
            DWORD elapsed=GetTickCount()-started;
            if(snapshot_interval&&elapsed-last_snapshot>=snapshot_interval) {
                snapshot(process.hProcess,process.hThread,process.dwThreadId,elapsed);
                dump_watched_words(process.hProcess);last_snapshot=elapsed;
            }
            continue;
        }
        if (event.dwDebugEventCode==LOAD_DLL_DEBUG_EVENT || event.dwDebugEventCode==CREATE_PROCESS_DEBUG_EVENT) {
            HANDLE file=event.dwDebugEventCode==LOAD_DLL_DEBUG_EVENT ? event.u.LoadDll.hFile : event.u.CreateProcessInfo.hFile;
            void *base=event.dwDebugEventCode==LOAD_DLL_DEBUG_EVENT ? event.u.LoadDll.lpBaseOfDll : event.u.CreateProcessInfo.lpBaseOfImage;
            char name[MAX_PATH*4]="<unavailable>";
            if (file) { GetFinalPathNameByHandleA(file,name,sizeof(name),0); CloseHandle(file); }
            printf("MODULE %p %s\n",base,name);
            if(event.dwDebugEventCode==CREATE_PROCESS_DEBUG_EVENT&&!arm_breakpoints(process.hProcess,base)) {
                puts("Cannot safely arm the requested breakpoints in the owned executable.");
                failed=1;TerminateProcess(process.hProcess,2);
            }
        } else if (event.dwDebugEventCode==EXCEPTION_DEBUG_EVENT) {
            EXCEPTION_DEBUG_INFO *exception=&event.u.Exception;
            HANDLE thread=OpenThread(THREAD_GET_CONTEXT|THREAD_SET_CONTEXT|THREAD_QUERY_INFORMATION,FALSE,event.dwThreadId);
            CONTEXT context={0};
            int observed=0;
            context.ContextFlags=CONTEXT_FULL;
            GetThreadContext(thread,&context);
            printf("EXCEPTION %08lx at %p first=%lu EIP=%08lx ESP=%08lx EAX=%08lx ECX=%08lx EDX=%08lx\n",
                exception->ExceptionRecord.ExceptionCode,exception->ExceptionRecord.ExceptionAddress,exception->dwFirstChance,
                context.Eip,context.Esp,context.Eax,context.Ecx,context.Edx);
            if(exception->ExceptionRecord.ExceptionCode==EXCEPTION_BREAKPOINT)for(i=0;i<(int)breakpoint_count;++i) {
                RestoreBreakpoint *point=&breakpoints[i];
                if(point->armed&&(uintptr_t)exception->ExceptionRecord.ExceptionAddress==point->address) {
                    printf("OBSERVED BREAKPOINT %08lx EAX=%08lx EBX=%08lx ECX=%08lx EDX=%08lx\n",point->address,context.Eax,context.Ebx,context.Ecx,context.Edx);
                    dump_watched_words(process.hProcess);
                    trace(process.hProcess,thread,context,argv[1]);
                    if(!code_byte(process.hProcess,point->address,point->original)) {failed=1;TerminateProcess(process.hProcess,2);}
                    context.Eip=point->address;
                    if(!SetThreadContext(thread,&context)) {failed=1;TerminateProcess(process.hProcess,2);}
                    point->armed=0;observed=1;break;
                }
            }
            if(observed) {
                /* Resume at the restored instruction; this location is observed once. */
            } else if(exception->ExceptionRecord.ExceptionCode==EXCEPTION_BREAKPOINT&&!initial_breakpoint) {
                initial_breakpoint=1;puts("INITIAL LOADER BREAKPOINT");
            } else if(exception->ExceptionRecord.ExceptionCode==0x406d1388u) {
                puts("MSVC DEBUG NOTIFICATION; allow the application handler.");
                disposition=DBG_EXCEPTION_NOT_HANDLED;
            } else {
                BYTE code[16]={0};
                DWORD stack[16]={0};
                SIZE_T read;
                ReadProcessMemory(process.hProcess,(void *)(uintptr_t)context.Eip,code,sizeof(code),&read);
                printf("CODE:"); for (i=0;i<sizeof(code);i++) printf(" %02x",code[i]); puts("");
                ReadProcessMemory(process.hProcess,(void *)(uintptr_t)context.Esp,stack,sizeof(stack),&read);
                printf("STACK:"); for (i=0;i<16;i++) printf(" %08lx",stack[i]); puts("");
                printf("EXCEPTION DATA: %08lx %08lx\n",(DWORD)exception->ExceptionRecord.ExceptionInformation[0],(DWORD)exception->ExceptionRecord.ExceptionInformation[1]);
                trace(process.hProcess,thread,context,argv[1]);
                dump_watched_words(process.hProcess);
                disposition=DBG_EXCEPTION_NOT_HANDLED;
                if (!exception->dwFirstChance||exception->ExceptionRecord.ExceptionCode==EXCEPTION_ACCESS_VIOLATION||exception->ExceptionRecord.ExceptionCode==EXCEPTION_BREAKPOINT) {
                    failed=1;TerminateProcess(process.hProcess,exception->ExceptionRecord.ExceptionCode);
                }
            }
            if(thread)CloseHandle(thread);
        } else if(event.dwDebugEventCode==OUTPUT_DEBUG_STRING_EVENT) {
            OUTPUT_DEBUG_STRING_INFO *info=&event.u.DebugString;
            if(!info->fUnicode) {
                char message[4096]={0};SIZE_T read=0,n=info->nDebugStringLength;
                if(n>=sizeof(message))n=sizeof(message)-1;
                if(ReadProcessMemory(process.hProcess,info->lpDebugStringData,message,n,&read))printf("DEBUG OUTPUT: %s\n",message);
            } else {
                wchar_t wide[2048]={0};char message[8192]={0};SIZE_T read=0,n=info->nDebugStringLength;
                if(n>=2048)n=2047;
                if(ReadProcessMemory(process.hProcess,info->lpDebugStringData,wide,n*sizeof(wchar_t),&read)&&
                   WideCharToMultiByte(CP_UTF8,0,wide,-1,message,sizeof(message),NULL,NULL))printf("DEBUG OUTPUT: %s\n",message);
            }
        } else if(event.dwDebugEventCode==CREATE_THREAD_DEBUG_EVENT) {
            CloseHandle(event.u.CreateThread.hThread);
        } else if (event.dwDebugEventCode==EXIT_PROCESS_DEBUG_EVENT) {
            printf("EXIT %08lx\n",event.u.ExitProcess.dwExitCode);
            exit_code=event.u.ExitProcess.dwExitCode;
            ContinueDebugEvent(event.dwProcessId,event.dwThreadId,DBG_CONTINUE);
            break;
        }
        ContinueDebugEvent(event.dwProcessId,event.dwThreadId,disposition);
    }
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    return failed||exit_code?1:0;
}
