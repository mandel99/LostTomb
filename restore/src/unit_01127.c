#include "restore_types.h"
#define DAT_005cc8c4 (*(undefined4 *)0x005cc8c4u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00e94e04 (*(undefined *)0x00e94e04u)
#define _DAT_00bc9734 (*(undefined4 *)0x00bc9734u)

void FUN_0047cf00(undefined2 param_1,int param_2,byte *param_3,undefined4 param_4)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  byte *pbVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  iVar7 = (int)param_3;
  iVar8 = param_2;
  if (DAT_00e94e04 == 0) {
    iVar6 = 0;
  }
  else {
    if (DAT_00e94e04 == 2) {
      iVar6 = 0;
      iVar5 = 0;
      goto LAB_0047cf13;
    }
    iVar6 = 1;
    if (DAT_00e94e04 == 4) {
      iVar5 = 0;
      goto LAB_0047cf13;
    }
  }
  iVar5 = 1;
LAB_0047cf13: ;
  if (param_2 < 0) {
    _DAT_00bc9734 = 1;
    return;
  }
  if ((int)param_3 < 0) {
    _DAT_00bc9734 = 1;
    return;
  }
  if (DAT_00bca2d0 <= param_2 + 1) {
    _DAT_00bc9734 = 1;
    return;
  }
  if (DAT_00bca2d4 <= (int)param_3 + 1) {
    _DAT_00bc9734 = 1;
    return;
  }
  param_2 = 0;
  param_3 = &DAT_005cc8c4;
  iVar8 = DAT_00bca2d8 + iVar7 * 0xe4 + iVar8;
  DAT_00bc9768 = iVar8;
  do {
    iVar7 = 0;
    pbVar4 = param_3;
    do {
      iVar3 = *(int *)(pbVar4 + -4) + iVar8;
      uVar2 = (&DAT_00af598c)[iVar3];
      (&DAT_00af598c)[iVar3] = uVar2 | 8;
      if ((uVar2 & 4) == 0) {
        (&DAT_00af598c)[iVar3] = uVar2 & 0x3052e80 | 8;
      }
      *(undefined2 *)(&DAT_00b4e6fc + iVar3 * 2) = param_1;
      (&DAT_00b285cc)[iVar3] = (&DAT_00b285cc)[iVar3] & 0xe1 | 1;
      (&DAT_00b7482c)[iVar3] = param_4;
      bVar1 = *pbVar4;
      (&DAT_00b67d1c)[iVar3] = bVar1;
      if ((iVar7 == iVar6) && (param_2 == iVar5)) {
        (&DAT_00b67d1c)[iVar3] = bVar1 | 0x40;
      }
      iVar7 = iVar7 + 1;
      pbVar4 = pbVar4 + 8;
    } while (iVar7 < 2);
    param_3 = param_3 + 0x30;
    param_2 = param_2 + 1;
  } while ((int)param_3 < 0x5cc924);
  return;
}
