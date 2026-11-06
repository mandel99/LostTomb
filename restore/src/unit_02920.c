#include "crt_lowio.h"
#include <windows.h>

uint32_t restore_crt_set_os_handle(uint32_t descriptor,uint32_t handle_bits)
{
 if(descriptor<RESTORE_CRT_LOWIO_CAPACITY){
  uint32_t offset=(descriptor&31u)*8u;
  RestoreCrtLowioRecord *volatile *slot=(RestoreCrtLowioRecord *volatile *)(uintptr_t)
   ((uint32_t)(uintptr_t)RESTORE_CRT_LOWIO_BLOCKS+
    (uint32_t)((int32_t)descriptor>>5)*4u);
  volatile RestoreCrtLowioRecord *record=(volatile RestoreCrtLowioRecord *)(uintptr_t)
   ((uint32_t)(uintptr_t)*slot+offset);
  if(record->handle==0xffffffffu){
   if(RESTORE_CRT_LOWIO_STD_HANDLE_MODE==1 && descriptor<=2)
    (void)SetStdHandle(0xfffffff6u-descriptor,(HANDLE)(uintptr_t)handle_bits);
   /* Native retains the slot address/offset, then reloads this cell even if
      the callback changed capacity, block pointer or record validity. */
   record=(volatile RestoreCrtLowioRecord *)(uintptr_t)
    ((uint32_t)(uintptr_t)*slot+offset);
   record->handle=handle_bits;
   return 0;
  }
 }
 RESTORE_CRT_DOSERRNO=0;
 RESTORE_CRT_ERRNO=9;
 return 0xffffffffu;
}
uint32_t FUN_005668d5(uint32_t descriptor,uint32_t handle_bits)
{return restore_crt_set_os_handle(descriptor,handle_bits);}
