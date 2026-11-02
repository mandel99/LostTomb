#include "restore_types.h"
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a8c (*(undefined4 *)0x00799a8cu)
#define DAT_00bcd5f9 (*(undefined4 *)0x00bcd5f9u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd624 (*(undefined4 *)0x00bcd624u)
#define DAT_00bcd626 (*(undefined4 *)0x00bcd626u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_004806c0();
extern void FUN_004b3250();

void FUN_004a4770(int param_1,int param_2)

{
  undefined1 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  
  iVar4 = param_1 * 0x184;
  *(short *)(&DAT_00bcd62e + iVar4) = *(short *)(&DAT_00bcd62e + iVar4) + 1;
  if (0x31 < *(short *)(&DAT_00bcd62e + iVar4)) {
    *(undefined2 *)(&DAT_00bcd62e + iVar4) = 0;
    (&DAT_00bcd5f9)[iVar4] = 0;
    uVar3 = _DAT_00d3b4e4;
    uVar2 = _DAT_00d3b4e0;
    if (param_2 != 0) {
      param_2 = param_2 * 0x108;
      *(short *)(&DAT_00799a8c + param_2) = (short)param_1;
      uVar1 = (&DAT_007999db)[param_2];
      (&DAT_00bcd630)[iVar4] = 9;
      FUN_004806c0((int)*(short *)((int)&DAT_007999dc + param_2 + 2),
                   (int)*(short *)(&DAT_007999e0 + param_2),uVar1);
      *(undefined2 *)(&DAT_00bcd624 + iVar4) = (&DAT_00bcd60c)[param_1 * 0xc2];
      *(undefined2 *)(&DAT_00bcd626 + iVar4) = (&DAT_00bcd60e)[param_1 * 0xc2];
      *(undefined2 *)(&DAT_00bcd61c + iVar4) = DAT_00d3b4e4;
      *(undefined2 *)(&DAT_00bcd61e + iVar4) = DAT_00d3b4e0;
      _DAT_00d3b4e0 = uVar2;
      _DAT_00d3b4e4 = uVar3;
      FUN_004b3250(param_1);
    }
  }
  return;
}
