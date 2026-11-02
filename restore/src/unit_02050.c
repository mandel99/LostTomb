#include "restore_types.h"

void FUN_004e96c0(int param_1)

{
  unsigned int selector = *(unsigned int *)(uintptr_t)0x00eaac28u;
  volatile int32_t *value = (volatile int32_t *)(uintptr_t)(0x00ea5b48u + selector * 0x49d8u);

  /* Native ADD wraps before JNS and the later signed upper clamp. */
  *value = (int32_t)((uint32_t)*value + (uint32_t)param_1);
  if (*value < 0) *value = 0;
  if (*value > 100) *value = 100;
}
