#include "restore_types.h"
#define DAT_005f80fc (*(undefined4 *)0x005f80fcu)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00c8ad38 (*(undefined4 *)0x00c8ad38u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00eae004 (*(undefined *)0x00eae004u)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define DAT_00f533c7 (*(undefined4 *)0x00f533c7u)
#define DAT_00f533c8 (*(undefined4 *)0x00f533c8u)
extern undefined4 FUN_0051cba0();

int FUN_0051dc10(int param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5,
                int param_6)

{
  int iVar1;
  byte bVar2;
  int iVar3;
  byte bVar4;
  int iVar5;
  undefined1 *puVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  int *piVar11;
  byte local_1d;
  int *local_1c;
  undefined4 *local_18;
  int local_14;
  int local_10;
  uint local_c;
  int local_4;
  
  uVar8 = DAT_00d3b374 & 3;
  iVar5 = DAT_00bca2d8 + param_5 * 0xe4 + param_4;
  local_1c = &DAT_00eed760 + iVar5;
  if ((int)(&DAT_00eed760)[iVar5] < 1) {
    DAT_00eae004 = iVar5;
    return 0;
  }
  iVar10 = 0;
  local_10 = 0;
  local_4 = -1;
  DAT_00eae004 = iVar5;
  if (1 < (int)(&DAT_00eed760)[iVar5]) {
    local_18 = &DAT_00af598c + iVar5;
    do {
      local_14 = *local_1c;
      local_1d = (byte)((uint)*local_18 >> 0x10) & 1;
      local_c = uVar8;
      if (param_6 == 1) {
        local_c = (byte)(&DAT_00acf85c)[iVar5] & 3;
      }
      FUN_0051cba0(param_4,param_5,param_2,param_3);
      iVar9 = 0;
      piVar11 = &DAT_005f80fc;
      iVar10 = -1;
      do {
        iVar7 = iVar10;
        bVar2 = local_1d;
        iVar3 = local_14;
        if (iVar9 != local_4) {
          iVar1 = (&DAT_00eed760)[*piVar11 + iVar5];
          bVar4 = (byte)((uint)(&DAT_00af598c)[*piVar11 + iVar5] >> 0x10) & 1;
          if ((iVar1 != 0) &&
             ((iVar1 < local_14 ||
              ((iVar1 == local_14 &&
               (((local_1d == 0 ||
                 (iVar7 = iVar9, bVar2 = bVar4, iVar3 = iVar1,
                 ((&DAT_00af598c)[*piVar11 + iVar5] & 0x10000) != 0)) &&
                (iVar7 = iVar10, bVar2 = local_1d, iVar3 = local_14, uVar8 == local_c)))))))) {
            iVar7 = iVar9;
            bVar2 = bVar4;
            iVar3 = iVar1;
          }
        }
        local_14 = iVar3;
        local_1d = bVar2;
        piVar11 = piVar11 + 1;
        iVar9 = iVar9 + 1;
        iVar10 = iVar7;
      } while ((int)piVar11 < 0x5f811c);
      if (iVar7 == -1) {
        return 0;
      }
      if (iVar7 == 0) {
        iVar5 = iVar5 + -0xe4;
        iVar10 = 0x390;
LAB_0051ddea: ;
        iVar10 = -iVar10;
LAB_0051ddfa: ;
        local_1c = (int *)((int)local_1c + iVar10);
        local_18 = (undefined4 *)((int)local_18 + iVar10);
      }
      else {
        if (iVar7 == 1) {
          iVar5 = iVar5 + -0xe3;
          iVar10 = 0x38c;
          goto LAB_0051ddea;
        }
        if (iVar7 == 2) {
          iVar5 = iVar5 + 1;
          iVar10 = 4;
          goto LAB_0051ddfa;
        }
        if (iVar7 == 3) {
          iVar5 = iVar5 + 0xe5;
          iVar10 = 0x394;
          goto LAB_0051ddfa;
        }
        iVar10 = 4;
        if (iVar7 == 4) {
          iVar5 = iVar5 + 0xe4;
          iVar10 = 0x390;
          goto LAB_0051ddfa;
        }
        if (iVar7 == 5) {
          iVar5 = iVar5 + 0xe3;
          iVar10 = 0x38c;
          goto LAB_0051ddfa;
        }
        if (iVar7 == 6) {
          iVar5 = iVar5 + -1;
          goto LAB_0051ddea;
        }
        if (iVar7 == 7) {
          iVar5 = iVar5 + -0xe5;
          iVar10 = 0x394;
          goto LAB_0051ddea;
        }
      }
      local_4 = iVar7 + 4;
      if (7 < local_4) {
        local_4 = iVar7 + -4;
      }
      (&DAT_00f533c8)[local_10] = (char)local_4;
      iVar10 = local_10 + 1;
      if (499 < iVar10) {
        return 0;
      }
      local_10 = iVar10;
    } while (1 < local_14);
  }
  iVar5 = 0;
  if (0 < iVar10) {
    puVar6 = &DAT_00f533c7 + iVar10;
    do {
      *(undefined1 *)((int)&DAT_00c8ad38 + param_1 * 500 + iVar5) = *puVar6;
      iVar5 = iVar5 + 1;
      puVar6 = puVar6 + -1;
    } while (iVar5 < iVar10);
  }
  return iVar10;
}
