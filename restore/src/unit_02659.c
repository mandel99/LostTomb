#include "restore_types.h"

void FUN_0055d250(undefined4 *param_1,undefined4 *param_2,uint param_3)

{
  undefined1 uVar1;
  undefined4 uVar2;
  uint uVar3;
  
  for (uVar3 = param_3 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
    uVar2 = *param_2;
    param_2 = param_2 + 1;
    *param_1 = uVar2;
    param_1 = param_1 + 1;
  }
  for (param_3 = param_3 & 3; param_3 != 0; param_3 = param_3 - 1) {
    uVar1 = *(undefined1 *)param_2;
    param_2 = (undefined4 *)((int)param_2 + 1);
    *(undefined1 *)param_1 = uVar1;
    param_1 = (undefined4 *)((int)param_1 + 1);
  }
  return;
}
