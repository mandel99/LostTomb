#include "restore_types.h"

void FUN_004cbee0(undefined1 *param_1,undefined1 *param_2)

{
  int iVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  
  if (param_1 < param_2) {
    puVar3 = param_1;
    puVar2 = param_1;
    for (iVar1 = (int)param_2 - (int)param_1; puVar2 = puVar2 + 1, iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar3 = *puVar2;
      puVar3 = puVar3 + 1;
    }
    param_1[(int)param_2 - (int)param_1] = 0;
    return;
  }
  *param_1 = 0;
  return;
}
