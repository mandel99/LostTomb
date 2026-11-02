#include "restore_types.h"
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f5d58 (*(undefined4 *)0x005f5d58u)
#define DAT_005f5f38 (*(undefined4 *)0x005f5f38u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00eaac44 (*(undefined *)0x00eaac44u)
#define DAT_00eaaf58 (*(undefined *)0x00eaaf58u)

int FUN_004f27b0(int param_1,undefined1 *param_2)

{
  int *piVar1;
  byte bVar2;
  int iVar3;
  int *piVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  
  bVar2 = *(byte *)(&DAT_00799a2c + param_1 * 0x84);
  *param_2 = 0;
  if (DAT_005e4ef4 == 5) {
    piVar4 = (int *)(&DAT_005f5d58 + (uint)bVar2 * 0xa0);
  }
  else {
    piVar4 = (int *)(&DAT_005f5f38 + (uint)bVar2 * 0x270);
  }
  iVar6 = 0;
  piVar5 = piVar4;
  if (-1 < *piVar4) {
    while( true ) {
      if (piVar5[1] == (int)(short)(&DAT_007999ea)[param_1 * 0x84]) {
        iVar7 = 0;
        if (DAT_00eaaf58 < 1) {
          return -1;
        }
        piVar5 = &DAT_00eaac44;
        do {
          iVar3 = *piVar5;
          if ((int)(short)(&DAT_007999ea)[iVar3 * 0x84] == piVar4[iVar6 * 2]) {
            *param_2 = 1;
            return iVar3;
          }
          iVar7 = iVar7 + 1;
          piVar5 = piVar5 + 1;
        } while (iVar7 < DAT_00eaaf58);
        return -1;
      }
      if (*piVar5 == (int)(short)(&DAT_007999ea)[param_1 * 0x84]) break;
      piVar1 = piVar5 + 2;
      iVar6 = iVar6 + 1;
      piVar5 = piVar5 + 2;
      if (*piVar1 < 0) {
        return -1;
      }
    }
    iVar7 = 0;
    if (0 < DAT_00eaaf58) {
      piVar5 = &DAT_00eaac44;
      do {
        if ((int)(short)(&DAT_007999ea)[*piVar5 * 0x84] == piVar4[iVar6 * 2 + 1]) {
          return *piVar5;
        }
        iVar7 = iVar7 + 1;
        piVar5 = piVar5 + 1;
      } while (iVar7 < DAT_00eaaf58);
    }
  }
  return -1;
}
