#include "restore_types.h"
#define DAT_00705af0 (*(undefined4 *)0x00705af0u)
extern longlong FUN_0056133c();

undefined4 FUN_0045a0a0(int param_1,undefined4 param_2,int *param_3)

{
  int *piVar1;
  int *piVar2;
  undefined4 uVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  int local_c [3];
  
  if (param_1 < 1) {
    return 0;
  }
  piVar2 = local_c;
  iVar6 = 3;
  piVar5 = param_3;
  do {
    iVar4 = *piVar5;
    *piVar2 = 0;
    piVar1 = piVar5;
    if (0 < iVar4) {
      do {
        *piVar2 = *piVar2 + (&DAT_00705af0)[piVar1[1]];
        iVar4 = iVar4 + -1;
        piVar1 = piVar1 + 1;
      } while (iVar4 != 0);
    }
    piVar2 = piVar2 + 1;
    piVar5 = piVar5 + 7;
    iVar6 = iVar6 + -1;
  } while (iVar6 != 0);
  if (param_3[0x14] * local_c[2] + param_3[0xd] * local_c[1] + param_3[6] * local_c[0] != 0) {
    uVar3 = FUN_0056133c();
    return uVar3;
  }
  return 0;
}
