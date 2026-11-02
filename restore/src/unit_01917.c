#include "restore_types.h"
#include <stdint.h>
int FUN_004e1750(void)

{
  const int16_t *entry=(const int16_t *)(uintptr_t)0x00e95232u;
  int index;

  for (index=0;index<1000;index++,entry+=24) {
    if (*entry==0) return index;
  }
  return -1;
}
