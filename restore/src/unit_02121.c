#include "restore_types.h"
#define DAT_005f5ca8 (*(undefined4 *)0x005f5ca8u)
#define DAT_005f5cb4 (*(undefined4 *)0x005f5cb4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)

int FUN_004f2190(int param_1)

{
  int iVar1;
  uint uVar2;
  int *piVar3;
  int iVar4;
  
  piVar3 = &DAT_005f5cb4;
  uVar2 = (uint)(int)*(short *)(0x007999eau + (uint)param_1 * 0x108u);
  iVar4 = 0;
  while (((int)uVar2 < piVar3[-3] || (*piVar3 < (int)uVar2))) {
    piVar3 = piVar3 + 4;
    iVar4 = iVar4 + 1;
    if (0x5f5d13 < (int)piVar3) {
      return param_1;
    }
  }
  if (iVar4 < 6) {
    if (((int)(&DAT_005f5ca8)[iVar4 * 4] <= (int)uVar2) &&
       (iVar1 = *(int *)(iVar4 * 0x10 + 0x5f5cac), (int)uVar2 <= iVar1)) {
      if (uVar2 == (&DAT_005f5ca8)[iVar4 * 4]) {
        return iVar1 + 1 + (param_1 - uVar2);
      }
      return param_1 + -1;
    }
    iVar1 = *(int *)(iVar4 * 0x10 + 0x5f5cb0);
    if ((iVar1 <= (int)uVar2) && ((int)uVar2 <= (int)(&DAT_005f5cb4)[iVar4 * 4])) {
      if (uVar2 == (&DAT_005f5cb4)[iVar4 * 4]) {
        return iVar1 + -1 + (param_1 - uVar2);
      }
      return param_1 + 1;
    }
    if ((uVar2 & 1) != 0) {
      return param_1 + -2;
    }
    param_1 = param_1 + 2;
  }
  return param_1;
}
