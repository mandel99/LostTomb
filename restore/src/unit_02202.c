#include "restore_types.h"
extern undefined4 FUN_004f6a10();

undefined4 FUN_004f68b0(void)

{
  int *piVar1;
  int iVar2;
  int in_ECX;
  
  if (*(int *)(in_ECX + 0xc) == 0) {
    return 0;
  }
  piVar1 = (int *)(in_ECX + 4);
  iVar2 = *piVar1;
  *piVar1 = iVar2 + 1;
  FUN_004f6a10(piVar1);
  *(int *)(in_ECX + 0xc) = *(int *)(in_ECX + 0xc) + -1;
  return *(undefined4 *)(*(int *)(in_ECX + 0x14) + iVar2 * 4);
}
