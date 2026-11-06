#include "crt_lowio.h"

/* Preserve the current/end/restore sequence and the actual END result. */
int32_t restore_crt_file_length(uint32_t descriptor)
{
 if(descriptor<RESTORE_CRT_LOWIO_CAPACITY){
  volatile RestoreCrtLowioRecord *record=(volatile RestoreCrtLowioRecord *)(uintptr_t)
   ((uint32_t)(uintptr_t)RESTORE_CRT_LOWIO_BLOCKS[(int32_t)descriptor>>5]+
    (descriptor&31u)*8u);
  if((record->flags&1u)!=0){
   uint32_t current=FUN_005662e3(descriptor,0,1),end;
   if(current==0xffffffffu)return -1;
   end=FUN_005662e3(descriptor,0,2);
   /* A failed END still attempts restoration when it differs from current.
      Restoration failure never substitutes its result for the saved END. */
   if(current!=end)(void)FUN_005662e3(descriptor,(int32_t)current,0);
   return (int32_t)end;
  }
 }
 RESTORE_CRT_DOSERRNO=0;
 RESTORE_CRT_ERRNO=9;
 return -1;
}
int32_t FUN_0056b1b6(uint32_t descriptor)
{return restore_crt_file_length(descriptor);}
