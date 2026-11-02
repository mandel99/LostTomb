#include "restore_types.h"
#define DAT_005e2ae4 (*(undefined4 *)0x005e2ae4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
extern void FUN_004dfbb0();

void FUN_004e0050(int param_1,int param_2,undefined1 param_3)

{
  char cVar1;
  short sVar2;
  int iVar3;
  bool bVar4;
  bool bVar5;
  uint uVar6;
  int iVar7;
  uint *puVar8;
  uint uVar9;
  int *piVar10;
  int iVar11;
  uint uVar12;
  uint local_8;
  
  bVar4 = false;
  iVar11 = param_1 * 0x108;
  iVar7 = (int)(short)(&DAT_007999ea)[param_1 * 0x84];
  uVar9 = (&DAT_005e2ae4)[iVar7 * 0xd];
  uVar6 = uVar9 & 0xf00;
  param_2 = param_2 * 0x184;
  cVar1 = (&DAT_00bcd602)[param_2];
  if (cVar1 == 'U') {
    iVar7 = (&DAT_00799a30)[param_1 * 0x42];
    if ((iVar7 != 0) && (iVar7 != 1)) {
      if (iVar7 < 0) {
        (&DAT_00799a30)[param_1 * 0x42] = 0;
      }
      else if (2 < iVar7) {
        (&DAT_00799a30)[param_1 * 0x42] = 2;
      }
      *(undefined4 *)(&DAT_00799a34 + iVar11) = 0;
      (&DAT_00bcd630)[param_2] = param_3;
      return;
    }
    *(short *)(&DAT_00bcd62e + param_2) = *(short *)(&DAT_00bcd62e + param_2) + 1;
    if (0x31 < *(short *)(&DAT_00bcd62e + param_2)) {
      (&DAT_00799a30)[param_1 * 0x42] = iVar7 + 1;
      if (((uVar9 & 0xff) == 10) || ((uVar9 & 0xff) == 0xc)) {
        (&DAT_00799a30)[(param_1 + 1) * 0x42] = iVar7 + 1;
        FUN_004dfbb0(param_1 + 1);
      }
      FUN_004dfbb0(param_1);
      param_1 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
      bVar4 = true;
      bVar5 = true;
      puVar8 = &DAT_005e2ae4;
      piVar10 = &DAT_00799a30 + param_1 * 0x42;
      do {
        if (3 < (*puVar8 & 0xff)) {
          if (*piVar10 == 0) {
            bVar4 = false;
          }
          if (*piVar10 < 2) {
            bVar5 = false;
          }
        }
        puVar8 = puVar8 + 0xd;
        piVar10 = piVar10 + 0x42;
      } while ((int)puVar8 < 0x5e4274);
      if (bVar4) {
        *(uint *)(&DAT_00799a3c + param_1 * 0x84) = *(uint *)(&DAT_00799a3c + param_1 * 0x84) | 4;
      }
      if (bVar5) {
        *(uint *)(&DAT_00799a3c + param_1 * 0x84) = *(uint *)(&DAT_00799a3c + param_1 * 0x84) | 8;
      }
LAB_004e04c0: ;
      *(undefined4 *)(&DAT_00799a34 + iVar11) = 0;
      (&DAT_00bcd630)[param_2] = param_3;
      return;
    }
  }
  else {
    if (cVar1 == 'O') {
      bVar4 = true;
    }
    else if (cVar1 != 'Q') {
      (&DAT_00bcd630)[param_2] = param_3;
      return;
    }
    if (uVar6 < 0x401) {
      if (uVar6 == 0x400) {
        local_8 = 0x40;
        iVar7 = (param_1 - iVar7) + 2;
      }
      else if (uVar6 == 0x100) {
        local_8 = 0x10;
        iVar7 = param_1 - iVar7;
      }
      else {
        if (uVar6 != 0x200) goto LAB_004e03da;
        local_8 = 0x20;
        iVar7 = (param_1 - iVar7) + 1;
      }
    }
    else {
      if (uVar6 != 0x800) goto LAB_004e03da;
      local_8 = 0x80;
      iVar7 = (param_1 - iVar7) + 3;
    }
    if (iVar7 == 0) {
LAB_004e03da: ;
      *(undefined4 *)(&DAT_00799a34 + iVar11) = 0;
      (&DAT_00bcd630)[param_2] = param_3;
      return;
    }
    uVar9 = uVar9 & 0xf000;
    uVar12 = 0;
    uVar6 = (&DAT_00799a30)[iVar7 * 0x42];
    if (uVar9 < 0x4001) {
      if (uVar9 == 0x4000) {
        if ((uVar6 & 0x10) == 0) {
          uVar12 = 0x10;
        }
        else if ((uVar6 & 0x20) == 0) {
          uVar12 = 0x20;
        }
        else if ((uVar6 & 0x1000) == 0) {
          uVar12 = 0x1000;
        }
        else if ((uVar6 & 0x2000) == 0) {
          uVar12 = 0x2000;
        }
      }
      else if (uVar9 == 0x1000) {
        if ((uVar6 & 1) == 0) {
          uVar12 = 1;
        }
        else if ((uVar6 & 2) == 0) {
          uVar12 = 2;
        }
        else if ((uVar6 & 0x100) == 0) {
          uVar12 = 0x100;
        }
        else if ((uVar6 & 0x200) == 0) {
          uVar12 = 0x200;
        }
      }
      else if (uVar9 == 0x2000) {
        if ((uVar6 & 4) == 0) {
          uVar12 = 4;
        }
        else if ((uVar6 & 8) == 0) {
          uVar12 = 8;
        }
        else if ((uVar6 & 0x400) == 0) {
          uVar12 = 0x400;
        }
        else if ((uVar6 & 0x800) == 0) {
          uVar12 = 0x800;
        }
      }
    }
    else if (uVar9 == 0x8000) {
      if ((uVar6 & 0x40) == 0) {
        uVar12 = 0x40;
      }
      else if ((uVar6 & 0x80) == 0) {
        uVar12 = 0x80;
      }
      else if ((uVar6 & 0x4000) == 0) {
        uVar12 = 0x4000;
      }
      else if ((uVar6 & 0x8000) == 0) {
        uVar12 = 0x8000;
      }
    }
    if (bVar4) {
      if (local_8 == 0) {
        *(undefined4 *)(&DAT_00799a34 + iVar11) = 0;
        (&DAT_00bcd630)[param_2] = param_3;
        return;
      }
      if (*(short *)(&DAT_00bcd62e + param_2) < 0) {
        *(undefined2 *)(&DAT_00bcd62e + param_2) = 0;
      }
      *(short *)(&DAT_00bcd62e + param_2) = *(short *)(&DAT_00bcd62e + param_2) + 1;
      if (0x31 < *(short *)(&DAT_00bcd62e + param_2)) {
        *(undefined4 *)(&DAT_00799a34 + iVar11) = 0;
        sVar2 = (&DAT_007999ea)[param_1 * 0x84];
        (&DAT_00bcd630)[param_2] = param_3;
        *(uint *)(&DAT_00799a3c + (param_1 - sVar2) * 0x84) =
             *(uint *)(&DAT_00799a3c + (param_1 - sVar2) * 0x84) | local_8;
        FUN_004dfbb0(iVar7);
        return;
      }
    }
    else {
      if (uVar12 == 0) {
        *(undefined4 *)(&DAT_00799a34 + iVar11) = 0;
        (&DAT_00bcd630)[param_2] = param_3;
        return;
      }
      if (*(short *)(&DAT_00bcd62e + param_2) < 0) {
        *(undefined2 *)(&DAT_00bcd62e + param_2) = 0;
      }
      *(short *)(&DAT_00bcd62e + param_2) = *(short *)(&DAT_00bcd62e + param_2) + 1;
      if (0x31 < *(short *)(&DAT_00bcd62e + param_2)) {
        (&DAT_00799a30)[iVar7 * 0x42] = uVar6 | uVar12;
        FUN_004dfbb0(iVar7);
        bVar4 = true;
        iVar7 = 0;
        bVar5 = true;
        do {
          iVar3 = *(int *)((int)&DAT_00799a30 +
                          iVar7 + (short)(&DAT_007999ea)[param_1 * 0x84] * -0x108 + iVar11);
          if (iVar3 < 0xffff) {
            bVar4 = false;
          }
          if ((char)iVar3 != -1) {
            bVar5 = false;
          }
          iVar7 = iVar7 + 0x108;
        } while (iVar7 < 0x420);
        if (bVar4) {
          iVar7 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
          *(uint *)(&DAT_00799a3c + iVar7 * 0x84) = *(uint *)(&DAT_00799a3c + iVar7 * 0x84) | 2;
        }
        if (bVar5) {
          *(uint *)(&DAT_00799a3c + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84) =
               *(uint *)(&DAT_00799a3c + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84)
               | 1;
        }
        goto LAB_004e04c0;
      }
    }
  }
  return;
}
