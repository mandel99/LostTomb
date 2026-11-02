#include "restore_types.h"
#define DAT_005e2ae4 (*(undefined4 *)0x005e2ae4u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd63c (*(undefined4 *)0x00bcd63cu)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)
#define DAT_00bcd66c (*(undefined4 *)0x00bcd66cu)
#define DAT_00bcd672 (*(undefined4 *)0x00bcd672u)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
extern int FUN_0045bb50();
extern void FUN_004dfbb0();
extern uint FUN_004e0530();

uint FUN_004e05f0(int param_1,int param_2,char param_3)

{
  char cVar1;
  undefined2 uVar2;
  uint uVar3;
  undefined4 uVar4;
  byte bVar5;
  undefined2 uVar6;
  int iVar7;
  uint3 uVar10;
  uint uVar8;
  undefined3 uVar11;
  undefined4 uVar9;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  int iVar15;
  int iVar16;
  bool bVar17;
  bool bVar18;
  bool bVar19;
  bool bVar20;
  uint local_4;
  
  iVar15 = param_2 * 0x184;
  cVar1 = (&DAT_00bcd602)[iVar15];
  bVar17 = cVar1 == 'U';
  bVar18 = cVar1 == 'O';
  bVar19 = cVar1 == 'Q';
  iVar16 = param_1 * 0x108;
  uVar3 = *(uint *)(&DAT_00799a3c + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84);
  uVar13 = uVar3 >> 1;
  local_4 = uVar3 & 0xf0;
  bVar20 = (char)local_4 == -0x10;
  if (((&DAT_007999ea)[param_1 * 0x84] == 0) && ((uVar13 & 1) != 0)) {
    local_4 = 0;
    do {
      iVar7 = (local_4 - (int)(short)(&DAT_007999ea)[param_1 * 0x84]) + param_1;
      if ((int)(&DAT_00799a30)[iVar7 * 0x42] < 0xffff) {
        (&DAT_00799a30)[iVar7 * 0x42] = 0xffff;
        FUN_004dfbb0(iVar7);
      }
      local_4 = local_4 + 1;
    } while ((int)local_4 < 4);
  }
  uVar10 = (uint3)(local_4 >> 8);
  if ((bVar18) &&
     ((uVar8 = CONCAT31(uVar10,bVar20), bVar20 || (uVar8 = (uint)uVar10 << 8, (uVar3 & 1) == 0))))
  goto switchD_004e0727_caseD_0;
  uVar12 = (&DAT_005e2ae4)[(short)(&DAT_007999ea)[param_1 * 0x84] * 0xd];
  uVar8 = uVar12 & 0xff;
  uVar14 = uVar12 & 0xf00;
  switch((int)(uVar8)) {
  case 0: ;
  case 1: ;
  case 2: ;
  case 3: ;
  case 0xb: ;
  case 0xd: ;
    goto switchD_004e0727_caseD_0;
  case 4: ;
  case 5: ;
  case 6: ;
  case 7: ;
  case 8: ;
  case 9: ;
  case 10: ;
  case 0xc: ;
  }
  if (((uVar14 != 0) && (0 < (int)(&DAT_00799a30)[param_1 * 0x42])) && ((uVar13 & 1) == 0)) {
    if (uVar14 < 0x401) {
      if (uVar14 == 0x400) {
        if ((uVar3 & 0x40) == 0) {
          if ((bVar19) && (uVar8 = 0, (uVar3 & 1) != 0)) goto switchD_004e0727_caseD_0;
        }
        else if (bVar18) {
          return 0;
        }
        iVar7 = (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) + 2;
      }
      else if (uVar14 == 0x100) {
        if ((uVar3 & 0x10) == 0) {
          if ((bVar19) && (uVar8 = 0, (uVar3 & 1) != 0)) goto switchD_004e0727_caseD_0;
        }
        else if (bVar18) {
          return 0;
        }
        iVar7 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
      }
      else {
        if (uVar14 != 0x200) goto switchD_004e0727_caseD_0;
        if ((uVar3 & 0x20) == 0) {
          if ((bVar19) && (uVar8 = 0, (uVar3 & 1) != 0)) goto switchD_004e0727_caseD_0;
        }
        else {
          uVar8 = 0;
          if (bVar18) goto switchD_004e0727_caseD_0;
        }
        iVar7 = (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) + 1;
      }
    }
    else {
      if (uVar14 != 0x800) goto switchD_004e0727_caseD_0;
      if ((uVar3 & 0x80) == 0) {
        if ((bVar19) && (uVar8 = 0, (uVar3 & 1) != 0)) goto switchD_004e0727_caseD_0;
      }
      else if (bVar18) {
        return 0;
      }
      iVar7 = (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) + 3;
    }
    if (!bVar17) {
      if (!bVar19) {
        uVar8 = CONCAT31((int3)((uint)iVar7 >> 8),bVar18);
        if ((bVar18) && (uVar8 = FUN_004e0530(param_1), (char)uVar8 == '\0')) {
          uVar9 = CONCAT31((int3)(uVar8 >> 8),param_3);
          if (param_3 != '\0') {
            (&DAT_00bcd630)[iVar15] = 9;
            *(int *)(&DAT_00799a34 + iVar16) = param_2;
            uVar2 = (&DAT_00799a00)[param_1 * 0x84];
            uVar9 = *(undefined4 *)(&DAT_007999e4 + iVar16);
            *(undefined2 *)(&DAT_00bcd61c + iVar15) = (&DAT_007999fe)[param_1 * 0x84];
            *(undefined2 *)(&DAT_00bcd61e + iVar15) = uVar2;
            *(undefined4 *)(&DAT_00bcd620 + iVar15) = uVar9;
            *(undefined2 *)(&DAT_00bcd63c + iVar15) = 0;
          }
          return CONCAT31((int3)((uint)uVar9 >> 8),1);
        }
        goto switchD_004e0727_caseD_0;
      }
      uVar12 = uVar12 & 0xf000;
      uVar8 = (&DAT_00799a30)[iVar7 * 0x42];
      bVar5 = (byte)uVar8;
      if (uVar12 < 0x4001) {
        if (uVar12 == 0x4000) {
          if ((bVar5 & 0x30) == 0x30) {
            uVar8 = uVar8 & 0x3000;
            if (uVar8 == 0x3000) goto switchD_004e0727_caseD_0;
LAB_004e0a5b: ;
            if ((uVar3 & 1) == 0) goto switchD_004e0727_caseD_0;
          }
        }
        else if (uVar12 == 0x1000) {
          if ((bVar5 & 3) == 3) {
            uVar8 = uVar8 & 0x300;
            if (uVar8 == 0x300) goto switchD_004e0727_caseD_0;
            goto LAB_004e0a5b;
          }
        }
        else {
          if (uVar12 != 0x2000) goto switchD_004e0727_caseD_0;
          if ((bVar5 & 0xc) == 0xc) {
            uVar8 = uVar8 & 0xc00;
            if (uVar8 == 0xc00) goto switchD_004e0727_caseD_0;
            goto LAB_004e0a5b;
          }
        }
      }
      else {
        if (uVar12 != 0x8000) goto switchD_004e0727_caseD_0;
        if ((bVar5 & 0xc0) == 0xc0) {
          uVar8 = uVar8 & 0xc000;
          if (uVar8 == 0xc000) goto switchD_004e0727_caseD_0;
          goto LAB_004e0a5b;
        }
      }
      uVar8 = FUN_004e0530(param_1);
      if ((char)uVar8 == '\0') {
        uVar9 = CONCAT31((int3)(uVar8 >> 8),param_3);
        if (param_3 != '\0') {
          (&DAT_00bcd630)[iVar15] = 9;
          *(int *)(&DAT_00799a34 + iVar16) = param_2;
          uVar2 = (&DAT_00799a00)[param_1 * 0x84];
          uVar9 = CONCAT22((short)(uVar8 >> 0x10),uVar2);
          uVar4 = *(undefined4 *)(&DAT_007999e4 + iVar16);
          *(undefined2 *)(&DAT_00bcd61c + iVar15) = (&DAT_007999fe)[param_1 * 0x84];
          *(undefined2 *)(&DAT_00bcd61e + iVar15) = uVar2;
          *(undefined4 *)(&DAT_00bcd620 + iVar15) = uVar4;
          *(undefined2 *)(&DAT_00bcd63c + iVar15) = 0;
        }
        return CONCAT31((int3)((uint)uVar9 >> 8),1);
      }
      goto switchD_004e0727_caseD_0;
    }
    uVar8 = FUN_004e0530(param_1);
    if ((char)uVar8 == '\0') goto switchD_004e0727_caseD_0;
    uVar12 = *(uint *)(&DAT_00799a34 + iVar16);
    iVar7 = uVar12 * 0x184;
    if (((&DAT_00bcd602)[iVar7] != 'Q') || ((&DAT_00bcd630)[iVar7] != '\n'))
    goto switchD_004e0727_caseD_0;
    if (*(short *)(&DAT_00bcd660 + iVar7) == 0) {
LAB_004e0915: ;
      uVar11 = (undefined3)(uVar8 >> 8);
      if (param_3 != '\0') {
        iVar16 = FUN_0045bb50(param_1,0x1e,1);
        uVar2 = DAT_00d3b4e4;
        if (iVar16 != 0) {
          (&DAT_00bcd672)[iVar15] = 0x1e;
          *(undefined2 *)(&DAT_00bcd66c + iVar15) = 0;
          (&DAT_00bcd656)[param_2 * 0xc2] = (short)iVar16;
          uVar6 = DAT_00d3b4e0;
          uVar9 = CONCAT22((short)((uint)iVar16 >> 0x10),DAT_00d3b4e0);
          *(short *)(&DAT_00bcd660 + iVar15) = (short)uVar12;
          (&DAT_00bcd630)[iVar15] = 0xf;
          *(undefined2 *)(&DAT_00bcd62e + iVar15) = 0;
          *(undefined2 *)(&DAT_00bcd61c + iVar15) = uVar2;
          *(undefined2 *)(&DAT_00bcd61e + iVar15) = uVar6;
          *(undefined2 *)(&DAT_00bcd660 + iVar7) = (undefined2)param_2;
          return CONCAT31((int3)((uint)uVar9 >> 8),1);
        }
        (&DAT_00bcd606)[iVar15] = 2;
        uVar11 = 0;
      }
      return CONCAT31(uVar11,1);
    }
    uVar8 = *(short *)(&DAT_00bcd660 + iVar7) * 0x184;
    if (((((&DAT_00bcd606)[uVar8] == '\0') || ((&DAT_00bcd602)[uVar8] != 'U')) ||
        ((cVar1 = (&DAT_00bcd630)[uVar8], cVar1 != '\x06' &&
         ((((cVar1 != '\a' && (cVar1 != '\x0f')) && (cVar1 != '\x10')) && (cVar1 != '\x1a')))))) ||
       (uVar8 = (uint)*(short *)(&DAT_00bcd660 + uVar8), uVar8 != uVar12)) {
      *(undefined2 *)(&DAT_00bcd660 + iVar7) = 0;
      goto LAB_004e0915;
    }
  }
  uVar8 = CONCAT31((int3)(uVar8 >> 8),bVar17);
  if (((bVar17) && (uVar8 = (&DAT_00799a30)[param_1 * 0x42], uVar8 != 2)) &&
     (((uVar8 != 1 || ((uVar8 = 0, (uVar3 >> 2 & 1) != 0 && ((uVar13 & 1) != 0)))) &&
      (uVar8 = FUN_004e0530(param_1), (char)uVar8 == '\0')))) {
    uVar9 = CONCAT31((int3)(uVar8 >> 8),param_3);
    if (param_3 != '\0') {
      (&DAT_00bcd630)[iVar15] = 0x1b;
      *(int *)(&DAT_00799a34 + iVar16) = param_2;
      uVar9 = CONCAT22((short)(uVar8 >> 0x10),(&DAT_007999fe)[param_1 * 0x84]);
      uVar2 = (&DAT_00799a00)[param_1 * 0x84];
      uVar4 = *(undefined4 *)(&DAT_007999e4 + iVar16);
      *(undefined2 *)(&DAT_00bcd61c + iVar15) = (&DAT_007999fe)[param_1 * 0x84];
      *(undefined2 *)(&DAT_00bcd61e + iVar15) = uVar2;
      *(undefined4 *)(&DAT_00bcd620 + iVar15) = uVar4;
      *(undefined2 *)(&DAT_00bcd63c + iVar15) = 0;
    }
    return CONCAT31((int3)((uint)uVar9 >> 8),1);
  }
switchD_004e0727_caseD_0: ;
  return uVar8 & 0xffffff00;
}
