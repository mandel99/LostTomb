#include "restore_types.h"

undefined4 FUN_004f65f0(int *param_1)

{
  int iVar1;
  undefined4 uVar2;
  int in_ECX;
  
  iVar1 = *param_1;
  uVar2 = 0;
  if (iVar1 != *(int *)(in_ECX + 8)) {
    uVar2 = *(undefined4 *)(in_ECX + 0x14 + iVar1 * 4);
    *param_1 = iVar1 + 1;
    if (*(int *)(in_ECX + 0xc) <= iVar1 + 1) {
      *param_1 = 0;
    }
  }
  return uVar2;
}
