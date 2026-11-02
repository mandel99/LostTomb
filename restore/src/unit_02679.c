#include "restore_types.h"

undefined4 FUN_0055d990(int param_1)

{
  int iVar1;
  int in_ECX;
  int *piVar2;
  
  if (-1 < param_1) {
    iVar1 = *(int *)(in_ECX + 0x11c);
    piVar2 = (int *)(**(int **)(in_ECX + 0x120) + -4);
    **(int **)(in_ECX + 0x120) = (int)piVar2;
    *piVar2 = iVar1 + param_1 * 0x18;
  }
  return 0;
}
