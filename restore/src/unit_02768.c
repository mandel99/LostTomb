#include "restore_types.h"
extern void FUN_00561a2b();

void FUN_005610d4(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int in_ECX;
  
  if ((*(int *)(in_ECX + 4) != 0) && (*(int *)(in_ECX + 0x10) != 0)) {
    FUN_00561a2b(*(int *)(in_ECX + 0x10));
  }
  *(undefined4 *)(in_ECX + 0x10) = param_1;
  *(undefined4 *)(in_ECX + 4) = param_3;
  *(undefined4 *)(in_ECX + 0x14) = param_2;
  return;
}
