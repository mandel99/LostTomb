#include "restore_types.h"
#define DAT_005f80fc (*(undefined4 *)0x005f80fcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00c8ad38 (*(undefined4 *)0x00c8ad38u)
#define DAT_00eae004 (*(undefined *)0x00eae004u)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define DAT_00f533c7 (*(undefined4 *)0x00f533c7u)
#define DAT_00f533c8 (*(undefined4 *)0x00f533c8u)
extern undefined4 FUN_0051cba0();

int FUN_0051d980(int param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5,
                int param_6)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  undefined1 *puVar4;
  int *piVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int *local_18;
  int local_14;
  int local_10;
  uint local_4;
  
  DAT_00eae004 = DAT_00bca2d8 + param_5 * 0xe4 + param_4;
  iVar2 = (&DAT_00eed760)[DAT_00eae004];
  local_18 = &DAT_00eed760 + DAT_00eae004;
  if (0 < iVar2) {
    iVar7 = 0;
    local_4 = 0xffffffff;
    iVar8 = DAT_00eae004;
    local_14 = param_4;
    local_10 = param_5;
    do {
      if (iVar2 < 2) {
        iVar2 = 0;
        if (0 < iVar7) {
          puVar4 = &DAT_00f533c7 + iVar7;
          do {
            *(undefined1 *)((int)&DAT_00c8ad38 + param_1 * 500 + iVar2) = *puVar4;
            iVar2 = iVar2 + 1;
            puVar4 = puVar4 + -1;
          } while (iVar2 < iVar7);
        }
        return iVar7;
      }
      uVar6 = 0xffffffff;
      iVar2 = *local_18;
      iVar9 = local_14;
      iVar10 = local_10;
      if (iVar7 <= param_6) {
        iVar9 = param_4;
        iVar10 = param_5;
      }
      uVar1 = FUN_0051cba0(iVar9,iVar10,param_2,param_3);
      uVar3 = 0;
      piVar5 = &DAT_005f80fc;
      do {
        if ((uVar3 != local_4) && (iVar9 = (&DAT_00eed760)[*piVar5 + iVar8], iVar9 != 0)) {
          if (param_6 < iVar7) {
            if ((iVar9 < iVar2) ||
               ((iVar9 == iVar2 && ((uVar3 == (uVar1 & 0xff) || (uVar6 == 0xffffffff))))))
            goto LAB_0051da6e;
          }
          else if ((iVar9 <= iVar2) && (uVar3 == (uVar1 & 0xff))) {
LAB_0051da6e: ;
            uVar6 = uVar3;
            iVar2 = iVar9;
          }
        }
        piVar5 = piVar5 + 1;
        uVar3 = uVar3 + 1;
      } while ((int)piVar5 < 0x5f811c);
      if (uVar6 == 0xffffffff) {
        return 0;
      }
      if (uVar6 == 0) {
        local_10 = local_10 + -1;
        iVar8 = iVar8 + -0xe4;
        local_18 = local_18 + -0xe4;
      }
      else if (uVar6 == 1) {
        local_10 = local_10 + -1;
        local_14 = local_14 + 1;
        iVar8 = iVar8 + -0xe3;
        local_18 = local_18 + -0xe3;
      }
      else if (uVar6 == 2) {
        local_14 = local_14 + 1;
        iVar8 = iVar8 + 1;
        local_18 = local_18 + 1;
      }
      else if (uVar6 == 3) {
        local_14 = local_14 + 1;
        local_10 = local_10 + 1;
        iVar8 = iVar8 + 0xe5;
        local_18 = local_18 + 0xe5;
      }
      else if (uVar6 == 4) {
        local_10 = local_10 + 1;
        iVar8 = iVar8 + 0xe4;
        local_18 = local_18 + 0xe4;
      }
      else if (uVar6 == 5) {
        local_10 = local_10 + 1;
        local_14 = local_14 + -1;
        iVar8 = iVar8 + 0xe3;
        local_18 = local_18 + 0xe3;
      }
      else if (uVar6 == 6) {
        local_14 = local_14 + -1;
        iVar8 = iVar8 + -1;
        local_18 = local_18 + -1;
      }
      else if (uVar6 == 7) {
        local_14 = local_14 + -1;
        local_10 = local_10 + -1;
        iVar8 = iVar8 + -0xe5;
        local_18 = local_18 + -0xe5;
      }
      local_4 = uVar6 + 4;
      if (7 < (int)local_4) {
        local_4 = uVar6 - 4;
      }
      (&DAT_00f533c8)[iVar7] = (char)local_4;
      iVar7 = iVar7 + 1;
    } while (iVar7 < 500);
  }
  return 0;
}
