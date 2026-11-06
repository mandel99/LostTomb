#include "restore_types.h"
#define DAT_005d19b0 (*(undefined4 *)0x005d19b0u)
#define DAT_00709a58 (*(undefined *)0x00709a58u)
#define DAT_00709a90 (*(undefined *)0x00709a90u)
#define DAT_00709ac8 (*(undefined *)0x00709ac8u)
#define DAT_00709cc0 (*(undefined *)0x00709cc0u)
#define DAT_00709cf8 (*(undefined *)0x00709cf8u)
#define DAT_00709d30 (*(undefined *)0x00709d30u)
#define DAT_0070a698 (*(undefined *)0x0070a698u)
#define DAT_00775900 (*(undefined *)0x00775900u)
#define DAT_00784904 (*(undefined *)0x00784904u)
#define DAT_00784908 (*(undefined *)0x00784908u)
#define DAT_00784bc0 (*(undefined4 *)0x00784bc0u)
#define DAT_00784bd0 (*(undefined4 *)0x00784bd0u)
#define DAT_00784be0 (*(undefined4 *)0x00784be0u)
#define DAT_00784dc4 (*(undefined *)0x00784dc4u)
#define DAT_00785d70 (*(undefined *)0x00785d70u)
#define DAT_00785d74 (*(undefined4 *)0x00785d74u)
#define DAT_00785da6 (*(undefined4 *)0x00785da6u)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(uint32_t *)0x00bca2d8u)
#define DAT_00bcba0c (*(undefined4 *)0x00bcba0cu)
#define DAT_00bcba32 (*(undefined4 *)0x00bcba32u)
#define DAT_00bcba39 (*(undefined4 *)0x00bcba39u)
#define DAT_00bcba40 (*(undefined4 *)0x00bcba40u)
#define DAT_00bcba50 (*(undefined4 *)0x00bcba50u)
#define DAT_00bcd605 (*(undefined4 *)0x00bcd605u)
#define DAT_00bcd628 (*(undefined4 *)0x00bcd628u)
#define DAT_00bcd62a (*(undefined4 *)0x00bcd62au)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd658 (*(undefined4 *)0x00bcd658u)
#define DAT_00bcd65d (*(undefined4 *)0x00bcd65du)
#define DAT_00bcd66e (*(undefined4 *)0x00bcd66eu)
#define DAT_00bcd678 (*(undefined4 *)0x00bcd678u)
#define DAT_00bcd728 (*(undefined4 *)0x00bcd728u)
#define DAT_00bcd764 (*(undefined4 *)0x00bcd764u)
#define DAT_00bcd768 (*(undefined4 *)0x00bcd768u)
#define DAT_00bcd76c (*(undefined4 *)0x00bcd76cu)
#define DAT_00bcd770 (*(undefined4 *)0x00bcd770u)
#define DAT_00bcd774 (*(undefined4 *)0x00bcd774u)
#define DAT_00bcd778 (*(undefined4 *)0x00bcd778u)
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define DAT_00e39080 (*(undefined *)0x00e39080u)
#define _DAT_00784c64 (*(undefined4 *)0x00784c64u)
#define _DAT_00784c66 (*(undefined4 *)0x00784c66u)
#define _DAT_00784c68 (*(undefined4 *)0x00784c68u)
#define _DAT_00784c6a (*(undefined4 *)0x00784c6au)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
extern void FUN_00446650();
extern uint FUN_004467a0();
extern void FUN_004620c0();
extern void FUN_00469140();
extern int FUN_004b4800();
extern void FUN_004b5af0();
extern int FUN_004b6ff0();
extern int FUN_004cde40();

int FUN_00446a30(int param_1,int param_2,int param_3,undefined4 param_4,int param_5)

{
  short sVar1;
  char cVar2;
  undefined2 *puVar3;
  int iVar4;
  uint uVar5;
  int *piVar6;
  int iVar7;
  short *psVar8;
  uint uVar9;
  undefined4 uVar10;
  short sVar11;
  int iVar12;
  int iVar13;
  uint uVar14;
  uint uVar15;
  int iVar16;
  int *piVar17;
  undefined4 uVar18;
  undefined4 uVar19;
  int local_6c;
  int local_68;
  int *local_64;
  int local_60;
  int *local_5c;
  int *local_58;
  int local_54;
  uint local_50;
  int local_4c;
  int local_48 [10];
  int local_20;
  int local_1c;
  int local_18;
  int local_10;
  int local_c;
  int local_8;
  
  piVar17 = (int *)0x0;
  local_60 = 0;
  local_5c = (int *)0x0;
  if ((param_5 < 0) || (DAT_00785d70 <= param_5)) {
    local_58 = (int *)0x0;
  }
  else {
    local_58 = (int *)(&DAT_00785d74 + param_5 * 0x3e);
    if (local_58 != (int *)0x0) {
      (&DAT_00785da6)[param_5 * 0x7c] = 1;
    }
  }
  if (((param_3 < 9) || (0x10 < param_3)) || (param_1 == 0xd)) {
    iVar7 = FUN_004cde40(param_2,*(undefined4 *)(&DAT_005d19b0 + DAT_00e39080 * 4));
    if (0x95 < iVar7) {
      iVar7 = 0x96;
    }
    DAT_00775900 = DAT_00775900 + 1;
    if (32000 < DAT_00775900) {
      DAT_00775900 = 1;
    }
    uVar19 = DAT_00709cc0;
    uVar10 = DAT_00709d30;
    uVar18 = DAT_00709cf8;
    if ((param_1 != 0xc) &&
       (uVar19 = DAT_00709a90, uVar10 = DAT_00709ac8, uVar18 = DAT_00709a58, param_1 == 0xd)) {
      uVar19 = DAT_0070a698;
      uVar10 = 0;
      uVar18 = 0;
    }
    FUN_004cde40(iVar7,uVar19);
    iVar12 = FUN_004cde40(iVar7,uVar18);
    iVar4 = FUN_004cde40(iVar7,uVar10);
    local_48[0] = 0;
    iVar13 = (iVar7 - iVar4) - iVar12;
    local_48[1] = 0;
    piVar17 = local_48 + 6;
    for (iVar7 = 0xc; iVar7 != 0; iVar7 = iVar7 + -1) {
      *piVar17 = 0;
      piVar17 = piVar17 + 1;
    }
    local_48[2] = 0;
    if (0 < iVar13) {
      if (iVar13 < 0x11) {
        local_48[0] = 1;
      }
      else if (iVar13 < 0x21) {
        local_48[0] = 2;
        local_48[7] = iVar13 / 2;
        iVar13 = iVar13 - iVar13 / 2;
      }
      else {
        local_48[0] = 3;
        local_48[8] = iVar13 / 3;
        local_48[7] = local_48[8];
        iVar13 = iVar13 + local_48[8] * -2;
      }
      local_48[6] = iVar13;
    }
    if (0 < iVar12) {
      if (iVar12 < 0x11) {
        local_48[1] = 1;
      }
      else if (iVar12 < 0x21) {
        local_48[1] = 2;
        local_1c = iVar12 / 2;
        iVar12 = iVar12 - iVar12 / 2;
      }
      else {
        local_48[1] = 3;
        local_18 = iVar12 / 3;
        local_1c = local_18;
        iVar12 = iVar12 + local_18 * -2;
      }
      local_20 = iVar12;
    }
    if (0 < iVar4) {
      if (iVar4 < 0x11) {
        local_48[2] = 1;
      }
      else if (iVar4 < 0x21) {
        local_48[2] = 2;
        local_c = iVar4 / 2;
        iVar4 = iVar4 - iVar4 / 2;
      }
      else {
        local_48[2] = 3;
        local_8 = iVar4 / 3;
        local_c = local_8;
        iVar4 = iVar4 + local_8 * -2;
      }
      local_10 = iVar4;
    }
    local_54 = -1;
    sVar11 = _DAT_00784c66;
    sVar1 = _DAT_00784c64;
    if (param_1 != 0xb) {
      iVar7 = 0;
      psVar8 = &DAT_00784bc0;
      do {
        if (*psVar8 != -1) {
          iVar7 = iVar7 + 1;
        }
        psVar8 = psVar8 + 1;
      } while ((int)psVar8 < 0x784bd0);
      if (param_3 == 8) {
        if (iVar7 < 3) {
          uVar5 = DAT_00d3b374 & 1;
        }
        else if (iVar7 < 5) {
          uVar5 = DAT_00d3b374 & 3;
        }
        else {
          uVar5 = DAT_00d3b374 & 7;
        }
      }
      else {
        uVar5 = param_3 - 1;
      }
      iVar7 = 0;
      do {
        if ((&DAT_00784bc0)[uVar5] != -1) break;
        uVar5 = uVar5 + 1;
        if (7 < (int)uVar5) {
          uVar5 = 0;
        }
        iVar7 = iVar7 + 1;
      } while (iVar7 < 8);
      sVar11 = (&DAT_00784be0)[uVar5];
      sVar1 = (&DAT_00784bc0)[uVar5];
    }
    local_6c = (int)sVar1;
    piVar17 = (int *)(int)sVar11;
    if ((local_6c == -1) || (piVar17 == (int *)0xffffffff)) {
      local_6c = (int)_DAT_00784c68;
      piVar17 = (int *)(int)_DAT_00784c6a;
    }
    if (piVar17 == (int *)0x0) {
      cVar2 = '\x04';
    }
    else if ((int)piVar17 < DAT_00784908 + -1) {
      if (local_6c == 0) {
        cVar2 = '\x02';
      }
      else {
        cVar2 = ((local_6c < DAT_00784904 + -1) - 1U & 2) + 4;
      }
    }
    else {
      cVar2 = '\0';
    }
    iVar7 = DAT_00bca2d8 + (int)piVar17 * 0xe4 + local_6c;
    uVar5 = (&DAT_00af598c)[iVar7];
    if ((uVar5 & 0x203) == 0) {
      local_64 = piVar17;
      if ((uVar5 & 4) == 0) {
        if ((uVar5 & 0xc108) != 0) {
          local_54 = iVar7;
          FUN_00469140(local_6c,piVar17,iVar7);
          iVar7 = local_54;
        }
      }
      else if ((uVar5 & 0x40) == 0) {
        return local_54;
      }
      local_54 = iVar7;
      local_60 = 0;
      local_5c = (int *)0x0;
      do {
        local_50 = local_48[(int)local_5c];
        if (0 < (int)local_50) {
          if (param_1 == 0xc) {
            if (local_5c == (int *)0x1) {
              param_2 = 0x37;
            }
            else if (local_5c == (int *)0x2) {
              param_2 = 0x38;
            }
            else {
              param_2 = 0x36;
            }
LAB_004472cd: ;
            local_68 = 10;
          }
          else {
            if (param_1 != 0xd) {
              if (local_5c == (int *)0x1) {
                param_2 = 0x2b;
              }
              else if (local_5c == (int *)0x2) {
                param_2 = 0x2d;
              }
              else {
                param_2 = 0x2c;
              }
              goto LAB_004472cd;
            }
            param_2 = 99;
            local_68 = 8;
          }
          if (0 < (int)local_50) {
            local_58 = local_48 + (int)local_5c * 4 + 6;
            do {
              iVar7 = FUN_004b6ff0(param_2,local_68,cVar2,local_6c,piVar17);
              if (0 < iVar7) {
                iVar4 = 0;
                iVar12 = iVar7 * 0x90;
                *(undefined2 *)(&DAT_00bcba0c + iVar12) = (undefined2)param_4;
                (&DAT_00bcba39)[iVar12] = cVar2;
                (&DAT_00bcba32)[iVar12] = (undefined1)param_1;
                sVar11 = DAT_00775900;
                *(undefined2 *)(&DAT_00bcba40 + iVar12) = (undefined2)param_5;
                *(short *)(&DAT_00bcba50 + iVar12) = sVar11;
                iVar12 = *local_58;
                if (0 < iVar12) {
                  local_4c = local_60 * 5;
                  do {
                    iVar13 = FUN_004b4800(0,param_2,local_6c,local_64,cVar2);
                    iVar16 = iVar13 * 0x184;
                    (&DAT_00bcd605)[iVar16] = 0;
                    (&DAT_00bcd630)[iVar16] = 9;
                    *(short *)(&DAT_00bcd62e + iVar16) =
                         ((short)iVar4 + 1 + (short)local_4c * 4) * 10;
                    (&DAT_00bcd658)[iVar13 * 0xc2] = (short)iVar7;
                    FUN_004b5af0(iVar13);
                    iVar4 = iVar4 + 1;
                    (&DAT_00bcd65d)[iVar16] = 1;
                  } while (iVar4 < iVar12);
                }
                local_60 = local_60 + 1;
                piVar17 = local_64;
              }
              local_58 = local_58 + 1;
              local_50 = local_50 + -1;
            } while (local_50 != 0);
          }
        }
        local_5c = (int *)((int)local_5c + 1);
      } while ((int)local_5c < 3);
    }
  }
  else {
    if (0x95 < param_2) {
      param_2 = 0x96;
    }
    DAT_00775900 = DAT_00775900 + 1;
    if (32000 < DAT_00775900) {
      DAT_00775900 = 1;
    }
    puVar3 = &DAT_00784bd0;
    do {
      puVar3 = puVar3 + 1;
    } while ((int)puVar3 < 0x784be0);
    piVar6 = &DAT_00784dc4;
    do {
      if (*piVar6 != -1) {
        local_48[(int)piVar17] = *piVar6;
        local_48[(int)piVar17 + 3] = piVar6[3];
        piVar17 = (int *)((int)piVar17 + 1);
      }
      piVar6 = piVar6 + 1;
    } while ((int)piVar6 < 0x784dd0);
    param_3 = param_3 + -1;
    iVar7 = 8;
    do {
      if ((&DAT_00784bc0)[param_3] != -1) break;
      param_3 = param_3 + 1;
      if (0xf < param_3) {
        param_3 = 8;
      }
      iVar7 = iVar7 + 1;
    } while (iVar7 < 0x10);
    iVar7 = (int)(short)(&DAT_00784bc0)[param_3];
    piVar6 = (int *)(int)(short)(&DAT_00784be0)[param_3];
    if ((iVar7 == -1) || (piVar6 == (int *)0xffffffff)) {
      iVar7 = (int)_DAT_00784c68;
      piVar6 = (int *)(int)_DAT_00784c6a;
    }
    if (piVar6 == (int *)0x0) {
      cVar2 = '\x04';
    }
    else if ((int)piVar6 < DAT_00784908 + -1) {
      if (iVar7 == 0) {
        cVar2 = '\x02';
      }
      else {
        cVar2 = ((iVar7 < DAT_00784904 + -1) - 1U & 2) + 4;
      }
    }
    else {
      cVar2 = '\0';
    }
    local_54 = DAT_00bca2d8 + (int)piVar6 * 0xe4 + iVar7;
    if (param_5 == 0x8c) {
      local_50 = (int)(param_2 + (param_2 >> 0x1f & 3U)) >> 2;
    }
    else {
      local_50 = (uint)*(byte *)((int)local_58 + 0x36);
    }
    iVar12 = 0;
    local_64 = piVar6;
    local_5c = piVar17;
    if (0 < (int)local_50) {
      do {
        if (param_1 == 0xc) {
          iVar4 = FUN_004b4800(0,100,iVar7,piVar6,cVar2);
        }
        else {
          local_68 = iVar7;
          local_58 = piVar6;
          FUN_004467a0(&local_68,&local_58);
          iVar4 = FUN_004b4800(0,0x5d,local_68,local_58,cVar2);
        }
        iVar13 = iVar4 * 0x184;
        (&DAT_00bcd605)[iVar13] = 0;
        (&DAT_00bcd630)[iVar13] = 8;
        *(short *)(&DAT_00bcd62e + iVar13) = (short)iVar12 * 0x19 + 10;
        FUN_004b5af0(iVar4);
        (&DAT_00bcd65d)[iVar13] = 1;
        local_60 = local_60 + 1;
        (&DAT_00bcd66e)[iVar13] = 1;
        *(undefined4 *)(&DAT_00bcd778 + iVar13) = 0;
        FUN_00446650(iVar4);
        (&DAT_00bcd678)[iVar13] = (undefined1)DAT_0078d7c0;
        iVar12 = iVar12 + 1;
      } while (iVar12 < (int)local_50);
    }
    iVar12 = 0;
    sVar11 = 0;
    if (0 < (int)local_5c) {
      (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
      DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
      if (99 < DAT_00d3b1c4) {
        DAT_00d3b1c4 = 0;
      }
      iVar12 = 0x1f;
      do {
        uVar5 = DAT_00d3b384 >> 4;
        uVar14 = DAT_00d3b384 & 1;
        uVar15 = DAT_00d3b380 >> 4;
        uVar9 = DAT_00d3b380 & 1;
        DAT_00d3b384 = DAT_00d3b384 >> 1;
        DAT_00d3b380 = DAT_00d3b380 >> 1;
        if ((uVar5 & 1) != uVar14) {
          DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
        }
        if ((uVar15 & 1) != uVar9) {
          DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
        }
        iVar12 = iVar12 + -1;
      } while (iVar12 != 0);
      _DAT_00d3b378 = DAT_00d3b384 & 7;
      _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
      _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
      _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
      DAT_00d3b374 = DAT_00d3b384 & 0x7f;
      _DAT_00d3b36c = DAT_00d3b380 & 7;
      iVar12 = (int)((longlong)(ulonglong)_DAT_00d3b370 % (longlong)(int)local_5c);
    }
    if (param_2 != 0) {
      local_50 = CONCAT22(RESTORE_SLICE2(local_50,2),(short)local_48[iVar12]);
      local_58 = (int *)CONCAT22(RESTORE_SLICE2(local_58,2),(short)local_48[iVar12 + 3]);
      do {
        iVar12 = param_2;
        if (param_2 < 0x10) {
          param_2 = 0;
        }
        else {
          iVar12 = 0x10;
          param_2 = param_2 + -0x10;
        }
        local_5c = local_64;
        local_60 = iVar7;
        FUN_004467a0(&local_60,&local_5c);
        if (param_1 == 0xc) {
          uVar19 = 0x65;
        }
        else {
          uVar19 = 0x5c;
        }
        iVar4 = FUN_004b4800(0,uVar19,local_60,local_5c,cVar2);
        iVar13 = iVar4 * 0x184;
        *(undefined2 *)(&DAT_00bcd628 + iVar13) = (undefined2)local_50;
        *(undefined2 *)(&DAT_00bcd62a + iVar13) = RESTORE_SLICE2(local_58,0);
        (&DAT_00bcd605)[iVar13] = 0;
        (&DAT_00bcd630)[iVar13] = 8;
        *(short *)(&DAT_00bcd62e + iVar13) = sVar11 * 0x28 + 10;
        FUN_004b5af0(iVar4);
        (&DAT_00bcd65d)[iVar13] = 1;
        (&DAT_00bcd66e)[iVar13] = 1;
        *(undefined4 *)(&DAT_00bcd778 + iVar13) = 0;
        FUN_004620c0(iVar4);
        iVar4 = (int)DAT_00775900;
        (&DAT_00bcd728)[iVar13] = 1;
        *(int *)(&DAT_00bcd764 + iVar13) = iVar12;
        *(int *)(&DAT_00bcd768 + iVar13) = iVar4;
        *(int *)(&DAT_00bcd76c + iVar13) = param_1;
        *(undefined4 *)(&DAT_00bcd770 + iVar13) = param_4;
        sVar11 = sVar11 + 1;
        *(int *)(&DAT_00bcd774 + iVar13) = param_5;
      } while (param_2 != 0);
      return local_54;
    }
  }
  return local_54;
}
