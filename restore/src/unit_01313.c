#include "restore_types.h"
#define DAT_0078d7f8 (*(undefined4 *)0x0078d7f8u)
#define DAT_0078d804 (*(undefined *)0x0078d804u)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a08 (*(undefined4 *)0x00799a08u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00bcd663 (*(undefined4 *)0x00bcd663u)
extern int FUN_00469a50();
extern void FUN_004b3250();

undefined4 FUN_0048e5e0(int param_1)

{
  char cVar1;
  undefined2 uVar2;
  int iVar3;
  int iVar4;
  
  if (DAT_0078d804 < 1) {
    return 0;
  }
  iVar4 = param_1 * 0x184;
  cVar1 = (&DAT_00bcd630)[iVar4];
  if (cVar1 == '\x05') {
    return 0;
  }
  if (cVar1 == '\x04') {
    return 0;
  }
  if (cVar1 == '\b') {
    return 0;
  }
  if (cVar1 == '\t') {
    return 0;
  }
  if (cVar1 == '\f') {
    return 0;
  }
  if (cVar1 == '\r') {
    return 0;
  }
  if (cVar1 == '\x0e') {
    return 0;
  }
  if (cVar1 == '\x0f') {
    return 0;
  }
  cVar1 = (&DAT_00bcd663)[iVar4];
  (&DAT_00bcd663)[iVar4] = cVar1 + 1U;
  if ((byte)(cVar1 + 1U) < 0x14) {
    return 0;
  }
  iVar3 = FUN_00469a50((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                       (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
  if (iVar3 < 1) {
    return 0;
  }
  if (0x19 < DAT_0078d7f8) {
    return 0;
  }
  (&DAT_00bcd663)[iVar4] = 0;
  (&DAT_00bcd630)[iVar4] = 0xc;
  uVar2 = (&DAT_00799a00)[iVar3 * 0x84];
  *(undefined2 *)(&DAT_00bcd61c + iVar4) = (&DAT_007999fe)[iVar3 * 0x84];
  *(undefined2 *)(&DAT_00bcd61e + iVar4) = uVar2;
  (&DAT_00bcd656)[param_1 * 0xc2] = (short)iVar3;
  FUN_004b3250(param_1);
  *(short *)(&DAT_00799a08 + iVar3 * 0x108) = (short)param_1;
  return 1;
}
