#include "restore_types.h"
#define DAT_005f80fc (*(undefined4 *)0x005f80fcu)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00c8ad38 (*(undefined4 *)0x00c8ad38u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00eae004 (*(undefined *)0x00eae004u)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define DAT_00f533c7 (*(undefined4 *)0x00f533c7u)
#define DAT_00f533c8 (*(undefined4 *)0x00f533c8u)
extern undefined4 FUN_0051cba0();

int FUN_0051e720(int param_1,undefined4 param_2,undefined4 param_3,int *param_4,int param_5,
                int param_6,uint *param_7)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  byte *pbVar4;
  uint uVar5;
  int iVar6;
  undefined1 *puVar7;
  int *piVar8;
  int iVar9;
  uint uVar10;
  int iVar11;
  int iVar12;
  int local_10;
  int local_4;
  
  uVar10 = DAT_00d3b374 & 3;
  DAT_00eae004 = DAT_00bca2d8 + param_5 * 0xe4 + (int)param_4;
  iVar3 = (&DAT_00eed760)[DAT_00eae004];
  if (0 < iVar3) {
    iVar12 = 0;
    local_4 = -1;
    iVar11 = DAT_00eae004;
    piVar8 = &DAT_00eed760 + DAT_00eae004;
    local_10 = (int)param_4;
    do {
      param_4 = piVar8;
      if (iVar3 < 2) {
        if (param_1 < 1000) {
          iVar3 = 0;
          if (0 < iVar12) {
            puVar7 = &DAT_00f533c7 + iVar12;
            do {
              *(undefined1 *)((int)&DAT_00c8ad38 + param_1 * 500 + iVar3) = *puVar7;
              iVar3 = iVar3 + 1;
              puVar7 = puVar7 + -1;
            } while (iVar3 < iVar12);
            return iVar12;
          }
        }
        else if ((param_7 != (uint *)0x0) && (0 < iVar12)) {
          pbVar4 = &DAT_00f533c7 + iVar12;
          iVar3 = iVar12;
          do {
            bVar1 = *pbVar4;
            pbVar4 = pbVar4 + -1;
            *param_7 = (uint)bVar1;
            iVar3 = iVar3 + -1;
            param_7 = param_7 + 1;
          } while (iVar3 != 0);
        }
        return iVar12;
      }
      iVar3 = *param_4;
      uVar5 = uVar10;
      if (param_6 == 1) {
        uVar5 = (byte)(&DAT_00acf85c)[iVar11] & 3;
      }
      iVar9 = -1;
      FUN_0051cba0(local_10,param_5,param_2,param_3);
      iVar6 = 0;
      piVar8 = &DAT_005f80fc;
      do {
        if (((iVar6 != local_4) && (iVar2 = (&DAT_00eed760)[*piVar8 + iVar11], iVar2 != 0)) &&
           ((iVar2 < iVar3 || ((iVar2 == iVar3 && (uVar10 == uVar5)))))) {
          iVar9 = iVar6;
          iVar3 = iVar2;
        }
        piVar8 = piVar8 + 2;
        iVar6 = iVar6 + 2;
      } while ((int)piVar8 < 0x5f811c);
      if (iVar9 == -1) {
        return 0;
      }
      if (iVar9 == 0) {
        param_5 = param_5 + -1;
        iVar11 = iVar11 + -0xe4;
        param_4 = param_4 + -0xe4;
      }
      else if (iVar9 == 2) {
        local_10 = local_10 + 1;
        iVar11 = iVar11 + 1;
        param_4 = param_4 + 1;
      }
      else if (iVar9 == 4) {
        param_5 = param_5 + 1;
        iVar11 = iVar11 + 0xe4;
        param_4 = param_4 + 0xe4;
      }
      else if (iVar9 == 6) {
        local_10 = local_10 + -1;
        iVar11 = iVar11 + -1;
        param_4 = param_4 + -1;
      }
      local_4 = iVar9 + 4;
      if (7 < local_4) {
        local_4 = iVar9 + -4;
      }
      (&DAT_00f533c8)[iVar12] = (char)local_4;
      iVar12 = iVar12 + 1;
      piVar8 = param_4;
    } while (iVar12 < 500);
  }
  return 0;
}
