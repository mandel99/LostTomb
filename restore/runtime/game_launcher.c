#include "game_launcher.h"
#include "media_loader.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

enum { LAUNCH_PATH_CAPACITY=32768 };
typedef struct {
    wchar_t executable[LAUNCH_PATH_CAPACITY];
    wchar_t directory[LAUNCH_PATH_CAPACITY];
    wchar_t command[LAUNCH_PATH_CAPACITY];
    wchar_t layers[LAUNCH_PATH_CAPACITY];
} RestoreLaunchBuffers;

static int current_has_assets(void)
{
    DWORD config=GetFileAttributesW(L"Pharaoh.inf");
    DWORD language=GetFileAttributesW(L"Language.inf");
    DWORD data=GetFileAttributesW(L"Data");
    return config!=INVALID_FILE_ATTRIBUTES&&!(config&FILE_ATTRIBUTE_DIRECTORY)&&
        language!=INVALID_FILE_ATTRIBUTES&&!(language&FILE_ATTRIBUTE_DIRECTORY)&&
        data!=INVALID_FILE_ATTRIBUTES&&(data&FILE_ATTRIBUTE_DIRECTORY);
}
static int select_assets(RestoreLaunchBuffers *buffer)
{
    wchar_t *separator;
    DWORD length;
    if(current_has_assets())return 1;
    length=GetCurrentDirectoryW(LAUNCH_PATH_CAPACITY,buffer->directory);
    if(!length||length>=LAUNCH_PATH_CAPACITY)return 0;
    if(wcscpy_s(buffer->command,LAUNCH_PATH_CAPACITY,buffer->executable))return 0;
    separator=wcsrchr(buffer->command,L'\\');if(!separator)return 0;*separator=0;
    if(SetCurrentDirectoryW(buffer->command)){
        if(current_has_assets())return 1;
        /* Development build: build/c-only/Release alongside the already
           prepared build/debug-startup-game assets. No original EXE is read. */
        if(SetCurrentDirectoryW(L"..\\..\\debug-startup-game")&&current_has_assets())return 1;
    }
    (void)SetCurrentDirectoryW(buffer->directory);
    fputs("Game data not found. Run from the game folder, or place this EXE beside Pharaoh.inf, Language.inf and Data.\n",stderr);
    return 0;
}
static int has_16bit_layer(const wchar_t *layers)
{
    const wchar_t *start=layers;
    while(*start){
        const wchar_t *end;
        while(*start==L' '||*start==L'\t')++start;
        end=start;while(*end&&*end!=L' '&&*end!=L'\t')++end;
        if(end-start==10&&!_wcsnicmp(start,L"16BITCOLOR",10))return 1;
        start=end;
    }
    return 0;
}
int restore_game_prepare_launch(int *code)
{
    RestoreLaunchBuffers *buffer;
    DWORD length,error,child_code=1;
    HDC screen;
    int depth;
    HANDLE job=NULL;
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION limits={0};
    STARTUPINFOW startup={0};PROCESS_INFORMATION child={0};
    *code=1;
    buffer=(RestoreLaunchBuffers *)calloc(1,sizeof(*buffer));
    if(!buffer){fputs("Not enough memory for game startup.\n",stderr);return 1;}
    length=GetModuleFileNameW(NULL,buffer->executable,LAUNCH_PATH_CAPACITY);
    if(!length||length>=LAUNCH_PATH_CAPACITY||!select_assets(buffer)){free(buffer);return 1;}
    length=GetCurrentDirectoryW(LAUNCH_PATH_CAPACITY,buffer->directory);
    if(!length||length>=LAUNCH_PATH_CAPACITY||!restore_media_set_game_directory(buffer->directory)){
        fputs("Cannot select media libraries from the game data directory.\n",stderr);
        free(buffer);return 1;
    }
    screen=GetDC(NULL);
    depth=screen?GetDeviceCaps(screen,BITSPIXEL)*GetDeviceCaps(screen,PLANES):0;
    if(screen)ReleaseDC(NULL,screen);
    length=GetEnvironmentVariableW(L"__COMPAT_LAYER",buffer->layers,LAUNCH_PATH_CAPACITY);
    if(length>=LAUNCH_PATH_CAPACITY){fputs("Color compatibility configuration is too long.\n",stderr);free(buffer);return 1;}
    if((depth>0&&depth<=16)||has_16bit_layer(buffer->layers)){free(buffer);return 0;}
    /* Windows applies these shims when creating the process, so setting the
       environment after CRT startup alone cannot repair DirectDraw setup.
       The inherited16BITCOLOR token also prevents recursive relaunches. */
    if(wcscat_s(buffer->layers,LAUNCH_PATH_CAPACITY,L" 16BITCOLOR HIGHDPIAWARE DWM8And16BitMitigation")||
       !SetEnvironmentVariableW(L"__COMPAT_LAYER",buffer->layers)||
       wcscpy_s(buffer->command,LAUNCH_PATH_CAPACITY,GetCommandLineW())){
        fputs("Cannot prepare 16-bit game startup.\n",stderr);free(buffer);return 1;
    }
    {
        char trace[8];
        if(GetEnvironmentVariableA("RESTORE_TRACE_STARTUP",trace,sizeof(trace))==1&&trace[0]=='1'){
            fputs("STARTUP launcher: restarting the game with 16-bit color compatibility.\n",stderr);fflush(stderr);
        }
    }
    /* Keep only our new child in an owned job. A debugger terminating this
       waiting launcher must also stop its game child, never leave an orphan. */
    job=CreateJobObjectW(NULL,NULL);
    limits.BasicLimitInformation.LimitFlags=JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if(!job||!SetInformationJobObject(job,JobObjectExtendedLimitInformation,&limits,sizeof(limits))){
        error=GetLastError();goto failed;
    }
    startup.cb=sizeof(startup);startup.dwFlags=STARTF_USESTDHANDLES;
    startup.hStdInput=GetStdHandle(STD_INPUT_HANDLE);
    startup.hStdOutput=GetStdHandle(STD_OUTPUT_HANDLE);
    startup.hStdError=GetStdHandle(STD_ERROR_HANDLE);
    if(!CreateProcessW(buffer->executable,buffer->command,NULL,NULL,TRUE,CREATE_SUSPENDED,NULL,NULL,&startup,&child)){
        error=GetLastError();goto failed;
    }
    if(!AssignProcessToJobObject(job,child.hProcess)||ResumeThread(child.hThread)==(DWORD)-1){
        error=GetLastError();TerminateProcess(child.hProcess,1);goto failed;
    }
    CloseHandle(child.hThread);child.hThread=NULL;
    free(buffer);buffer=NULL;
    if(WaitForSingleObject(child.hProcess,INFINITE)!=WAIT_OBJECT_0||!GetExitCodeProcess(child.hProcess,&child_code)){
        error=GetLastError();goto failed;
    }
    CloseHandle(child.hProcess);CloseHandle(job);*code=(int)child_code;return 1;
failed:
    if(child.hThread)CloseHandle(child.hThread);
    if(child.hProcess)CloseHandle(child.hProcess);
    if(job)CloseHandle(job);
    free(buffer);fprintf(stderr,"Cannot start the game in 16-bit compatibility mode (Windows error %lu).\n",error);
    return 1;
}
