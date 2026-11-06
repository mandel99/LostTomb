#include "crt_lowio.h"
#include <windows.h>

uint32_t restore_crt_seek(uint32_t descriptor,int32_t offset,uint32_t origin)
{
 if(descriptor<RESTORE_CRT_LOWIO_CAPACITY){
  uint32_t record_offset=(descriptor&31u)*8u;
  RestoreCrtLowioRecord *volatile *slot=(RestoreCrtLowioRecord *volatile *)(uintptr_t)
   ((uint32_t)(uintptr_t)RESTORE_CRT_LOWIO_BLOCKS+
    (uint32_t)((int32_t)descriptor>>5)*4u);
  volatile RestoreCrtLowioRecord *record=(volatile RestoreCrtLowioRecord *)(uintptr_t)
   ((uint32_t)(uintptr_t)*slot+record_offset);
  if((record->flags&1u)!=0){
   uint32_t handle=FUN_005669c6(descriptor),position;
   if(handle==0xffffffffu){
    /* Unlike descriptor validation, this path leaves DOSerrno alone. */
    RESTORE_CRT_ERRNO=9;
    return 0xffffffffu;
   }
   position=SetFilePointer((HANDLE)(uintptr_t)handle,offset,0,origin);
   if(position==0xffffffffu){
    uint32_t error=GetLastError();
    if(error!=0){FUN_005672c8(error);return 0xffffffffu;}
   }
   /* Same captured table cell and byte offset; record validity/capacity
      are not rechecked after the accessor or either Win32 callback. */
   record=(volatile RestoreCrtLowioRecord *)(uintptr_t)
    ((uint32_t)(uintptr_t)*slot+record_offset);
   record->flags&=0xfdu;
   return position;
  }
 }
 RESTORE_CRT_DOSERRNO=0;
 RESTORE_CRT_ERRNO=9;
 return 0xffffffffu;
}
uint32_t FUN_005662e3(uint32_t descriptor,int32_t offset,uint32_t origin)
{return restore_crt_seek(descriptor,offset,origin);}
