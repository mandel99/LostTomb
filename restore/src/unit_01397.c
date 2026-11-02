#include "restore_types.h"
#define DAT_00709a58 (*(undefined *)0x00709a58u)
#define DAT_00709a90 (*(undefined *)0x00709a90u)
#define DAT_00709ac8 (*(undefined *)0x00709ac8u)
#define DAT_00709cc0 (*(undefined *)0x00709cc0u)
#define DAT_00709cf8 (*(undefined *)0x00709cf8u)
#define DAT_00709d30 (*(undefined *)0x00709d30u)
#define DAT_0070a698 (*(undefined *)0x0070a698u)
#define DAT_00775900 (*(undefined *)0x00775900u)
#define DAT_00bcba0c (*(undefined4 *)0x00bcba0cu)
#define DAT_00bcba32 (*(undefined4 *)0x00bcba32u)
#define DAT_00bcba39 (*(undefined4 *)0x00bcba39u)
#define DAT_00bcba40 (*(undefined4 *)0x00bcba40u)
#define DAT_00bcba50 (*(undefined4 *)0x00bcba50u)
#define DAT_00bcd605 (*(undefined4 *)0x00bcd605u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd658 (*(undefined4 *)0x00bcd658u)
#define DAT_00bcd65d (*(undefined4 *)0x00bcd65du)
#define DAT_00bcd728 (*(undefined4 *)0x00bcd728u)
#define DAT_00bcd764 (*(undefined4 *)0x00bcd764u)
#define DAT_00bcd76c (*(undefined4 *)0x00bcd76cu)
#define DAT_00bcd770 (*(undefined4 *)0x00bcd770u)
#define DAT_00bcd774 (*(undefined4 *)0x00bcd774u)
extern int FUN_004b4800();
extern void FUN_004b5af0();
extern int FUN_004b6ff0();
extern int FUN_004cde40();

void FUN_004ad4c0(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined4 uVar9;
  short sVar10;
  short sVar11;
  undefined4 uVar13;
  int iVar14;
  int iVar15;
  int *piVar16;
  undefined4 local_64;
  int *local_60;
  undefined1 local_5c;
  int local_54;
  int local_50;
  undefined2 local_48;
  undefined2 local_44;
  int local_3c [7];
  int local_20;
  int local_1c;
  int local_18;
  int local_10;
  int local_c;
  int local_8;
  short sVar12;
  
  iVar4 = param_1 * 0x184;
  iVar1 = *(int *)(&DAT_00bcd76c + iVar4);
  iVar8 = *(int *)(&DAT_00bcd764 + iVar4);
  uVar2 = *(undefined4 *)(&DAT_00bcd770 + iVar4);
  uVar3 = *(undefined4 *)(&DAT_00bcd774 + iVar4);
  DAT_00775900 = DAT_00775900 + 1;
  if (32000 < DAT_00775900) {
    DAT_00775900 = 1;
  }
  uVar5 = DAT_00709cc0;
  uVar9 = DAT_00709d30;
  uVar13 = DAT_00709cf8;
  if ((iVar1 != 0xc) &&
     (uVar5 = DAT_00709a90, uVar9 = DAT_00709ac8, uVar13 = DAT_00709a58, iVar1 == 0xd)) {
    uVar5 = DAT_0070a698;
    uVar9 = 0;
    uVar13 = 0;
  }
  FUN_004cde40(iVar8,uVar5);
  iVar6 = FUN_004cde40(iVar8,uVar13);
  iVar7 = FUN_004cde40(iVar8,uVar9);
  iVar15 = (iVar8 - iVar7) - iVar6;
  piVar16 = local_3c + 3;
  for (iVar8 = 0xc; iVar8 != 0; iVar8 = iVar8 + -1) {
    *piVar16 = 0;
    piVar16 = piVar16 + 1;
  }
  local_3c[0] = 0;
  local_3c[1] = 0;
  local_3c[2] = 0;
  if (0 < iVar15) {
    if (iVar15 < 0x11) {
      local_3c[0] = 1;
    }
    else if (iVar15 < 0x21) {
      local_3c[0] = 2;
      local_3c[4] = iVar15 / 2;
      iVar15 = iVar15 - iVar15 / 2;
    }
    else {
      local_3c[0] = 3;
      local_3c[5] = iVar15 / 3;
      local_3c[4] = local_3c[5];
      iVar15 = iVar15 + local_3c[5] * -2;
    }
    local_3c[3] = iVar15;
  }
  if (0 < iVar6) {
    if (iVar6 < 0x11) {
      local_3c[1] = 1;
    }
    else if (iVar6 < 0x21) {
      local_3c[1] = 2;
      local_1c = iVar6 / 2;
      iVar6 = iVar6 - iVar6 / 2;
    }
    else {
      local_3c[1] = 3;
      local_18 = iVar6 / 3;
      local_1c = local_18;
      iVar6 = iVar6 + local_18 * -2;
    }
    local_20 = iVar6;
  }
  if (0 < iVar7) {
    if (iVar7 < 0x11) {
      local_3c[2] = 1;
    }
    else if (iVar7 < 0x21) {
      local_3c[2] = 2;
      local_c = iVar7 / 2;
      iVar7 = iVar7 - iVar7 / 2;
    }
    else {
      local_3c[2] = 3;
      local_8 = iVar7 / 3;
      local_c = local_8;
      iVar7 = iVar7 + local_8 * -2;
    }
    local_10 = iVar7;
  }
  sVar10 = 0;
  sVar12 = 0;
  local_50 = 0;
  do {
    local_54 = local_3c[local_50];
    if (0 < local_54) {
      if (iVar1 == 0xc) {
        if (local_50 == 1) {
          param_1 = 0x37;
        }
        else if (local_50 == 2) {
          param_1 = 0x38;
        }
        else {
          param_1 = 0x36;
        }
LAB_004ad710: ;
        local_64 = 10;
      }
      else {
        if (iVar1 != 0xd) {
          if (local_50 == 1) {
            param_1 = 0x2b;
          }
          else if (local_50 == 2) {
            param_1 = 0x2d;
          }
          else {
            param_1 = 0x2c;
          }
          goto LAB_004ad710;
        }
        param_1 = 99;
        local_64 = 8;
      }
      if (0 < local_54) {
        local_60 = local_3c + local_50 * 4 + 3;
        do {
          iVar8 = FUN_004b6ff0(param_1,local_64,0,param_2,param_3);
          if (0 < iVar8) {
            local_48 = (undefined2)uVar2;
            local_5c = (undefined1)iVar1;
            iVar7 = 0;
            iVar6 = iVar8 * 0x90;
            *(undefined2 *)(&DAT_00bcba0c + iVar6) = local_48;
            local_44 = (undefined2)uVar3;
            (&DAT_00bcba39)[iVar6] = 0;
            (&DAT_00bcba32)[iVar6] = local_5c;
            sVar11 = DAT_00775900;
            *(undefined2 *)(&DAT_00bcba40 + iVar6) = local_44;
            *(short *)(&DAT_00bcba50 + iVar6) = sVar11;
            iVar6 = *local_60;
            sVar11 = sVar10;
            if (0 < iVar6) {
              do {
                iVar15 = FUN_004b4800(0,param_1,param_2,param_3,0);
                iVar14 = iVar15 * 0x184;
                (&DAT_00bcd605)[iVar14] = 0;
                (&DAT_00bcd630)[iVar14] = 9;
                *(short *)(&DAT_00bcd62e + iVar14) = ((short)iVar7 + 1 + sVar10 * 0x14) * 10;
                (&DAT_00bcd658)[iVar15 * 0xc2] = (short)iVar8;
                FUN_004b5af0(iVar15);
                iVar7 = iVar7 + 1;
                (&DAT_00bcd65d)[iVar14] = 1;
                sVar11 = sVar12;
              } while (iVar7 < iVar6);
            }
            sVar12 = sVar11 + 1;
            *(short *)(&DAT_00bcd62e + iVar4) = ((short)iVar7 + 1 + sVar12 * 0x14) * 10;
            sVar10 = sVar12;
          }
          local_60 = local_60 + 1;
          local_54 = local_54 + -1;
        } while (local_54 != 0);
      }
    }
    local_50 = local_50 + 1;
    if (2 < local_50) {
      (&DAT_00bcd728)[iVar4] = 0;
      (&DAT_00bcd630)[iVar4] = 8;
      return;
    }
  } while( true );
}
