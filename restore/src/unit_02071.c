#include "restore_types.h"
#define DAT_00ac2d4c (*(undefined4 *)0x00ac2d4cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
extern uint FUN_004ec410();
extern bool FUN_005181c0();
extern undefined1 FUN_0051cd80();

undefined4
FUN_004eabf0(int param_1,int *param_2,undefined4 param_3,undefined4 param_4,int *param_5,
            int *param_6)

{
  int iVar1;
  int iVar2;
  int iVar3;
  char cVar4;
  uint uVar5;
  int iVar6;
  int *piVar7;
  uint uVar8;
  int *piVar9;
  int *piVar10;
  int *piVar11;
  int *piVar12;
  uint uVar13;
  int local_60 [24];
  
  iVar3 = (int)param_2;
  local_60[0x16] = 0xffffffff;
  local_60[9] = 0xffffffff;
  local_60[10] = 0xffffffff;
  local_60[0xb] = 0xffffffff;
  local_60[0] = -1;
  local_60[1] = 0xffffffff;
  local_60[7] = 0xffffffff;
  local_60[0x12] = 1;
  local_60[0xd] = 1;
  local_60[0xe] = 1;
  local_60[0xf] = 1;
  local_60[3] = 1;
  local_60[4] = 1;
  local_60[5] = 1;
  local_60[0x10] = 0xffffff1c;
  local_60[0x11] = 0xffffff1d;
  local_60[0x13] = 0xe5;
  local_60[0x14] = 0xe4;
  local_60[0x15] = 0xe3;
  local_60[0x17] = 0xffffff1b;
  local_60[8] = 0;
  local_60[0xc] = 0;
  local_60[2] = 0;
  local_60[6] = 0;
  uVar5 = FUN_0051cd80(param_3,param_4,param_1,param_2);
  iVar2 = DAT_00bca2d8;
  uVar5 = uVar5 & 0xff;
  uVar8 = uVar5 - 1;
  iVar1 = (int)param_2 * 0xe4;
  if ((int)uVar8 < 0) {
    uVar8 = uVar5 + 7;
  }
  if (uVar5 != uVar8) {
    piVar9 = local_60 + uVar5;
    piVar11 = local_60 + uVar5 + 8;
    param_2 = local_60 + uVar5 + 0x10;
    do {
      iVar6 = iVar2 + *param_2 + param_1 + iVar1;
      if ((((&DAT_00ac2d4c)[iVar6] == '\0') && (((&DAT_00af598c)[iVar6] & 0x80000) == 0)) &&
         (cVar4 = FUN_004ec410((&DAT_00af598c)[iVar6],1), cVar4 != '\0')) {
        iVar6 = *piVar9;
        *param_5 = *piVar11 + param_1;
        iVar6 = iVar6 + iVar3;
        *param_6 = iVar6;
        iVar6 = FUN_005181c0(param_1,iVar3,*param_5,iVar6);
        if (iVar6 != 0) {
          return 1;
        }
      }
      uVar13 = uVar5 + 1;
      piVar7 = param_2 + 1;
      piVar12 = piVar11 + 1;
      piVar10 = piVar9 + 1;
      if (7 < (int)uVar13) {
        piVar7 = param_2 + -7;
        uVar13 = uVar5 - 7;
        piVar12 = piVar11 + -7;
        piVar10 = piVar9 + -7;
      }
      piVar9 = piVar10;
      piVar11 = piVar12;
      uVar5 = uVar13;
      param_2 = piVar7;
    } while (uVar13 != uVar8);
  }
  return 0;
}
