#include "restore_types.h"
#define DAT_007090e4 (*(undefined4 *)0x007090e4u)
#define DAT_007090e8 (*(undefined4 *)0x007090e8u)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60a (*(undefined4 *)0x00bcd60au)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd662 (*(undefined4 *)0x00bcd662u)
#define DAT_00bcd663 (*(undefined4 *)0x00bcd663u)
#define DAT_00bcd678 (*(undefined4 *)0x00bcd678u)
#define DAT_00bcd67a (*(undefined4 *)0x00bcd67au)
#define DAT_00bcd754 (*(undefined4 *)0x00bcd754u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define DAT_00ed4140 (*(undefined4 *)0x00ed4140u)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern undefined4 FUN_00489970();
extern void FUN_004ae2c0();
extern void FUN_004b3250();
extern int FUN_004b45c0();
extern int FUN_004bcf40();
extern int FUN_0051dc10();
extern uint FUN_00562348();

void FUN_004ae650(int param_1,undefined4 *param_2)

{
  char cVar1;
  short sVar2;
  bool bVar3;
  char cVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  
  iVar9 = param_1 * 0x184;
  if (*(int *)(&DAT_00bcd754 + iVar9) != 9) {
    if (*(int *)(&DAT_00bcd754 + iVar9) != 0x10) {
      return;
    }
    iVar5 = FUN_004bcf40(param_1,0xe4);
    if (iVar5 == 0) {
      (&DAT_00bcd630)[iVar9] = 0x11;
      return;
    }
    cVar4 = (&DAT_00ed4140)
            [(short)(&DAT_00bcd60e)[iVar5 * 0xc2] * 0xe4 +
             DAT_00bca2d8 + (short)(&DAT_00bcd60c)[iVar5 * 0xc2]];
    switch((&DAT_00bcd602)[iVar5 * 0x184]) {
    case 0xb: ;
    case 0xc: ;
    case 0xd: ;
    case 0x4d: ;
    case 0x4e: ;
      uVar6 = FUN_00562348();
      if ((uVar6 & 1) != 0) {
        (&DAT_00bcd678)[iVar9] = (char)((DAT_0078d7c0 + 1) % 0xc);
      }
    }
    cVar1 = (&DAT_00bcd602)[iVar5 * 0x184];
    if (((cVar1 == '\f') || (cVar1 == '\v')) || (cVar1 == '\r')) {
      *(undefined2 *)(&DAT_00bcd61c + iVar9) = (&DAT_00bcd60c)[param_1 * 0xc2];
      *(undefined2 *)(&DAT_00bcd61e + iVar9) = (&DAT_00bcd60e)[param_1 * 0xc2];
    }
    else if (cVar4 == -1) {
      sVar2 = (&DAT_00bcd60c)[iVar5 * 0xc2];
      if ((short)(&DAT_00bcd60c)[param_1 * 0xc2] < sVar2) {
        iVar8 = sVar2 + -1;
      }
      else {
        iVar8 = (int)sVar2;
      }
      iVar10 = (int)(short)(&DAT_00bcd60e)[iVar5 * 0xc2];
      if ((short)(&DAT_00bcd60e)[param_1 * 0xc2] < (short)(&DAT_00bcd60e)[iVar5 * 0xc2]) {
        iVar10 = iVar10 + -1;
      }
      cVar4 = FUN_00489970(iVar5,&param_2);
      if (cVar4 == '\0') {
        *(undefined2 *)(&DAT_00bcd61c + iVar9) = (&DAT_00bcd60c)[param_1 * 0xc2];
        *(undefined2 *)(&DAT_00bcd61e + iVar9) = (&DAT_00bcd60e)[param_1 * 0xc2];
      }
      else {
        iVar7 = FUN_0051dc10(param_2,(int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],iVar8,iVar10,0);
        if (iVar7 == 0) {
          *(undefined2 *)(&DAT_00bcd61c + iVar9) = (&DAT_00bcd60c)[param_1 * 0xc2];
          *(undefined2 *)(&DAT_00bcd61e + iVar9) = (&DAT_00bcd60e)[param_1 * 0xc2];
        }
        else {
          *(short *)(&DAT_00bcd61c + iVar9) = (short)iVar8;
          *(short *)(&DAT_00bcd61e + iVar9) = (short)iVar10;
        }
      }
    }
    else {
      *(undefined2 *)(&DAT_00bcd61c + iVar9) = (&DAT_00bcd60c)[iVar5 * 0xc2];
      *(undefined2 *)(&DAT_00bcd61e + iVar9) = (&DAT_00bcd60e)[iVar5 * 0xc2];
      *param_2 = 1;
    }
    *(undefined4 *)(&DAT_00bcd754 + iVar9) = 9;
    (&DAT_00bcd609)[iVar9] = (&DAT_00bcd60a)[iVar9];
    *(short *)(&DAT_00bcd67a + iVar9) = (short)iVar5;
    FUN_004b3250(param_1);
    return;
  }
  iVar10 = (int)*(short *)(&DAT_00bcd67a + iVar9);
  iVar8 = (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2];
  _DAT_00d3b4f8 = (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2];
  iVar5 = (int)(short)(&DAT_00bcd60c)[iVar10 * 0xc2];
  iVar7 = (int)(short)(&DAT_00bcd60e)[iVar10 * 0xc2];
  if (iVar5 < iVar8) {
    DAT_00d3b4fc = iVar8 - iVar5;
  }
  else if (iVar8 < iVar5) {
    DAT_00d3b4fc = iVar5 - iVar8;
  }
  else {
    DAT_00d3b4fc = 0;
  }
  if (iVar7 < _DAT_00d3b4f8) {
    _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar7;
  }
  else if (_DAT_00d3b4f8 < iVar7) {
    _DAT_00d3b4f8 = iVar7 - _DAT_00d3b4f8;
  }
  else {
    _DAT_00d3b4f8 = 0;
  }
  iVar5 = _DAT_00d3b4f8;
  if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
    iVar5 = DAT_00d3b4fc;
  }
  if (iVar5 < *(int *)(&DAT_007090e4 + (uint)(byte)(&DAT_00bcd602)[iVar9] * 0x38)) {
    cVar4 = (&DAT_00bcd663)[iVar9];
    bVar3 = true;
    (&DAT_00bcd663)[iVar9] = cVar4 + 1U;
    if (((int)(uint)(byte)(cVar4 + 1U) <=
         *(int *)(&DAT_007090e8 + (uint)(byte)(&DAT_00bcd602)[iVar9] * 0x38)) ||
       ((&DAT_00bcd663)[iVar9] = 0, iVar10 == 0)) goto LAB_004ae950;
    (&DAT_00bcd662)[iVar9] = 1;
  }
  else {
    bVar3 = false;
LAB_004ae950: ;
    (&DAT_00bcd662)[iVar9] = 0;
  }
  if ((&DAT_00bcd662)[iVar9] != '\0') {
    if ((&DAT_00bcd662)[iVar9] == '\x01') {
      FUN_004b45c0(param_1,(int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                   (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],
                   (int)(short)(&DAT_00bcd60c)[iVar10 * 0xc2],
                   (int)(short)(&DAT_00bcd60e)[iVar10 * 0xc2],0x47);
    }
    cVar4 = (&DAT_00bcd662)[iVar9];
    (&DAT_00bcd662)[iVar9] = cVar4 + 1U;
    if (100 < (byte)(cVar4 + 1U)) {
      (&DAT_00bcd662)[iVar9] = 0;
    }
  }
  if (bVar3) {
    if ((10 < (byte)(&DAT_00bcd602)[*(short *)(&DAT_00bcd67a + iVar9) * 0x184]) &&
       ((byte)(&DAT_00bcd602)[*(short *)(&DAT_00bcd67a + iVar9) * 0x184] < 0xe)) {
      *param_2 = 0;
      goto LAB_004aea17;
    }
  }
  else if ((10 < (byte)(&DAT_00bcd602)[*(short *)(&DAT_00bcd67a + iVar9) * 0x184]) &&
          ((byte)(&DAT_00bcd602)[*(short *)(&DAT_00bcd67a + iVar9) * 0x184] < 0xe))
  goto LAB_004aea17;
  FUN_004ae2c0(param_1,param_2);
LAB_004aea17: ;
  *(short *)(&DAT_00bcd62e + iVar9) = *(short *)(&DAT_00bcd62e + iVar9) + 1;
  if (((&DAT_00bcd609)[iVar9] == '\b') || (0x31 < *(short *)(&DAT_00bcd62e + iVar9))) {
    *(undefined2 *)(&DAT_00bcd62e + iVar9) = 0;
    *(undefined4 *)(&DAT_00bcd754 + iVar9) = 0x10;
    FUN_004b3250(param_1);
  }
  return;
}
