#include "restore_types.h"

undefined4 FUN_004f6a10(int *param_1)

{
  int in_ECX;
  int iVar1;
  int iVar2;
  
  if (*(int *)(in_ECX + 0x10) != 0) {
    iVar1 = *param_1;
    while (iVar1 < 0) {
      iVar1 = *param_1 + *(int *)(in_ECX + 0x10);
      *param_1 = iVar1;
    }
    iVar1 = *(int *)(in_ECX + 0x10);
    if (iVar1 <= *param_1) {
      do {
        iVar2 = *param_1 - iVar1;
        *param_1 = iVar2;
        iVar1 = *(int *)(in_ECX + 0x10);
      } while (iVar1 <= iVar2);
    }
    return 1;
  }
  return 0;
}
