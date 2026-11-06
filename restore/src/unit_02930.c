#include "crt_lowio.h"

typedef struct RestoreCrtOsErrorMapping {
 uint32_t os_error,crt_error;
} RestoreCrtOsErrorMapping;
typedef char RestoreCrtOsErrorMappingLayout[(sizeof(RestoreCrtOsErrorMapping)==8 &&
 offsetof(RestoreCrtOsErrorMapping,crt_error)==4)?1:-1];
#define RESTORE_CRT_OS_ERROR_MAP ((volatile const RestoreCrtOsErrorMapping *)0x00604f30u)

/* Retain the45-pair lookup and ordered fallback stores. Native scratch EAX,
   ECX/EDX and flags are not exposed as invented C inputs or return values. */
void restore_crt_translate_os_error(uint32_t error)
{
 uint32_t i;
 RESTORE_CRT_DOSERRNO=error;
 for(i=0;i<45;++i){
  if(error==RESTORE_CRT_OS_ERROR_MAP[i].os_error){
   RESTORE_CRT_ERRNO=RESTORE_CRT_OS_ERROR_MAP[i].crt_error;
   return;
  }
 }
 if(error>=0x13u && error<=0x24u){RESTORE_CRT_ERRNO=13;return;}
 if(error>=0xbcu){
  RESTORE_CRT_ERRNO=8;
  if(error<=0xcau)return;
 }
 RESTORE_CRT_ERRNO=22;
}
void FUN_005672c8(uint32_t error)
{restore_crt_translate_os_error(error);}
