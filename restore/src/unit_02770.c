#include "restore_types.h"

int FUN_00561178(char param_1)

{
  char *pcVar1;
  int iVar2;
  int *in_ECX;
  
  if ((uint)in_ECX[9] < (uint)in_ECX[10]) {
    pcVar1 = (char *)(in_ECX[10] - 1);
    in_ECX[10] = (int)pcVar1;
    *pcVar1 = param_1;
    iVar2 = (int)param_1;
  }
  else {
    iVar2 = (**(code **)(*in_ECX + 0x24))((int)param_1);
  }
  return iVar2;
}
