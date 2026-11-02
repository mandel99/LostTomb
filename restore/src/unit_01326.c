#include "restore_types.h"
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd624 (*(undefined4 *)0x00bcd624u)
#define DAT_00bcd626 (*(undefined4 *)0x00bcd626u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
extern undefined4 FUN_00480180();

undefined4 FUN_00495c10(int param_1,int param_2)

{
  undefined2 uVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = param_1 * 0x184;
  (&DAT_00bcd630)[iVar3] = 8;
  (&DAT_00bcd656)[param_1 * 0xc2] = (short)param_2;
  param_2 = param_2 * 0x108;
  (&DAT_00bcd609)[iVar3] = 0;
  iVar2 = FUN_00480180((int)*(short *)((int)&DAT_007999dc + param_2 + 2),
                       (int)*(short *)(&DAT_007999e0 + param_2),(&DAT_007999db)[param_2]);
  uVar1 = DAT_00d3b4e0;
  if (iVar2 != 0) {
    *(undefined2 *)(&DAT_00bcd61c + iVar3) = DAT_00d3b4e4;
    *(undefined2 *)(&DAT_00bcd61e + iVar3) = uVar1;
    return 0;
  }
  iVar2 = FUN_00480180((int)*(short *)((int)&DAT_007999dc + param_2 + 2),
                       (int)*(short *)(&DAT_007999e0 + param_2),3);
  uVar1 = DAT_00d3b4e0;
  if (iVar2 == 0) {
    (&DAT_00bcd630)[iVar3] = 9;
    *(undefined2 *)(&DAT_00bcd61c + iVar3) = *(undefined2 *)(&DAT_00bcd624 + iVar3);
    *(undefined2 *)(&DAT_00bcd61e + iVar3) = *(undefined2 *)(&DAT_00bcd626 + iVar3);
    return 0;
  }
  *(undefined2 *)(&DAT_00bcd61c + iVar3) = DAT_00d3b4e4;
  *(undefined2 *)(&DAT_00bcd61e + iVar3) = uVar1;
  return 0;
}
