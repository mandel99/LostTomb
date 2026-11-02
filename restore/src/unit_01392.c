#include "restore_types.h"
#define DAT_007090e4 (*(undefined4 *)0x007090e4u)
#define DAT_007090e8 (*(undefined4 *)0x007090e8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd608 (*(undefined4 *)0x00bcd608u)
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
#define DAT_00bcd67a (*(undefined4 *)0x00bcd67au)
#define DAT_00bcd744 (*(undefined4 *)0x00bcd744u)
#define DAT_00bcd746 (*(undefined4 *)0x00bcd746u)
#define DAT_00bcd754 (*(undefined4 *)0x00bcd754u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern void FUN_004a6160();
extern void FUN_004b3250();
extern int FUN_004b45c0();
extern int FUN_004bcae0();

void FUN_004a6d40(int param_1,undefined4 param_2)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar6 = param_1 * 0x184;
  iVar2 = (int)*(short *)(&DAT_00bcd67a + iVar6);
  if (((iVar2 == 0) || ((&DAT_00bcd606)[iVar2 * 0x184] != '\x01')) ||
     ((&DAT_00bcd630)[iVar2 * 0x184] == '\x04')) {
    iVar2 = FUN_004bcae0(param_1,0xe4);
    *(short *)(&DAT_00bcd67a + iVar6) = (short)iVar2;
    *(undefined4 *)(&DAT_00bcd754 + iVar6) = 0x10;
  }
  if (*(int *)(&DAT_00bcd754 + iVar6) == 9) {
    iVar4 = (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2];
    _DAT_00d3b4f8 = (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2];
    iVar3 = (int)(short)(&DAT_00bcd60c)[iVar2 * 0xc2];
    iVar5 = (int)(short)(&DAT_00bcd60e)[iVar2 * 0xc2];
    if (iVar3 < iVar4) {
      DAT_00d3b4fc = iVar4 - iVar3;
    }
    else if (iVar4 < iVar3) {
      DAT_00d3b4fc = iVar3 - iVar4;
    }
    else {
      DAT_00d3b4fc = 0;
    }
    if (iVar5 < _DAT_00d3b4f8) {
      _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar5;
    }
    else if (_DAT_00d3b4f8 < iVar5) {
      _DAT_00d3b4f8 = iVar5 - _DAT_00d3b4f8;
    }
    else {
      _DAT_00d3b4f8 = 0;
    }
    iVar3 = _DAT_00d3b4f8;
    if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
      iVar3 = DAT_00d3b4fc;
    }
    if (((iVar3 < *(int *)(&DAT_007090e4 + (uint)(byte)(&DAT_00bcd602)[iVar6] * 0x38)) &&
        (cVar1 = (&DAT_00bcd663)[iVar6], (&DAT_00bcd663)[iVar6] = cVar1 + 1U,
        *(int *)(&DAT_007090e8 + (uint)(byte)(&DAT_00bcd602)[iVar6] * 0x38) <
        (int)(uint)(byte)(cVar1 + 1U))) && ((&DAT_00bcd663)[iVar6] = 0, iVar2 != 0)) {
      (&DAT_00bcd662)[iVar6] = 1;
    }
    else {
      (&DAT_00bcd662)[iVar6] = 0;
    }
    if ((&DAT_00bcd662)[iVar6] != '\0') {
      if ((&DAT_00bcd662)[iVar6] == '\x01') {
        FUN_004b45c0(param_1,(int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                     (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],
                     (int)(short)(&DAT_00bcd60c)[iVar2 * 0xc2],
                     (int)(short)(&DAT_00bcd60e)[iVar2 * 0xc2],0x3c);
      }
      cVar1 = (&DAT_00bcd662)[iVar6];
      (&DAT_00bcd662)[iVar6] = cVar1 + 1U;
      if (100 < (byte)(cVar1 + 1U)) {
        (&DAT_00bcd662)[iVar6] = 0;
      }
    }
    FUN_004a6160(param_1,param_2);
    if (((&DAT_00bcd609)[iVar6] == '\b') ||
       (((*(short *)(&DAT_00bcd62e + iVar6) = *(short *)(&DAT_00bcd62e + iVar6) + 1,
         10 < *(short *)(&DAT_00bcd62e + iVar6) &&
         (*(undefined2 *)(&DAT_00bcd62e + iVar6) = 0,
         *(short *)(&DAT_00bcd61c + iVar6) != (&DAT_00bcd60c)[iVar2 * 0xc2])) &&
        (*(short *)(&DAT_00bcd61e + iVar6) != (&DAT_00bcd60e)[iVar2 * 0xc2])))) {
      *(undefined4 *)(&DAT_00bcd754 + iVar6) = 0x10;
      FUN_004b3250(param_1);
    }
  }
  else if (*(int *)(&DAT_00bcd754 + iVar6) == 0x10) {
    if (iVar2 != 0) {
      *(undefined4 *)(&DAT_00bcd754 + iVar6) = 9;
      (&DAT_00bcd609)[iVar6] = (&DAT_00bcd60a)[iVar6];
      *(undefined2 *)(&DAT_00bcd61c + iVar6) = (&DAT_00bcd60c)[iVar2 * 0xc2];
      *(undefined2 *)(&DAT_00bcd61e + iVar6) = (&DAT_00bcd60e)[iVar2 * 0xc2];
      *(short *)(&DAT_00bcd67a + iVar6) = (short)iVar2;
      FUN_004b3250(param_1);
      return;
    }
    (&DAT_00bcd630)[iVar6] = (&DAT_00bcd608)[iVar6];
    *(undefined2 *)(&DAT_00bcd61c + iVar6) = (&DAT_00bcd60c)[param_1 * 0xc2];
    *(undefined2 *)(&DAT_00bcd61e + iVar6) = (&DAT_00bcd60e)[param_1 * 0xc2];
    *(undefined2 *)(&DAT_00bcd744 + iVar6) = (&DAT_00bcd60c)[param_1 * 0xc2];
    *(undefined2 *)(&DAT_00bcd746 + iVar6) = (&DAT_00bcd60e)[param_1 * 0xc2];
    if ((&DAT_00bcd630)[iVar6] != '\x17') {
      if ((&DAT_00bcd630)[iVar6] == '\n') {
        *(undefined4 *)(&DAT_00bcd754 + iVar6) = 0x10;
      }
      *(undefined2 *)(&DAT_00bcd67a + iVar6) = 0;
      FUN_004b3250(param_1);
      return;
    }
    *(undefined4 *)(&DAT_00bcd754 + iVar6) = 0x16;
    *(undefined2 *)(&DAT_00bcd67a + iVar6) = 0;
    FUN_004b3250(param_1);
    return;
  }
  return;
}
