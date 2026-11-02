#include "restore_types.h"
#define DAT_005cccec (*(undefined4 *)0x005cccecu)
#define DAT_005dbe48 (*(undefined4 *)0x005dbe48u)
#define DAT_005dbed8 (*(undefined4 *)0x005dbed8u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc96dc (*(undefined *)0x00bc96dcu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00d05720 (*(undefined4 *)0x00d05720u)
#define _DAT_00bc96d8 (*(undefined4 *)0x00bc96d8u)

int FUN_0047d060(int param_1,int param_2,int param_3)

{
  uint uVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int *piVar5;
  
  if ((param_3 == 1) && (DAT_00bca2e0 != 0)) {
    if (DAT_00bca2e0 == 6) {
      param_2 = param_2 + -2;
    }
    else if (DAT_00bca2e0 == 4) {
      param_1 = param_1 + -2;
      param_2 = param_2 + -2;
    }
    else if (DAT_00bca2e0 == 2) {
      param_1 = param_1 + -2;
    }
  }
  if ((((-1 < param_1) && (-1 < param_2)) && (param_1 + 2 < DAT_00bca2d0)) &&
     (param_2 + 2 < DAT_00bca2d4)) {
    param_1 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
    DAT_00bc96dc = 0;
    do {
      iVar2 = 0;
      param_2 = 0;
      piVar5 = &DAT_005cccec;
      piVar3 = &DAT_005dbe48 + DAT_00bc96dc * 9;
      do {
        if (*piVar3 == 0) {
          iVar4 = *piVar5 + param_1;
          uVar1 = (&DAT_00af598c)[iVar4];
          if (((uVar1 & 0x10004) != 0) ||
             ((*(int *)(&DAT_00d05720 + iVar4 * 4) != 0 &&
              (*(char *)(*(int *)(&DAT_00d05720 + iVar4 * 4) + 0x18) == '\0')))) break;
          if (((uVar1 & 0x40) == 0) && ((uVar1 & 0x26341237) == 0)) {
            uVar1 = uVar1 & 8;
            goto joined_r0x0047d16d;
          }
LAB_0047d16f: ;
          param_2 = param_2 + 1;
        }
        else {
          uVar1 = (&DAT_00af598c)[*piVar5 + param_1];
          if ((uVar1 & 4) == 0) break;
          if ((uVar1 & 0x40) != 0) {
            param_2 = param_2 + 1;
          }
          uVar1 = uVar1 & 1;
joined_r0x0047d16d: ;
          if (uVar1 != 0) goto LAB_0047d16f;
        }
        iVar2 = iVar2 + 1;
        piVar5 = piVar5 + 1;
        piVar3 = piVar3 + 1;
      } while ((int)piVar5 < 0x5ccd10);
      piVar3 = &DAT_005dbed8 + DAT_00bc96dc * 2;
      iVar4 = 2;
      do {
        if ((*(byte *)(&DAT_00af598c + *piVar3 + param_1) & 4) == 0) {
          iVar2 = 0;
        }
        piVar3 = piVar3 + 1;
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
      if (iVar2 == 9) {
        _DAT_00bc96d8 = DAT_00bc96dc;
        DAT_00bc96dc = DAT_00bc96dc - DAT_00bca2e0 / 2;
        if (DAT_00bc96dc < 0) {
          DAT_00bc96dc = DAT_00bc96dc + 4;
        }
        return param_2;
      }
      DAT_00bc96dc = DAT_00bc96dc + 1;
    } while (DAT_00bc96dc < 4);
  }
  return 999;
}
