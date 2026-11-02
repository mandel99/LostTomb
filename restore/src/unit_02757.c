#include "restore_types.h"

void FUN_00560ccf(void)

{
  uint *puVar1;
  int iVar2;
  int *in_ECX;
  
  iVar2 = (**(code **)(**(int **)(*(int *)(*in_ECX + 4) + 4 + (int)in_ECX) + 4))();
  if (iVar2 == -1) {
    puVar1 = (uint *)(*(int *)(*in_ECX + 4) + 8 + (int)in_ECX);
    *puVar1 = *puVar1 | 2;
  }
  return;
}
