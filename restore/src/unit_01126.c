#include "restore_types.h"
#define DAT_005cccc8 (*(undefined4 *)0x005cccc8u)
#define DAT_005dbda8 (*(undefined4 *)0x005dbda8u)
#define DAT_005dbde8 (*(undefined4 *)0x005dbde8u)
#define DAT_005dbe08 (*(undefined4 *)0x005dbe08u)
#define DAT_005dbe28 (*(undefined4 *)0x005dbe28u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc96dc (*(undefined *)0x00bc96dcu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00d05720 (*(undefined4 *)0x00d05720u)
#define _DAT_00bc96d8 (*(undefined4 *)0x00bc96d8u)

int FUN_0047cc70(int param_1,int param_2,int param_3,char param_4)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  int *piVar6;
  int *piVar7;
  int iVar8;
  
  if ((param_3 == 1) && (DAT_00bca2e0 != 0)) {
    if (DAT_00bca2e0 == 6) {
      param_2 = param_2 + -1;
    }
    else if (DAT_00bca2e0 == 4) {
      param_1 = param_1 + -1;
      param_2 = param_2 + -1;
    }
    else if (DAT_00bca2e0 == 2) {
      param_1 = param_1 + -1;
    }
  }
  if ((((-1 < param_1) && (-1 < param_2)) && (param_1 + 1 < DAT_00bca2d0)) &&
     (param_2 + 1 < DAT_00bca2d4)) {
    param_3 = 0;
    DAT_00bc96dc = 0;
    param_1 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
    do {
      iVar2 = 0;
      param_2 = 0;
      piVar7 = &DAT_005cccc8;
      piVar6 = &DAT_005dbda8 + DAT_00bc96dc * 4;
      do {
        iVar3 = *piVar7 + param_1;
        uVar4 = (&DAT_00af598c)[iVar3];
        if (*piVar6 == 0) {
          if (((uVar4 & 0x10004) != 0) &&
             (((param_4 == '\0' || (*(int *)(&DAT_00d05720 + iVar3 * 4) == 0)) ||
              (*(char *)(*(int *)(&DAT_00d05720 + iVar3 * 4) + 0x18) != '\0')))) break;
          if (((uVar4 & 0x40) == 0) && ((uVar4 & 0x26341237) == 0)) {
            uVar4 = uVar4 & 8;
            goto joined_r0x0047cdcb;
          }
LAB_0047cdcd: ;
          iVar2 = iVar2 + 1;
        }
        else {
          bVar1 = false;
          if ((((*(int *)(&DAT_00d05720 + iVar3 * 4) != 0) &&
               (*(char *)(*(int *)(&DAT_00d05720 + iVar3 * 4) + 0x18) == '\0')) && (param_3 != 2))
             && (param_3 = 1, param_4 != '\0')) {
            bVar1 = true;
          }
          if (((uVar4 & 4) == 0) || (param_3 == 1)) {
            if (!bVar1) break;
          }
          else {
            param_3 = 2;
          }
          if ((uVar4 & 0x40) != 0) {
            iVar2 = iVar2 + 1;
          }
          uVar4 = uVar4 & 1;
joined_r0x0047cdcb: ;
          if (uVar4 != 0) goto LAB_0047cdcd;
        }
        param_2 = param_2 + 1;
        piVar7 = piVar7 + 1;
        piVar6 = piVar6 + 1;
      } while ((int)piVar7 < 0x5cccd8);
      iVar3 = DAT_00bc96dc * 8;
      uVar4 = -(uint)(param_4 != '\0') & 0x10000 | 4;
      iVar8 = 2;
      do {
        iVar5 = *(int *)((int)&DAT_005dbde8 + iVar3) + param_1;
        if ((((&DAT_00af598c)[iVar5] & uVar4) == 0) &&
           (((param_4 == '\0' || (*(int *)(&DAT_00d05720 + iVar5 * 4) == 0)) ||
            (*(char *)(*(int *)(&DAT_00d05720 + iVar5 * 4) + 0x18) != '\0')))) {
          param_2 = 0;
        }
        iVar5 = *(int *)((int)&DAT_005dbe08 + iVar3) + param_1;
        if ((((&DAT_00af598c)[iVar5] & uVar4) == 0) &&
           (((param_4 == '\0' || (*(int *)(&DAT_00d05720 + iVar5 * 4) == 0)) ||
            (*(char *)(*(int *)(&DAT_00d05720 + iVar5 * 4) + 0x18) != '\0')))) {
          param_2 = 0;
        }
        iVar5 = *(int *)((int)&DAT_005dbe28 + iVar3) + param_1;
        if ((((&DAT_00af598c)[iVar5] & uVar4) == 0) &&
           (((param_4 == '\0' || (*(int *)(&DAT_00d05720 + iVar5 * 4) == 0)) ||
            (*(char *)(*(int *)(&DAT_00d05720 + iVar5 * 4) + 0x18) != '\0')))) {
          param_2 = 0;
        }
        iVar3 = iVar3 + 4;
        iVar8 = iVar8 + -1;
      } while (iVar8 != 0);
      if (param_2 == 4) {
        _DAT_00bc96d8 = DAT_00bc96dc;
        DAT_00bc96dc = DAT_00bc96dc - DAT_00bca2e0 / 2;
        if (DAT_00bc96dc < 0) {
          DAT_00bc96dc = DAT_00bc96dc + 4;
        }
        return iVar2;
      }
      DAT_00bc96dc = DAT_00bc96dc + 1;
    } while (DAT_00bc96dc < 4);
  }
  return 999;
}
