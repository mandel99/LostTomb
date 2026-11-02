#include "restore_types.h"

void FUN_004e5140(undefined4 *param_1)

{
  undefined4 *in_ECX;
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  
  iVar3 = 4;
  *in_ECX = *param_1;
  in_ECX[1] = param_1[1];
  in_ECX[2] = param_1[2];
  in_ECX[3] = param_1[3];
  in_ECX[4] = param_1[4];
  puVar1 = in_ECX + 5;
  puVar2 = param_1 + 9;
  do {
    *puVar1 = *(undefined4 *)(((int)param_1 - (int)in_ECX) + (int)puVar1);
    puVar1[4] = *puVar2;
    puVar1 = puVar1 + 1;
    iVar3 = iVar3 + -1;
    puVar2 = puVar2 + 1;
  } while (iVar3 != 0);
  return;
}
