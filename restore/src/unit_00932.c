#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_0078d7f0 (*(undefined *)0x0078d7f0u)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_0096e6b0 (*(undefined4 *)0x0096e6b0u)
#define DAT_0096e6b4 (*(undefined4 *)0x0096e6b4u)
#define DAT_00972530 (*(undefined4 *)0x00972530u)
#define DAT_00972534 (*(undefined4 *)0x00972534u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
extern int FUN_00475b50();
extern int FUN_004b28f0();
extern void FUN_00519a30();

int FUN_00467450(int param_1)

{
  char cVar1;
  short sVar2;
  undefined2 uVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  short *psVar12;
  int iVar13;
  
  iVar8 = param_1;
  iVar11 = param_1 * 0x184;
  sVar6 = 0;
  sVar5 = 0;
  sVar4 = 0;
  DAT_0078d7f0 = 0;
  switch((&DAT_00bcd602)[iVar11]) {
  case 0xf: ;
    sVar6 = 0x1f;
  case 0x10: ;
    sVar5 = 0x1e;
  case 0x11: ;
    sVar4 = 0x21;
    break;
  case 0x12: ;
    sVar6 = 0x20;
    break;
default: ;
    goto switchD_00467496_caseD_13;
  case 0x69: ;
    sVar6 = 0xe2;
  }
  iVar13 = 1;
  iVar9 = 0;
  DAT_0096e6b0 = 0;
  if (0 < DAT_0078d790) {
    psVar12 = &DAT_00799af0;
    do {
      if (((((((char)psVar12[-8] == '\x01') && (0 < psVar12[0x18])) &&
            (sVar2 = *psVar12, sVar2 != 0)) &&
           (((sVar2 == sVar6 || (sVar2 == sVar5)) || (sVar2 == sVar4)))) &&
          (iVar7 = FUN_00475b50(iVar13), iVar10 = DAT_00bca2d8, iVar9 = DAT_0096e6b0,
          iVar7 == iVar13)) &&
         (((((cVar1 = (&DAT_00bcd602)[iVar11], cVar1 != '\x0f' && (cVar1 != '\x12')) &&
            (cVar1 != 'i')) || (*(char *)((int)psVar12 + 0x97) == '\0')) &&
          (((cVar1 != '\x10' || ((char)psVar12[0x4c] == '\0')) &&
           ((cVar1 != '\x11' || (*(char *)((int)psVar12 + 0x99) == '\0')))))))) {
        sVar2 = psVar12[0xc];
        (&DAT_00972534)[DAT_0096e6b0] = iVar13;
        DAT_0096e6b0 = DAT_0096e6b0 + 1;
        (&DAT_0096e6b0)[DAT_0096e6b0] = iVar10 + psVar12[0xb] + sVar2 * 0xe4;
        iVar9 = DAT_0096e6b0;
      }
      iVar13 = iVar13 + 1;
      psVar12 = psVar12 + 0x84;
    } while (iVar13 <= DAT_0078d790);
    if (iVar9 != 0) {
      if (iVar9 < 1) {
        return 0;
      }
      iVar8 = FUN_004b28f0(param_1,&DAT_0096e6b4,iVar9);
      if (iVar8 < 1) {
        return 0;
      }
      return (&DAT_00972530)[iVar8];
    }
  }
  iVar9 = param_1 * 0xc2;
  iVar13 = param_1 * 0xc2;
  param_1 = 1000;
  iVar10 = 0;
  FUN_00519a30((int)(short)(&DAT_00bcd60c)[iVar13],(int)(short)(&DAT_00bcd60e)[iVar9]);
  iVar9 = 1;
  if (0 < DAT_0078d790) {
    psVar12 = &DAT_00799af0;
    do {
      if (((((char)psVar12[-8] == '\x01') && (0 < psVar12[0x18])) && (sVar2 = *psVar12, sVar2 != 0))
         && ((((sVar2 == sVar6 || (sVar2 == sVar5)) || (sVar2 == sVar4)) &&
             (iVar13 = FUN_00475b50(iVar9), iVar13 == iVar9)))) {
        iVar13 = (&DAT_00eed760)[DAT_00bca2d8 + psVar12[0xb] + psVar12[0xc] * 0xe4];
        cVar1 = (&DAT_00bcd602)[iVar11];
        if ((cVar1 == '\x0f') && (*(byte *)((int)psVar12 + 0x97) != 0)) {
          iVar13 = iVar13 + (uint)*(byte *)((int)psVar12 + 0x97);
        }
        if ((cVar1 == '\x10') && (*(byte *)(psVar12 + 0x4c) != 0)) {
          iVar13 = iVar13 + (uint)*(byte *)(psVar12 + 0x4c);
        }
        if ((cVar1 == '\x11') && (*(byte *)((int)psVar12 + 0x99) != 0)) {
          iVar13 = iVar13 + (uint)*(byte *)((int)psVar12 + 0x99);
        }
        if (iVar13 < param_1) {
          iVar10 = iVar9;
          param_1 = iVar13;
        }
      }
      iVar9 = iVar9 + 1;
      psVar12 = psVar12 + 0x84;
    } while (iVar9 <= DAT_0078d790);
    if (param_1 < 1000) {
      (&DAT_00bcd656)[iVar8 * 0xc2] = (short)iVar10;
      uVar3 = (&DAT_00799a00)[iVar10 * 0x84];
      *(undefined2 *)(&DAT_00bcd61c + iVar11) = (&DAT_007999fe)[iVar10 * 0x84];
      *(undefined2 *)(&DAT_00bcd61e + iVar11) = uVar3;
      return iVar10;
    }
  }
switchD_00467496_caseD_13: ;
  return 0;
}
