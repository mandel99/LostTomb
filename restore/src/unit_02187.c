#include "restore_types.h"

undefined4 FUN_004f6570(int param_1)

{
  int iVar1;
  int in_ECX;
  
  iVar1 = *(int *)(in_ECX + 4);
  while( true ) {
    if (iVar1 == *(int *)(in_ECX + 8)) {
      return 0;
    }
    if (*(int *)(in_ECX + 0x14 + iVar1 * 4) == param_1) break;
    iVar1 = iVar1 + 1;
    if (*(int *)(in_ECX + 0xc) <= iVar1) {
      iVar1 = 0;
    }
  }
  return 1;
}
