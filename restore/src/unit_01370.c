#include "restore_types.h"
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60a (*(undefined4 *)0x00bcd60au)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd624 (*(undefined4 *)0x00bcd624u)
#define DAT_00bcd626 (*(undefined4 *)0x00bcd626u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern int FUN_00406a30();
extern undefined4 FUN_004806c0();
extern void FUN_004b3250();

void FUN_004a45d0(int param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar3 = FUN_00406a30(DAT_00d056f8);
  if (0 < iVar3) {
    iVar5 = param_1 * 0x184;
    if ((&DAT_00bcd602)[iVar5] == 'M') {
      (&DAT_00bcd630)[iVar5] = 0xb;
    }
    else if ((&DAT_00bcd602)[iVar5] == 'N') {
      (&DAT_00bcd630)[iVar5] = 0xd;
    }
    uVar2 = _DAT_00d3b4e4;
    uVar1 = _DAT_00d3b4e0;
    iVar4 = iVar3 * 0x108;
    FUN_004806c0((int)*(short *)((int)&DAT_007999dc + iVar4 + 2),
                 (int)*(short *)(&DAT_007999e0 + iVar4),(&DAT_007999db)[iVar4]);
    *(undefined2 *)(&DAT_00bcd624 + iVar5) = (&DAT_00bcd60c)[param_1 * 0xc2];
    *(undefined2 *)(&DAT_00bcd626 + iVar5) = (&DAT_00bcd60e)[param_1 * 0xc2];
    (&DAT_00bcd609)[iVar5] = (&DAT_00bcd60a)[iVar5];
    (&DAT_00bcd656)[param_1 * 0xc2] = (short)iVar3;
    *(undefined2 *)(&DAT_00bcd61c + iVar5) = DAT_00d3b4e4;
    *(undefined2 *)(&DAT_00bcd61e + iVar5) = DAT_00d3b4e0;
    _DAT_00d3b4e0 = uVar1;
    _DAT_00d3b4e4 = uVar2;
    FUN_004b3250(param_1);
    return;
  }
  if (((&DAT_00bcd602)[param_1 * 0x184] == 'M') || ((&DAT_00bcd602)[param_1 * 0x184] == 'N')) {
    (&DAT_00bcd630)[param_1 * 0x184] = 8;
  }
  return;
}
