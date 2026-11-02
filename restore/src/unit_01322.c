#include "restore_types.h"
#define DAT_007999ec (*(undefined4 *)0x007999ecu)
#define DAT_007999f8 (*(undefined4 *)0x007999f8u)
#define DAT_00799a8c (*(undefined4 *)0x00799a8cu)
#define DAT_00bcd603 (*(undefined4 *)0x00bcd603u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00bcd666 (*(undefined4 *)0x00bcd666u)
#define DAT_00bcd667 (*(undefined4 *)0x00bcd667u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
#define DAT_00ea5d04 (*(undefined4 *)0x00ea5d04u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_00445ac0();

undefined4 FUN_00495950(int param_1)

{
  undefined2 uVar1;
  undefined2 uVar2;
  int iVar3;
  int iVar4;
  
  if (*(short *)(&DAT_00799a8c + param_1 * 0x108) == 0) {
    return 0;
  }
  iVar4 = *(short *)(&DAT_00799a8c + param_1 * 0x108) * 0x184;
  if ((&DAT_00bcd630)[iVar4] != '\n') {
    return 0;
  }
  if (0xb < (byte)(&DAT_00bcd667)[iVar4]) {
    return 0;
  }
  iVar4 = FUN_00445ac0((int)(short)(&DAT_00bcd60c)[DAT_00d056f8 * 0xc2],
                       (int)(short)(&DAT_00bcd60e)[DAT_00d056f8 * 0xc2],(&DAT_00bcd666)[iVar4],
                       (int)(short)(&DAT_007999f8)[param_1 * 0x84],(&DAT_007999ec)[param_1 * 0x108])
  ;
  uVar1 = DAT_00d3b4e0;
  if (iVar4 == 0) {
    return 0;
  }
  iVar3 = DAT_00d056f8 * 0x184;
  (&DAT_00bcd656)[DAT_00d056f8 * 0xc2] = (short)iVar4;
  uVar2 = DAT_00d3b4e4;
  (&DAT_00bcd630)[iVar3] = 0xc;
  *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0;
  *(undefined2 *)(&DAT_00bcd61c + iVar3) = uVar2;
  iVar4 = DAT_00eaac28;
  *(undefined2 *)(&DAT_00bcd61e + iVar3) = uVar1;
  (&DAT_00bcd603)[iVar3] = *(undefined1 *)(&DAT_00ea5d04 + iVar4 * 0x1276);
  return 1;
}
