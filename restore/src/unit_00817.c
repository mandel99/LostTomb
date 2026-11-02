#include "restore_types.h"

uint FUN_00455ca0(int param_1,undefined4 param_2,int *param_3)

{
  uint uVar1;
  
  uVar1 = 0;
  do {
    if (*param_3 == param_1) {
      return CONCAT31((int3)(uVar1 >> 8),1);
    }
    uVar1 = uVar1 + 1;
    param_3 = param_3 + 1;
  } while ((int)uVar1 < 4);
  return uVar1 & 0xffffff00;
}
