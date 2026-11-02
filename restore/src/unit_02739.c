#include "restore_types.h"

void FUN_00560671(int param_1)

{
  int in_ECX;
  
  if ((*(int *)(in_ECX + 0x1c) != 0) && (*(undefined4 **)(in_ECX + 4) != (undefined4 *)0x0)) {
    (**(code **)**(undefined4 **)(in_ECX + 4))(1);
  }
  *(int *)(in_ECX + 4) = param_1;
  if (param_1 == 0) {
    *(uint *)(in_ECX + 8) = *(uint *)(in_ECX + 8) | 4;
  }
  else {
    *(uint *)(in_ECX + 8) = *(uint *)(in_ECX + 8) & 0xfffffffb;
  }
  return;
}
