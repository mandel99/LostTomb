#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00a9010c (*(undefined4 *)0x00a9010cu)
#define DAT_00a9cc1c (*(undefined4 *)0x00a9cc1cu)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00f56c4c (*(undefined *)0x00f56c4cu)
#define _DAT_00bc9734 (*(undefined4 *)0x00bc9734u)
extern void FUN_0047a220();

void FUN_0047c2b0(int *param_1,int param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  uint *puVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int local_c;
  int *local_8;
  
  iVar4 = (int)param_1;
  if (param_2 < 0) {
    _DAT_00bc9734 = 1;
    return;
  }
  if (param_3 < 0) {
    _DAT_00bc9734 = 1;
    return;
  }
  if (DAT_00bca2d0 <= param_2 + -1 + param_4) {
    _DAT_00bc9734 = 1;
    return;
  }
  if (param_3 + -1 + param_5 < DAT_00bca2d4) {
    local_c = 0;
    iVar1 = DAT_00bca2d8 + param_3 * 0xe4;
    if (0 < param_5) {
      local_8 = &DAT_005cc8c0;
      do {
        iVar7 = 0;
        if (0 < param_4) {
          param_1 = local_8;
          do {
            iVar5 = *param_1 + iVar1 + param_2;
            if (*(short *)(&DAT_00b4e6fc + iVar5 * 2) == iVar4) {
              if ((iVar4 != 0) && ((&DAT_007999e8)[iVar4 * 0x84] != 99)) {
                (&DAT_00a9010c)[iVar5] = *(undefined1 *)(&DAT_007999e8 + iVar4 * 0x84);
              }
              puVar2 = &DAT_00af598c + iVar5;
              (&DAT_00ae8e7c)[iVar5] = 0;
              (&DAT_00b285cc)[iVar5] = (&DAT_00b285cc)[iVar5] & 0xe0;
              uVar3 = *puVar2;
              *(undefined2 *)(&DAT_00b4e6fc + iVar5 * 2) = 0;
              uVar6 = uVar3 & 4;
              *(undefined2 *)(&DAT_00a9cc1c + iVar5 * 2) = 0;
              (&DAT_00adc36c)[iVar5] = 0;
              (&DAT_00b67d1c)[iVar5] = 0x40;
              if (uVar6 == 0) {
                *puVar2 = uVar3 & 0x3052e80 | 0x1000;
                (&DAT_00b7482c)[iVar5] = ((byte)(&DAT_00acf85c)[iVar5] & 7) + DAT_00f56c4c;
              }
              else {
                *puVar2 = uVar6;
                FUN_0047a220(iVar7 + param_2,local_c + param_3,1);
              }
            }
            iVar7 = iVar7 + 1;
            param_1 = param_1 + 2;
          } while (iVar7 < param_4);
        }
        local_c = local_c + 1;
        local_8 = local_8 + 0xc;
      } while (local_c < param_5);
    }
    return;
  }
  _DAT_00bc9734 = 1;
  return;
}
