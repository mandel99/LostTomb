#include "restore_types.h"
extern undefined4 FUN_004f6a10();
extern undefined4 FUN_004f6a50();

void FUN_004f6900(undefined4 param_1)

{
  int *piVar1;
  int iVar2;
  int in_ECX;
  
  if ((*(int *)(in_ECX + 0xc) == *(int *)(in_ECX + 0x10)) &&
     (iVar2 = FUN_004f6a50(*(int *)(in_ECX + 0x10) + 10), iVar2 == 0)) {
    return;
  }
  if (*(int *)(in_ECX + 0x14) != 0) {
    piVar1 = (int *)(in_ECX + 8);
    *piVar1 = *(int *)(in_ECX + 8) + 1;
    FUN_004f6a10(piVar1);
    *(undefined4 *)(*(int *)(in_ECX + 0x14) + *piVar1 * 4) = param_1;
    *(int *)(in_ECX + 0xc) = *(int *)(in_ECX + 0xc) + 1;
  }
  return;
}
