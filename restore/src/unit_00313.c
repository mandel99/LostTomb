#include "restore_types.h"
#define DAT_00596150 (*(undefined4 *)0x00596150u)
#define DAT_00596f30 (*(undefined4 *)0x00596f30u)
#define DAT_00597d10 (*(undefined4 *)0x00597d10u)
#define DAT_00704dc0 (*(int32_t *)0x00704dc0u)

void FUN_0041f8c0(int param_1)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  
  if ((param_1 != 0) && (param_1 != 4)) {
    DAT_00704dc0 = 2;
    puVar2 = &DAT_00597d10;
    puVar3 = &DAT_00596150;
    for (iVar1 = 0x378; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar3 = *puVar2;
      puVar2 = puVar2 + 1;
      puVar3 = puVar3 + 1;
    }
    return;
  }
  DAT_00704dc0 = 0;
  puVar2 = &DAT_00596f30;
  puVar3 = &DAT_00596150;
  for (iVar1 = 0x378; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar3 = *puVar2;
    puVar2 = puVar2 + 1;
    puVar3 = puVar3 + 1;
  }
  return;
}
