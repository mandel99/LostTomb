#include "restore_types.h"

undefined4 FUN_004f65b0(undefined4 *param_1)

{
  int iVar1;
  int in_ECX;
  
  iVar1 = *(int *)(in_ECX + 8) + 1;
  if (*(int *)(in_ECX + 0xc) <= iVar1) {
    iVar1 = 0;
  }
  if (iVar1 == *(int *)(in_ECX + 4)) {
    return 0;
  }
  *(undefined4 *)(in_ECX + 0x14 + *(int *)(in_ECX + 8) * 4) = *param_1;
  *(int *)(in_ECX + 8) = iVar1;
  return 1;
}
