#include "restore_types.h"

void FUN_004bd5e0(int param_1)

{
  int in_ECX;
  
  param_1 = *(int *)(in_ECX + 0x1c) + param_1;
  *(int *)(in_ECX + 0x1c) = param_1;
  if (param_1 < 0) {
    param_1 = 0;
  }
  else if (100 < param_1) {
    *(undefined4 *)(in_ECX + 0x1c) = 100;
    return;
  }
  *(int *)(in_ECX + 0x1c) = param_1;
  return;
}
