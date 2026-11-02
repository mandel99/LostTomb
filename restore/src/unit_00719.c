#include "restore_types.h"

int * FUN_0044bb40(int param_1)

{
  int *in_ECX;
  
  if (param_1 == 0x8c) {
    return in_ECX + 0x10f5;
  }
  if ((-1 < param_1) && (param_1 < *in_ECX)) {
    return (int *)((uint)(in_ECX + param_1 * 0x1f + 1) &
                  (*(char *)((int)(in_ECX + param_1 * 0x1f + 1) + 2) != '\x02') - 1);
  }
  return (int *)0x0;
}
