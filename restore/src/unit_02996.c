#include "crt_multibyte.h"

uint32_t restore_crt_resolve_codepage(int32_t requested)
{
 RESTORE_CRT_DEFAULT_CODEPAGE=0;
 if(requested==-2){RESTORE_CRT_DEFAULT_CODEPAGE=1;return GetOEMCP();}
 if(requested==-3){RESTORE_CRT_DEFAULT_CODEPAGE=1;return GetACP();}
 if(requested==-4){
  uint32_t codepage=RESTORE_CRT_LOCALE_CODEPAGE;
  RESTORE_CRT_DEFAULT_CODEPAGE=1;
  return codepage;
 }
 return (uint32_t)requested;
}
uint32_t FUN_005699ca(int32_t requested){return restore_crt_resolve_codepage(requested);}
