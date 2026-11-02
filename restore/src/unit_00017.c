#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a39 (*(undefined4 *)0x00799a39u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00f57584 (*(undefined *)0x00f57584u)
#define DAT_00f5758c (*(undefined *)0x00f5758cu)
extern int FUN_0053e590();

void FUN_00401f70(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined *local_8c;
  int *local_88;
  int *local_84;
  int local_7c [31];
  
  local_7c[0xb] = 0x129;
  local_7c[0x11] = 0x129;
  local_7c[9] = 0x9e;
  local_7c[0xf] = 0x9e;
  local_84 = local_7c + 3;
  iVar6 = 0;
  uVar3 = DAT_00bca2e0 / 2;
  local_7c[3] = 0;
  local_7c[4] = 0;
  local_7c[5] = 0x1e;
  local_8c = &DAT_00799a39 + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x108;
  local_7c[6] = 0x46;
  local_7c[0] = 0x30;
  local_7c[1] = 0xcb;
  local_7c[2] = 0x153;
  local_7c[7] = 0;
  local_7c[8] = 0;
  local_7c[10] = 0xffffffb1;
  local_7c[0xc] = 0xffffff6f;
  local_7c[0xd] = 0;
  local_7c[0xe] = 0xffffffc4;
  local_7c[0x10] = 0xffffff75;
  local_7c[0x12] = 0xffffff33;
  local_7c[0x13] = 0xfffffff1;
  local_7c[0x14] = 0xffffff56;
  local_7c[0x15] = 0x8f;
  local_7c[0x16] = 0xffffff07;
  local_7c[0x17] = 0x11a;
  local_7c[0x18] = 0xfffffec5;
  local_7c[0x19] = 0xffffffdd;
  local_7c[0x1a] = 0xfffffee3;
  local_7c[0x1b] = 0x7b;
  local_7c[0x1c] = 0xfffffe94;
  local_7c[0x1d] = 0x106;
  local_7c[0x1e] = 0xfffffe52;
  do {
    iVar5 = 0;
    local_88 = local_7c;
    do {
      if (local_8c[iVar5] != '\0') {
        iVar4 = DAT_00f5758c;
        if (iVar6 < 4) {
          iVar4 = DAT_00f57584;
        }
        if (uVar3 == 0) {
          iVar1 = local_7c[(iVar6 + iVar5) * 2 + 8] + -99;
          iVar2 = local_7c[(iVar6 + iVar5) * 2 + 7] + 0xf3;
        }
        else {
          if (uVar3 != 3) goto LAB_00402160;
          iVar1 = local_7c[(iVar6 + iVar5) * 2 + 8] + *local_88 + -0x127;
          iVar2 = local_7c[(iVar6 + iVar5) * 2 + 7] + *local_84 + -0x19b;
        }
        FUN_0053e590((uVar3 & 1) + iVar4,iVar2 + param_2,iVar1 + param_3);
      }
LAB_00402160: ;
      iVar5 = iVar5 + 1;
      local_88 = local_88 + 1;
    } while (iVar5 < 3);
    iVar6 = iVar6 + 3;
    local_8c = local_8c + 3;
    local_84 = local_84 + 1;
    if (0xb < iVar6) {
      return;
    }
  } while( true );
}
