#include "restore_types.h"
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f5d58 (*(undefined4 *)0x005f5d58u)
#define DAT_005f5f38 (*(undefined4 *)0x005f5f38u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)

int FUN_004f2720(int param_1)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  
  if (DAT_005e4ef4 == 5) {
    piVar3 = (int *)(&DAT_005f5d58 + (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84) * 0xa0);
  }
  else {
    piVar3 = (int *)(&DAT_005f5f38 + (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84) * 0x270);
  }
  iVar5 = (int)(short)(&DAT_007999ea)[param_1 * 0x84];
  iVar4 = 0;
  iVar1 = *piVar3;
  piVar2 = piVar3;
  while( true ) {
    if (iVar1 < 0) {
      return -1;
    }
    if (piVar2[1] == iVar5) break;
    if (*piVar2 == iVar5) {
      return piVar3[iVar4 * 2 + 1] + (param_1 - iVar5);
    }
    iVar1 = piVar2[2];
    piVar2 = piVar2 + 2;
    iVar4 = iVar4 + 1;
  }
  return piVar3[iVar4 * 2] + (param_1 - iVar5);
}
