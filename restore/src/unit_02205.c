#include "restore_types.h"
extern undefined4 FUN_004f6a10();

undefined4 FUN_004f6950(int param_1)

{
  int iVar1;
  int in_ECX;
  int iVar2;
  int local_4;
  
  iVar1 = param_1;
  iVar2 = 0;
  local_4 = *(int *)(in_ECX + 4);
  if (0 < *(int *)(in_ECX + 0xc)) {
    do {
      if (*(int *)(*(int *)(in_ECX + 0x14) + local_4 * 4) == iVar1) break;
      local_4 = local_4 + 1;
      FUN_004f6a10(&local_4);
      iVar2 = iVar2 + 1;
    } while (iVar2 < *(int *)(in_ECX + 0xc));
  }
  if (iVar2 != *(int *)(in_ECX + 0xc)) {
    if (iVar2 < *(int *)(in_ECX + 0xc) + -1) {
      do {
        param_1 = local_4 + 1;
        FUN_004f6a10(&param_1);
        *(undefined4 *)(*(int *)(in_ECX + 0x14) + local_4 * 4) =
             *(undefined4 *)(*(int *)(in_ECX + 0x14) + param_1 * 4);
        local_4 = local_4 + 1;
        FUN_004f6a10(&local_4);
        iVar2 = iVar2 + 1;
      } while (iVar2 < *(int *)(in_ECX + 0xc) + -1);
    }
    *(int *)(in_ECX + 8) = *(int *)(in_ECX + 8) + -1;
    *(int *)(in_ECX + 0xc) = *(int *)(in_ECX + 0xc) + -1;
    FUN_004f6a10((int *)(in_ECX + 8));
    return 1;
  }
  return 0;
}
