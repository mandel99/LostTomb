#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
#define DAT_00f5695c (*(undefined *)0x00f5695cu)
#include "paving_neighbors.h"
extern void FUN_0047bf50();

void FUN_00475720(uint param_1,short *param_2,uint param_3,uint param_4,int param_5)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int local_4;
  
  uVar2 = param_4;
  uVar6 = param_1;
  if ((int)param_3 < (int)param_1) {
    param_1 = param_3;
    param_3 = uVar6;
  }
  uVar6 = (uint)param_2;
  if ((int)param_4 < (int)param_2) {
    param_4 = (uint)param_2;
    uVar6 = uVar2;
  }
  if ((int)param_1 < 0) {
    param_1 = 0;
  }
  if ((int)uVar6 < 0) {
    uVar6 = 0;
  }
  if (DAT_00bca2d0 <= (int)param_3) {
    param_3 = DAT_00bca2d0 - 1;
  }
  if (DAT_00bca2d4 <= (int)param_4) {
    param_4 = DAT_00bca2d4 - 1;
  }
  iVar5 = DAT_00bca2d0;
  if (param_5 != 0) {
    iVar3 = DAT_00bca2d0 - param_3;
    iVar4 = DAT_00bca2d8 + uVar6 * 0xe4 + param_1;
    if ((int)uVar6 <= (int)param_4) {
      local_4 = (param_4 - uVar6) + 1;
      do {
        if ((int)param_1 <= (int)param_3) {
          param_2 = (short *)(&DAT_00b4e6fc + iVar4 * 2);
          iVar7 = (param_3 - param_1) + 1;
          do {
            if ((((*(byte *)(&DAT_00af598c + iVar4) & 0x40) != 0) &&
                (bVar1 = (&DAT_00b285cc)[iVar4], (bVar1 & 0x80) != 0)) &&
               ((*param_2 == 0 || ((&DAT_007999e8)[*param_2 * 0x84] != 0x8a)))) {
              (&DAT_00b7482c)[iVar4] = 0;
              (&DAT_00b285cc)[iVar4] = bVar1 & 0xf0;
              (&DAT_00b67d1c)[iVar4] = (&DAT_00b67d1c)[iVar4] | 0x40;
            }
            iVar4 = iVar4 + 1;
            param_2 = param_2 + 1;
            iVar7 = iVar7 + -1;
            iVar5 = DAT_00bca2d0;
          } while (iVar7 != 0);
        }
        iVar4 = iVar4 + DAT_00bca2dc + iVar3 + -1 + param_1;
        local_4 = local_4 + -1;
      } while (local_4 != 0);
    }
  }
  iVar3 = DAT_00bca2d8 + uVar6 * 0xe4 + param_1;
  for (; uVar2 = param_1, (int)uVar6 <= (int)param_4; uVar6 = uVar6 + 1) {
    for (; (int)uVar2 <= (int)param_3; uVar2 = uVar2 + 1) {
      if ((((*(byte *)(&DAT_00af598c + iVar3) & 0x40) != 0) && ((&DAT_00b7482c)[iVar3] == 0)) &&
         (((&DAT_00b285cc)[iVar3] & 0x80) != 0)) {
        bVar1 = (&DAT_00acf85c)[iVar3];
        iVar4 = FUN_00475670(iVar3);
        if (iVar4 == 0) {
          if (((uVar2 & 1) == 0) || ((uVar6 & 1) == 0)) {
            if (((uVar2 & 1) == 0) && ((uVar6 & 1) == 0)) {
              (&DAT_00b7482c)[iVar3] = DAT_00f5695c;
            }
            else {
              (&DAT_00b7482c)[iVar3] = DAT_00f5695c + 2;
            }
          }
          else {
            (&DAT_00b7482c)[iVar3] = DAT_00f5695c + 1;
          }
          (&DAT_00b67d1c)[iVar3] = (&DAT_00b67d1c)[iVar3] | 0x40;
        }
        else {
          if ((bVar1 & 1) == 0) {
            iVar4 = DAT_00f5695c + 6;
          }
          else {
            iVar4 = DAT_00f5695c + 7;
          }
          FUN_0047bf50(0,uVar2,uVar6,2,2,iVar4,0x40);
        }
      }
      iVar3 = iVar3 + 1;
    }
    iVar3 = iVar3 + DAT_00bca2dc + (iVar5 - param_3) + -1 + param_1;
  }
  return;
}
