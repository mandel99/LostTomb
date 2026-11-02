#define WIN32_LEAN_AND_MEAN
#include "media_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { const char *name; INIT_ONCE once; HMODULE module; DWORD error; } MediaModule;
static MediaModule modules[]={
    {"mss32.dll",INIT_ONCE_STATIC_INIT,NULL,0},
    {"binkw32.dll",INIT_ONCE_STATIC_INIT,NULL,0},
    {"smackw32.dll",INIT_ONCE_STATIC_INIT,NULL,0}
};
static BOOL CALLBACK load_module(PINIT_ONCE once,PVOID parameter,PVOID *context)
{
    MediaModule *module=(MediaModule *)parameter;
    char filename[32768],*separator;
    DWORD length=GetModuleFileNameA(NULL,filename,sizeof(filename));
    (void)once; (void)context;
    if(length==0 || length>=sizeof(filename)) { module->error=ERROR_INSUFFICIENT_BUFFER; return TRUE; }
    separator=strrchr(filename,'\\');
    if(!separator) { module->error=ERROR_BAD_PATHNAME; return TRUE; }
    strcpy_s(separator+1,sizeof(filename)-(size_t)(separator+1-filename),module->name);
    module->module=LoadLibraryExA(filename,NULL,LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR|LOAD_LIBRARY_SEARCH_SYSTEM32);
    if(!module->module) module->error=GetLastError();
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
