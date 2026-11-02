#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00a76aec (*(undefined4 *)0x00a76aecu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00ea5f1c (*(undefined4 *)0x00ea5f1cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

undefined4 FUN_00480460(int param_1,int param_2,uint *param_3)

{
  uint uVar1;
  short *psVar2;
  uint *puVar3;
  int iVar4;
  uint *puVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  short *local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_14;
  int local_10;
  
  iVar4 = param_2;
  iVar6 = param_1;
  if ((param_3 == (uint *)0x0) || (param_3 == (uint *)0x4)) {
    param_1 = 9;
    param_2 = 0xf;
    iVar6 = iVar6 + -1;
    iVar4 = iVar4 + -3;
  }
  else {
    param_1 = 0xf;
    param_2 = 9;
    iVar6 = iVar6 + -3;
    iVar4 = iVar4 + -1;
  }
  local_24 = 0xc;
  iVar6 = DAT_00bca2d8 + iVar4 * 0xe4 + iVar6;
  local_10 = 0;
  if (param_2 != 0) {
    local_2c = (short *)(&DAT_00b4e6fc + iVar6 * 2);
    param_3 = &DAT_00af598c + iVar6;
    local_28 = iVar6;
    local_14 = iVar6;
    do {
      if ((local_10 == 0) || (iVar4 = param_1 + -1, local_10 == param_2 + -1)) {
        iVar4 = 1;
      }
      local_20 = 0;
      if (param_1 != 0) {
        psVar2 = local_2c;
        puVar5 = param_3;
        iVar7 = local_28;
        do {
          uVar1 = *puVar5;
          if (((((uVar1 & 8) == 0) ||
               (((&DAT_007999e8)[*psVar2 * 0x84] != 0x8a &&
                ((&DAT_007999e8)[*psVar2 * 0x84] != 0x88)))) && ((uVar1 & 0x40) != 0)) &&
             ((uVar1 & 4) == 0)) {
            iVar6 = 0;
            puVar3 = &DAT_00ea5f1c + DAT_00eaac28 * 0x1276;
            do {
              iVar8 = iVar6;
              if (*puVar3 == (uint)(byte)(&DAT_00a76aec)[iVar7]) break;
              iVar6 = iVar6 + 1;
              puVar3 = puVar3 + 2;
              iVar8 = 0xb;
            } while (iVar6 < 10);
            if (iVar8 < local_24) {
              local_24 = iVar8;
              local_14 = iVar7;
            }
          }
          psVar2 = psVar2 + iVar4 * 0xe4;
          puVar5 = puVar5 + iVar4 * 0xe4;
          local_20 = local_20 + iVar4;
          iVar7 = iVar7 + iVar4 * 0xe4;
          iVar6 = local_14;
        } while (local_20 < param_1);
      }
      local_10 = local_10 + 1;
      param_3 = param_3 + 1;
      local_2c = local_2c + 1;
      local_28 = local_28 + 1;
    } while (local_10 < param_2);
    if (local_24 < 0xc) {
      _DAT_00d3b4e4 = (iVar6 - DAT_00bca2d8) % 0xe4;
      _DAT_00d3b4e0 = (iVar6 - DAT_00bca2d8) / 0xe4;
      return 1;
    }
  }
  return 0;
}
