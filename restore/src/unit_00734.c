#include "restore_types.h"

void FUN_0044c780(short param_1,short param_2,short param_3)

{
  short sVar1;
  int in_ECX;
  
  if (param_1 < 0) {
    if (param_2 < 0) {
      if (-1 < param_3) {
        param_1 = param_3;
        param_3 = -1;
      }
      goto LAB_0044c7b2;
    }
    param_1 = param_2;
    sVar1 = param_3;
    param_2 = -1;
  }
  else {
LAB_0044c7b2: ;
    sVar1 = param_3;
    if (-1 < param_2) goto LAB_0044c7c1;
  }
  param_3 = sVar1;
  if (-1 < sVar1) {
    param_3 = -1;
    param_2 = sVar1;
  }
LAB_0044c7c1: ;
  *(short *)(in_ECX + 2) = param_1;
  *(short *)(in_ECX + 4) = param_2;
  *(short *)(in_ECX + 6) = param_3;
  return;
}
