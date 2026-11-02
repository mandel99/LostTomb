#include "restore_types.h"
extern void FUN_004f6760();
extern void * FUN_00561a1d();

undefined4 FUN_004f6700(uint param_1)

{
  undefined4 *puVar1;
  int in_ECX;
  int iVar2;
  
  if ((int)param_1 <= *(int *)(in_ECX + 0x10)) {
    return 1;
  }
  FUN_004f6760();
  *(uint *)(in_ECX + 0x10) = param_1;
  puVar1 = FUN_00561a1d(param_1 * 4);
  param_1 = param_1 & 0x3fffffff;
  *(undefined4 **)(in_ECX + 0x14) = puVar1;
  for (; param_1 != 0; param_1 = param_1 - 1) {
    *puVar1 = 0;
    puVar1 = puVar1 + 1;
  }
  for (iVar2 = 0; iVar2 != 0; iVar2 = iVar2 + -1) {
    *(undefined1 *)puVar1 = 0;
    puVar1 = (undefined4 *)((int)puVar1 + 1);
  }
  *(undefined4 *)(in_ECX + 4) = 1;
  *(undefined4 *)(in_ECX + 8) = 0;
  *(undefined4 *)(in_ECX + 0xc) = 0;
  return 1;
}
