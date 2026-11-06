#include "crt_lowio.h"
#include <windows.h>

int32_t restore_crt_close(uint32_t descriptor)
{
 if(descriptor<RESTORE_CRT_LOWIO_CAPACITY){
  uint32_t offset=(descriptor&31u)*8u,error=0;
  RestoreCrtLowioRecord *volatile *slot=(RestoreCrtLowioRecord *volatile *)(uintptr_t)
   ((uint32_t)(uintptr_t)RESTORE_CRT_LOWIO_BLOCKS+
    (uint32_t)((int32_t)descriptor>>5)*4u);
  volatile RestoreCrtLowioRecord *record=(volatile RestoreCrtLowioRecord *)(uintptr_t)
   ((uint32_t)(uintptr_t)*slot+offset);
  if((record->flags&1u)!=0){
   if(FUN_005669c6(descriptor)!=0xffffffffu){
    uint32_t shared=0;
    if(descriptor==1 || descriptor==2){
     uint32_t stderr_handle=FUN_005669c6(2);
     uint32_t stdout_handle=FUN_005669c6(1);
     shared=stderr_handle==stdout_handle;
    }
    if(!shared){
     uint32_t handle=FUN_005669c6(descriptor);
     if(!CloseHandle((HANDLE)(uintptr_t)handle))error=GetLastError();
    }
   }
   (void)FUN_0056694c(descriptor);
   /* Native retains the original cell/offset through all callbacks. */
   record=(volatile RestoreCrtLowioRecord *)(uintptr_t)
    ((uint32_t)(uintptr_t)*slot+offset);
   record->flags=0;
   if(error!=0){FUN_005672c8(error);return -1;}
   return 0;
  }
 }
 RESTORE_CRT_DOSERRNO=0;
 RESTORE_CRT_ERRNO=9;
 return -1;
}
int32_t FUN_005647eb(uint32_t descriptor)
{return restore_crt_close(descriptor);}
