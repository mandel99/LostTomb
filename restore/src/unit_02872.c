#include "crt_runtime_init.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
typedef BOOL(WINAPI *RestoreProcessorFeatureQuery)(DWORD);

uint32_t restore_crt_query_fdiv_erratum(void)
{
 HMODULE module=GetModuleHandleA((LPCSTR)(uintptr_t)0x5706dcu);
 FARPROC query;
 if(module){
  query=GetProcAddress(module,(LPCSTR)(uintptr_t)0x5706c0u);
  if(query){
   /* The pinned symbol is IsProcessorFeaturePresent(DWORD), WINAPI. */
   #pragma warning(suppress:4191)
   return(uint32_t)((RestoreProcessorFeatureQuery)query)(0u);
  }
 }
 return restore_crt_probe_fdiv_erratum();
}
uint32_t FUN_0056413c(void){return restore_crt_query_fdiv_erratum();}
