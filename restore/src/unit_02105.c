#include "restore_types.h"
#define DAT_00570084 (*(undefined4 *)0x00570084u)
#define DAT_005700f4 (*(undefined4 *)0x005700f4u)
#define DAT_005700f8 (*(undefined4 *)0x005700f8u)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005e4ef8 (*(undefined4 *)0x005e4ef8u)
#define DAT_005e68f8 (*(undefined4 *)0x005e68f8u)
#define DAT_005f5b68 (*(undefined4 *)0x005f5b68u)
#define DAT_005f5b6c (*(undefined4 *)0x005f5b6cu)
#define DAT_005f5d30 (*(undefined4 *)0x005f5d30u)
#define DAT_0070e44c (*(undefined *)0x0070e44cu)
#define DAT_0070e45c (*(undefined *)0x0070e45cu)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00799a36 (*(undefined4 *)0x00799a36u)
#define DAT_00799a38 (*(undefined4 *)0x00799a38u)
#define DAT_00799a3a (*(undefined4 *)0x00799a3au)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799a44 (*(undefined4 *)0x00799a44u)
#define DAT_00799a48 (*(undefined4 *)0x00799a48u)
#define DAT_00799a4c (*(undefined4 *)0x00799a4cu)
#define DAT_00799a50 (*(undefined4 *)0x00799a50u)
#define DAT_00799a54 (*(undefined4 *)0x00799a54u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bcd600 (*(undefined4 *)0x00bcd600u)
#define DAT_00bcd718 (*(undefined4 *)0x00bcd718u)
#define DAT_00f57334 (*(undefined *)0x00f57334u)
extern void FUN_00438e50();
extern int FUN_0053e590();
extern undefined4 FUN_0053f250();

void FUN_004ef550(undefined4 param_1,int param_2,int param_3,int param_4)

{
  byte bVar1;
  byte bVar2;
  short sVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  bool bVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  char cVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  int *piVar18;
  int local_1c;
  int local_18;
  int local_8;
  
  iVar14 = param_4 * 0x108;
  uVar7 = (uint)*(byte *)(&DAT_00799a2c + param_4 * 0x84);
  bVar1 = *(byte *)((int)&DAT_00799a2e + iVar14 + 1);
  local_8 = 0;
  if (DAT_005e4ef4 == 1) {
    iVar13 = *(int *)(&DAT_005e68f8 + ((uint)bVar1 + uVar7 * 6) * 4);
  }
  else {
    iVar13 = (uint)bVar1 * 0x5a;
  }
  bVar2 = *(byte *)((int)&DAT_00799a2c + iVar14 + 1);
  local_18 = param_2;
  local_1c = param_3;
  if ((bVar2 < 2) &&
     ((((bVar2 != 0 || (bVar1 != 0)) || (*(char *)(&DAT_00799a2e + param_4 * 0x84) != '\0')) ||
      ((DAT_005e4ef4 == 5 || ((short)(&DAT_007999ea)[param_4 * 0x84] < 4)))))) {
    local_18 = DAT_0070e45c + 1 + param_2;
    local_1c = (param_3 - DAT_0070e44c) - iVar13;
  }
  bVar1 = *(byte *)((int)&DAT_00799a30 + iVar14 + 1);
  if (bVar1 < 7) {
    cVar11 = '\0';
  }
  else if ((DAT_005e4ef4 == 1) || (DAT_005e4ef4 != 3)) {
    if ((uint)bVar1 % 6 == 1) goto LAB_004ef670;
    cVar11 = '\0';
  }
  else if ((int)(bVar1 - 1) % 6 < 3) {
LAB_004ef670: ;
    iVar17 = *(int *)(&DAT_005e4ef8 +
                     ((int)(short)(&DAT_007999ea)[param_4 * 0x84] + uVar7 * 0x90) * 8);
    iVar10 = DAT_00bca2e0 / 2;
    if (iVar17 < 4) {
      iVar16 = iVar17 + iVar10;
      if (3 < iVar17 + iVar10) {
LAB_004ef6aa: ;
        iVar16 = iVar17 + iVar10 + -4;
      }
    }
    else {
      iVar16 = iVar17;
      if ((iVar17 < 8) && (iVar16 = iVar17 + iVar10, 7 < iVar16)) goto LAB_004ef6aa;
    }
    if (DAT_005e4ef4 == 3) {
      if (((iVar16 == 0) || (iVar16 == 4)) || (iVar16 == 7)) {
        cVar11 = '\0';
      }
      else {
LAB_004ef6df: ;
        if (iVar16 == 2) {
          iVar17 = *(int *)(&DAT_005700f4 + iVar10 * 0xc);
        }
        else {
          iVar17 = *(int *)(&DAT_005700f8 + iVar10 * 0xc);
        }
        iVar10 = (int)*(short *)(&DAT_00b4e6fc + (iVar17 + *(int *)(&DAT_007999e4 + iVar14)) * 2);
        if (iVar10 == 0) {
          cVar11 = (char)*(short *)(&DAT_00b4e6fc + (iVar17 + *(int *)(&DAT_007999e4 + iVar14)) * 2)
          ;
        }
        else {
          iVar17 = iVar10 * 0x108;
          if ((&DAT_007999e8)[iVar10 * 0x84] == 0xb7) {
            *(int *)(&DAT_00799a44 + iVar10 * 0x84) = param_4;
            *(int *)(&DAT_00799a48 + iVar17) = local_18;
            *(int *)(&DAT_00799a4c + iVar17) = local_1c;
            *(int *)(&DAT_00799a50 + iVar17) = param_2;
            *(int *)(&DAT_00799a54 + iVar17) = param_3;
            cVar11 = '\x01';
          }
          else {
            cVar11 = '\0';
          }
        }
      }
    }
    else {
      if ((iVar16 == 5) || (iVar16 == 6)) goto LAB_004ef6df;
      cVar11 = '\0';
    }
  }
  else {
    cVar11 = '\0';
  }
  iVar17 = *(int *)(&DAT_00799a44 + param_4 * 0x84);
  bVar6 = false;
  if (((cVar11 == '\0') && (*(int *)(&DAT_00799a40 + param_4 * 0x84) == 0)) && (iVar17 == 0)) {
    piVar18 = &DAT_00570084;
    do {
      local_8 = *(int *)(&DAT_007999e4 + iVar14) + *piVar18;
      sVar3 = *(short *)(&DAT_00b350dc + local_8 * 2);
      while (iVar10 = (int)sVar3, 0 < iVar10) {
        if (*(short *)(&DAT_00bcd718 + iVar10 * 0x184) == 3) {
          FUN_00438e50(iVar10,param_2,param_3,0);
        }
        sVar3 = *(short *)(&DAT_00bcd600 + iVar10 * 0x184);
      }
      piVar18 = piVar18 + 1;
    } while ((int)piVar18 < 0x570094);
    bVar6 = true;
  }
  if (iVar13 != 0) {
    if (((DAT_005e4ef4 == 5) && (1 < *(byte *)(&DAT_00799a2c + param_4 * 0x84))) &&
       (((&DAT_005f5d30)[(short)(&DAT_007999ea)[param_4 * 0x84]] != '\0' &&
        (*(char *)((int)&DAT_00799a2e + iVar14 + 1) != '\0')))) {
      iVar10 = DAT_00f57334 + 0x65 + ((byte)(&DAT_00acf85c)[local_8] & 1) * 6;
      FUN_0053e590(iVar10,local_18,local_1c + iVar13);
      FUN_0053f250(iVar10,local_18,local_1c + iVar13);
    }
    FUN_0053e590(param_1,local_18,local_1c);
  }
  FUN_0053f250(param_1,local_18,local_1c);
  if (iVar17 != 0) {
    iVar16 = 3;
    iVar13 = *(int *)(&DAT_00799a48 + iVar14);
    iVar10 = *(int *)(&DAT_00799a4c + iVar14);
    uVar4 = *(undefined4 *)(&DAT_00799a50 + iVar14);
    uVar5 = *(undefined4 *)(&DAT_00799a54 + iVar14);
    if (DAT_005e4ef4 == 3) {
      iVar12 = iVar17 * 0x108;
      if (*(int *)(&DAT_005e4ef8 + ((int)(short)(&DAT_007999ea)[iVar17 * 0x84] + uVar7 * 0x90) * 8)
          < 4) {
        switch((uint)*(byte *)((int)&DAT_00799a30 + iVar12 + 1) % 6) {
default: ;
          iVar15 = 0;
          break;
        case 2: ;
          iVar15 = 1;
          break;
        case 4: ;
          iVar15 = 2;
          break;
        case 5: ;
          iVar15 = 3;
        }
        switch((uint)*(byte *)((int)&DAT_00799a30 + iVar12 + 2) % 6) {
default: ;
          goto switchD_004ef94c_caseD_1;
        case 2: ;
          iVar16 = 1;
          break;
        case 4: ;
          iVar16 = 2;
          break;
        case 5: ;
          break;
        }
      }
      else {
        iVar15 = (int)(*(byte *)((int)&DAT_00799a30 + iVar12 + 1) - 1) % 3;
        iVar16 = (int)(*(byte *)((int)&DAT_00799a30 + iVar12 + 2) - 1) % 3;
      }
    }
    else {
      iVar15 = 0;
switchD_004ef94c_caseD_1: ;
      iVar16 = 0;
    }
    iVar12 = iVar17 * 0x108;
    if (*(int *)(&DAT_00799a40 + iVar17 * 0x84) != 0) {
      if (*(char *)((int)&DAT_00799a30 + iVar12 + 1) != '\0') {
        iVar15 = *(int *)(&DAT_00799a40 + iVar17 * 0x84) + iVar15;
        if (DAT_005e4ef4 == 3) {
          iVar8 = (iVar15 - DAT_00f57334) + -0x6d;
          iVar9 = *(int *)(&DAT_005f5b68 + iVar8 * 8);
          iVar8 = *(int *)(&DAT_005f5b6c + iVar8 * 8);
        }
        else {
          iVar8 = 0;
          iVar9 = 0;
        }
        FUN_0053e590(iVar15,*(short *)(&DAT_00799a34 + iVar12) + iVar9 + iVar13,
                     *(short *)(&DAT_00799a36 + iVar12) + iVar8 + iVar10);
      }
      if (*(char *)((int)&DAT_00799a30 + iVar12 + 2) != '\0') {
        if (DAT_005e4ef4 == 3) {
          iVar9 = ((iVar16 + *(int *)(&DAT_00799a40 + iVar17 * 0x84)) - DAT_00f57334) + -0x6d;
          iVar15 = *(int *)(&DAT_005f5b68 + iVar9 * 8);
          iVar9 = *(int *)(&DAT_005f5b6c + iVar9 * 8);
        }
        else {
          iVar9 = 0;
          iVar15 = 0;
        }
        FUN_0053e590(iVar16 + *(int *)(&DAT_00799a40 + iVar17 * 0x84),
                     *(short *)(&DAT_00799a38 + iVar12) + iVar15 + iVar13,
                     *(short *)(&DAT_00799a3a + iVar12) + iVar9 + iVar10);
      }
    }
    piVar18 = &DAT_00570084;
    do {
      sVar3 = *(short *)(&DAT_00b350dc + (*piVar18 + *(int *)(&DAT_007999e4 + iVar12)) * 2);
      while (iVar13 = (int)sVar3, 0 < iVar13) {
        if (*(short *)(&DAT_00bcd718 + iVar13 * 0x184) != 0) {
          FUN_00438e50(iVar13,uVar4,uVar5,0);
        }
        sVar3 = *(short *)(&DAT_00bcd600 + iVar13 * 0x184);
      }
      piVar18 = piVar18 + 1;
    } while ((int)piVar18 < 0x570094);
  }
  if (cVar11 != '\0') {
    return;
  }
  if (*(int *)(&DAT_00799a40 + param_4 * 0x84) == 0) goto LAB_004efc66;
  if (DAT_005e4ef4 == 3) {
    if (*(int *)(&DAT_005e4ef8 + ((int)(short)(&DAT_007999ea)[param_4 * 0x84] + uVar7 * 0x90) * 8) <
        4) {
      switch((uint)*(byte *)((int)&DAT_00799a30 + iVar14 + 1) % 6) {
default: ;
        iVar13 = 0;
        break;
      case 2: ;
        iVar13 = 1;
        break;
      case 4: ;
        iVar13 = 2;
        break;
      case 5: ;
        iVar13 = 3;
      }
      switch((uint)*(byte *)((int)&DAT_00799a30 + iVar14 + 2) % 6) {
default: ;
        goto switchD_004efb58_caseD_1;
      case 2: ;
        iVar17 = 1;
        break;
      case 4: ;
        iVar17 = 2;
        break;
      case 5: ;
        iVar17 = 3;
      }
    }
    else {
      iVar13 = (int)(*(byte *)((int)&DAT_00799a30 + iVar14 + 1) - 1) % 3;
      iVar17 = (int)(*(byte *)((int)&DAT_00799a30 + iVar14 + 2) - 1) % 3;
    }
  }
  else {
    iVar13 = 0;
switchD_004efb58_caseD_1: ;
    iVar17 = 0;
  }
  if (*(char *)((int)&DAT_00799a30 + iVar14 + 1) != '\0') {
    iVar13 = iVar13 + *(int *)(&DAT_00799a40 + param_4 * 0x84);
    if (DAT_005e4ef4 == 3) {
      iVar16 = (iVar13 - DAT_00f57334) + -0x6d;
      iVar10 = *(int *)(&DAT_005f5b68 + iVar16 * 8);
      iVar16 = *(int *)(&DAT_005f5b6c + iVar16 * 8);
    }
    else {
      iVar16 = 0;
      iVar10 = 0;
    }
    FUN_0053e590(iVar13,*(short *)(&DAT_00799a34 + iVar14) + iVar10 + local_18,
                 *(short *)(&DAT_00799a36 + iVar14) + iVar16 + local_1c);
  }
  if (*(char *)((int)&DAT_00799a30 + iVar14 + 2) != '\0') {
    if (DAT_005e4ef4 == 3) {
      iVar10 = ((*(int *)(&DAT_00799a40 + param_4 * 0x84) + iVar17) - DAT_00f57334) + -0x6d;
      iVar13 = *(int *)(&DAT_005f5b68 + iVar10 * 8);
      iVar10 = *(int *)(&DAT_005f5b6c + iVar10 * 8);
    }
    else {
      iVar10 = 0;
      iVar13 = 0;
    }
    FUN_0053e590(*(int *)(&DAT_00799a40 + param_4 * 0x84) + iVar17,
                 *(short *)(&DAT_00799a38 + iVar14) + iVar13 + local_18,
                 *(short *)(&DAT_00799a3a + iVar14) + iVar10 + local_1c);
  }
LAB_004efc66: ;
  if (!bVar6) {
    piVar18 = &DAT_00570084;
    do {
      sVar3 = *(short *)(&DAT_00b350dc + (*(int *)(&DAT_007999e4 + iVar14) + *piVar18) * 2);
      while (iVar13 = (int)sVar3, 0 < iVar13) {
        if (*(short *)(&DAT_00bcd718 + iVar13 * 0x184) == 3) {
          FUN_00438e50(iVar13,param_2,param_3,0);
        }
        sVar3 = *(short *)(&DAT_00bcd600 + iVar13 * 0x184);
      }
      piVar18 = piVar18 + 1;
    } while ((int)piVar18 < 0x570094);
  }
  piVar18 = &DAT_00570084;
  do {
    sVar3 = *(short *)(&DAT_00b350dc + (*piVar18 + *(int *)(&DAT_007999e4 + iVar14)) * 2);
    while (iVar13 = (int)sVar3, 0 < iVar13) {
      if ((0 < *(short *)(&DAT_00bcd718 + iVar13 * 0x184)) &&
         (*(short *)(&DAT_00bcd718 + iVar13 * 0x184) != 3)) {
        FUN_00438e50(iVar13,param_2,param_3,0);
      }
      sVar3 = *(short *)(&DAT_00bcd600 + iVar13 * 0x184);
    }
    piVar18 = piVar18 + 1;
  } while ((int)piVar18 < 0x570094);
  return;
}
