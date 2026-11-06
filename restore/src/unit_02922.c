#include "crt_lowio.h"
uint32_t restore_crt_get_os_handle(uint32_t descriptor)
{
 if (descriptor<RESTORE_CRT_LOWIO_CAPACITY) {
  /* Native SAR is retained even for a malformed capacity admitting bit31. */
  int32_t block=(int32_t)descriptor>>5;
  volatile RestoreCrtLowioRecord *record=RESTORE_CRT_LOWIO_BLOCKS[block]+(descriptor&31u);
  if ((record->flags&1u)!=0) return record->handle;
 }
 RESTORE_CRT_DOSERRNO=0;
 RESTORE_CRT_ERRNO=9;
 return UINT32_MAX;
}
uint32_t FUN_005669c6(uint32_t descriptor)
{
 return restore_crt_get_os_handle(descriptor);
}
