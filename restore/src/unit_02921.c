#include "crt_lowio.h"
#include <windows.h>

uint32_t restore_crt_free_os_handle(uint32_t descriptor)
{
 if(descriptor<RESTORE_CRT_LOWIO_CAPACITY){
  uint32_t offset=(descriptor&31u)*8u;
  RestoreCrtLowioRecord *volatile *slot=(RestoreCrtLowioRecord *volatile *)(uintptr_t)
   ((uint32_t)(uintptr_t)RESTORE_CRT_LOWIO_BLOCKS+
    (uint32_t)((int32_t)descriptor>>5)*4u);
  volatile RestoreCrtLowioRecord *record=(volatile RestoreCrtLowioRecord *)(uintptr_t)
   ((uint32_t)(uintptr_t)*slot+offset);
  if((record->flags&1u)!=0 && record->handle!=0xffffffffu){
   if(RESTORE_CRT_LOWIO_STD_HANDLE_MODE==1 && descriptor<=2)
    (void)SetStdHandle(0xfffffff6u-descriptor,(HANDLE)0);
   /* Free clears only the handle. Flags/text state/reserved bytes retain
      their current values in the freshly reloaded target block. */
   record=(volatile RestoreCrtLowioRecord *)(uintptr_t)
    ((uint32_t)(uintptr_t)*slot+offset);
   record->handle=0xffffffffu;
   return 0;
  }
 }
 RESTORE_CRT_DOSERRNO=0;
 RESTORE_CRT_ERRNO=9;
 return 0xffffffffu;
}
uint32_t FUN_0056694c(uint32_t descriptor)
{return restore_crt_free_os_handle(descriptor);}
