#define WIN32_LEAN_AND_MEAN
#include "media_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

enum { MEDIA_PATH_CAPACITY=32768 };
static wchar_t game_directory[MEDIA_PATH_CAPACITY];
typedef struct { const char *name; const wchar_t *filename; INIT_ONCE once; HMODULE module; DWORD error; } MediaModule;
static MediaModule modules[]={
    {"mss32.dll",L"mss32.dll",INIT_ONCE_STATIC_INIT,NULL,0},
    {"binkw32.dll",L"binkw32.dll",INIT_ONCE_STATIC_INIT,NULL,0},
    {"smackw32.dll",L"smackw32.dll",INIT_ONCE_STATIC_INIT,NULL,0}
};
int restore_media_set_game_directory(const wchar_t *directory)
{
    size_t length;
    game_directory[0]=0;
    if(!directory)return 1;
    length=wcslen(directory);
    if(length<3u||length>MEDIA_PATH_CAPACITY-14u)return 0;
    if(!((directory[1]==L':'&&(directory[2]==L'\\'||directory[2]==L'/'))||
         (directory[0]==L'\\'&&directory[1]==L'\\')))return 0;
    return wcscpy_s(game_directory,MEDIA_PATH_CAPACITY,directory)==0;
}
static BOOL CALLBACK load_module(PINIT_ONCE once,PVOID parameter,PVOID *context)
{
    MediaModule *module=(MediaModule *)parameter;
    wchar_t filename[MEDIA_PATH_CAPACITY],*separator;
    DWORD length=GetModuleFileNameW(NULL,filename,MEDIA_PATH_CAPACITY);
    size_t directory_length;
    (void)once; (void)context;
    module->error=ERROR_MOD_NOT_FOUND;
    if(length&&length<MEDIA_PATH_CAPACITY&&(separator=wcsrchr(filename,L'\\'))!=NULL){
        if(wcslen(module->filename)<MEDIA_PATH_CAPACITY-(size_t)(separator+1-filename)){
            wcscpy_s(separator+1,MEDIA_PATH_CAPACITY-(size_t)(separator+1-filename),module->filename);
            module->module=LoadLibraryExW(filename,NULL,LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR|LOAD_LIBRARY_SEARCH_SYSTEM32);
            if(module->module){module->error=0;return TRUE;}
            module->error=GetLastError();
        }else module->error=ERROR_INSUFFICIENT_BUFFER;
    }else module->error=length&&length<MEDIA_PATH_CAPACITY?ERROR_BAD_PATHNAME:ERROR_INSUFFICIENT_BUFFER;
    /* The C launcher can select game assets separately from its EXE. Keep
       executable-local DLL precedence, then use that exact selected folder. */
    directory_length=wcslen(game_directory);
    if(directory_length){
        wcscpy_s(filename,MEDIA_PATH_CAPACITY,game_directory);
        if(filename[directory_length-1u]!=L'\\'&&filename[directory_length-1u]!=L'/')
            wcscat_s(filename,MEDIA_PATH_CAPACITY,L"\\");
        wcscat_s(filename,MEDIA_PATH_CAPACITY,module->filename);
        module->module=LoadLibraryExW(filename,NULL,LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR|LOAD_LIBRARY_SEARCH_SYSTEM32);
        module->error=module->module?0:GetLastError();
    }
    return TRUE;
}
FARPROC restore_media_resolve(const char *dll,const char *export_name)
{
    unsigned int i;
    FARPROC function=NULL;
    DWORD error=ERROR_MOD_NOT_FOUND;
    for(i=0;i<sizeof(modules)/sizeof(modules[0]);++i) {
        MediaModule *module=&modules[i];
        if(strcmp(dll,module->name)!=0) continue;
        if(!InitOnceExecuteOnce(&module->once,load_module,module,NULL)) error=GetLastError();
        else if(!module->module) error=module->error;
        else {
            function=GetProcAddress(module->module,export_name);
            if(!function) error=GetLastError();
        }
        break;
    }
    if(!function) {
        fprintf(stderr,"Required media export unavailable: %s!%s (Windows error %lu).\n",dll,export_name,error);
        abort();
    }
    return function;
}
