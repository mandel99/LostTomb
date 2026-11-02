#include "restore_types.h"
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

int FUN_004f4aa0(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = -1;
  iVar2 = -1;
  do {
    iVar2 = iVar2 + 1;
    if (2 < iVar2) {
      return iVar1;
    }
    if (*(int *)(0x00ea5facu + (uint)DAT_00eaac28 * 0x49d8u +
                 (uint)iVar2 * 4u) == param_1) {
      iVar1 = iVar2;
    }
  } while (iVar1 < 0);
  return iVar1;
}
