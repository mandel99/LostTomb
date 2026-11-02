#include "restore_types.h"

int FUN_0041b9d0(int param_1)

{
  int iVar1;
  int in_ECX;
  int *piVar2;
  
  iVar1 = 0;
  if (0 < *(int *)(in_ECX + 0x1900)) {
    piVar2 = (int *)(in_ECX + 0xc);
    do {
      if (*piVar2 == param_1) {
        return iVar1;
      }
      iVar1 = iVar1 + 1;
      piVar2 = piVar2 + 0x10;
    } while (iVar1 < *(int *)(in_ECX + 0x1900));
  }
  return -1;
}
