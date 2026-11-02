#include "restore_types.h"
#define DAT_005f80fc (*(undefined4 *)0x005f80fcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00c8ad38 (*(undefined4 *)0x00c8ad38u)
#define DAT_00eae004 (*(undefined *)0x00eae004u)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define DAT_00f533c7 (*(undefined4 *)0x00f533c7u)
#define DAT_00f533c8 (*(undefined4 *)0x00f533c8u)
extern undefined4 FUN_0051cba0();

int FUN_0051b400(int param_1,int param_2,undefined4 param_3,undefined4 param_4,int param_5,
                int param_6)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  undefined1 *puVar6;
  int iVar7;
  int *piVar8;
  uint uVar9;
  uint uVar10;
  int iVar11;
  int iVar12;
  int *piVar13;
  int iVar14;
  uint local_c;
  
  DAT_00eae004 = DAT_00bca2d8 + param_6 * 0xe4 + param_5;
  iVar5 = (&DAT_00eed760)[DAT_00eae004];
  piVar13 = &DAT_00eed760 + DAT_00eae004;
  if (0 < iVar5) {
    local_c = 0xffffffff;
    iVar7 = (param_1 != 8) + 1;
    iVar11 = 0;
    iVar14 = DAT_00eae004;
    do {
      if (iVar5 < 2) {
        iVar5 = 0;
        if (0 < iVar11) {
          puVar6 = &DAT_00f533c7 + iVar11;
          do {
            *(undefined1 *)((int)&DAT_00c8ad38 + param_2 * 500 + iVar5) = *puVar6;
            iVar5 = iVar5 + 1;
            puVar6 = puVar6 + -1;
          } while (iVar5 < iVar11);
        }
        return iVar11;
      }
      param_1 = *piVar13;
      uVar3 = FUN_0051cba0(param_5,param_6,param_3,param_4);
      uVar4 = 0;
      piVar8 = &DAT_005f80fc;
      uVar9 = 0xffffffff;
      iVar12 = param_1;
      do {
        uVar10 = uVar9;
        iVar5 = iVar12;
        iVar2 = param_1;
        if ((((uVar4 != local_c) && (iVar1 = (&DAT_00eed760)[*piVar8 + iVar14], iVar1 != 0)) &&
            (uVar10 = uVar4, iVar5 = iVar1, iVar2 = iVar1, iVar12 <= iVar1)) &&
           (((uVar10 = uVar9, iVar5 = iVar12, iVar2 = param_1, iVar1 == iVar12 &&
             (uVar10 = uVar4, iVar5 = iVar1, iVar2 = iVar1, uVar4 != (uVar3 & 0xff))) &&
            (uVar10 = uVar9, iVar5 = param_1, iVar2 = param_1, uVar9 == 0xffffffff)))) {
          uVar10 = uVar4;
          iVar5 = iVar1;
          iVar2 = iVar1;
        }
        param_1 = iVar2;
        uVar4 = uVar4 + iVar7;
        piVar8 = piVar8 + iVar7;
        uVar9 = uVar10;
        iVar12 = iVar5;
      } while ((int)piVar8 < 0x5f811c);
      if (uVar10 == 0xffffffff) {
        return 0;
      }
      if (uVar10 == 0) {
        iVar14 = iVar14 + -0xe4;
        param_6 = param_6 + -1;
        piVar13 = piVar13 + -0xe4;
      }
      else if (uVar10 == 1) {
        param_6 = param_6 + -1;
        param_5 = param_5 + 1;
        iVar14 = iVar14 + -0xe3;
        piVar13 = piVar13 + -0xe3;
      }
      else if (uVar10 == 2) {
        param_5 = param_5 + 1;
        iVar14 = iVar14 + 1;
        piVar13 = piVar13 + 1;
      }
      else if (uVar10 == 3) {
        param_5 = param_5 + 1;
        param_6 = param_6 + 1;
        iVar14 = iVar14 + 0xe5;
        piVar13 = piVar13 + 0xe5;
      }
      else if (uVar10 == 4) {
        iVar14 = iVar14 + 0xe4;
        param_6 = param_6 + 1;
        piVar13 = piVar13 + 0xe4;
      }
      else if (uVar10 == 5) {
        param_6 = param_6 + 1;
        param_5 = param_5 + -1;
        iVar14 = iVar14 + 0xe3;
        piVar13 = piVar13 + 0xe3;
      }
      else if (uVar10 == 6) {
        param_5 = param_5 + -1;
        iVar14 = iVar14 + -1;
        piVar13 = piVar13 + -1;
      }
      else if (uVar10 == 7) {
        param_5 = param_5 + -1;
        param_6 = param_6 + -1;
        iVar14 = iVar14 + -0xe5;
        piVar13 = piVar13 + -0xe5;
      }
      local_c = uVar10 + 4;
      if (7 < (int)local_c) {
        local_c = uVar10 - 4;
      }
      (&DAT_00f533c8)[iVar11] = (char)local_c;
      iVar11 = iVar11 + 1;
    } while (iVar11 < 500);
  }
  return 0;
}
