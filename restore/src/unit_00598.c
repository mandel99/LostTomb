#include "restore_types.h"
#define DAT_00775614 (*(undefined4 *)0x00775614u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)

void FUN_004413b0(int param_1,int param_2,int param_3)

{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 *puVar4;
  uint *puVar5;
  uint *puVar6;
  int iVar7;
  
  if (param_3 == 0x1f) {
    iVar7 = 2;
  }
  else if (param_3 == 0x1e) {
    iVar7 = 3;
  }
  else if (param_3 == 0x21) {
    iVar7 = 4;
  }
  else {
    iVar7 = (-(uint)(param_3 != 0xd1) & 0xfffffffc) + 5;
  }
  puVar4 = &DAT_00775614;
  do {
    iVar3 = 5;
    puVar2 = puVar4;
    do {
      *puVar2 = 0;
      puVar2 = puVar2 + 5;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    puVar4 = puVar4 + 1;
  } while ((int)puVar4 < 0x775628);
  if (param_2 < iVar7 + param_2) {
    param_3 = (iVar7 + param_2) - param_2;
    puVar5 = &DAT_00775614;
    iVar3 = DAT_00bca2d8 + param_2 * 0xe4;
    iVar1 = param_1;
    puVar6 = puVar5;
    do {
      for (; iVar1 < iVar7 + param_1; iVar1 = iVar1 + 1) {
        *puVar5 = (uint)((*(byte *)(&DAT_00af598c + iVar3 + iVar1) & 0x40) != 0);
        puVar5 = puVar5 + 5;
      }
      iVar3 = iVar3 + 0xe4;
      puVar5 = puVar6 + 1;
      param_3 = param_3 + -1;
      iVar1 = param_1;
      puVar6 = puVar5;
    } while (param_3 != 0);
  }
  return;
}
