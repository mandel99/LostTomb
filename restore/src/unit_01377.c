#include "restore_types.h"
#define DAT_007090ec (*(undefined4 *)0x007090ecu)
#define DAT_00bcb9fa (*(undefined4 *)0x00bcb9fau)
#define DAT_00bcb9fc (*(undefined4 *)0x00bcb9fcu)
#define DAT_00bcba4c (*(undefined4 *)0x00bcba4cu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd68a (*(undefined4 *)0x00bcd68au)
#define DAT_00bcd728 (*(undefined4 *)0x00bcd728u)
extern uint FUN_004a43f0();
extern void FUN_004a4470();
extern void FUN_004a5da0();
extern void FUN_004b3250();

void FUN_004a4d00(int param_1,undefined4 *param_2)

{
  char cVar1;
  undefined2 uVar2;
  uint uVar3;
  int iVar4;
  
  iVar4 = param_1 * 0x184;
  FUN_004a5da0(param_1,*(undefined4 *)(&DAT_007090ec + (uint)(byte)(&DAT_00bcd602)[iVar4] * 0x38));
  cVar1 = (&DAT_00bcd609)[iVar4];
  if (cVar1 == '\b') {
    *param_2 = 0;
    uVar3 = FUN_004a43f0(param_1);
    if (uVar3 != 0) {
      if (uVar3 == (byte)(&DAT_00bcd68a)[iVar4]) {
        if ((&DAT_00bcba4c)[uVar3 * 0x24] == 0) {
          (&DAT_00bcd728)[iVar4] = 1;
          uVar2 = (&DAT_00bcd60e)[param_1 * 0xc2];
          *(undefined2 *)(&DAT_00bcb9fa + uVar3 * 0x90) = (&DAT_00bcd60c)[param_1 * 0xc2];
          *(undefined2 *)(&DAT_00bcb9fc + uVar3 * 0x90) = uVar2;
          FUN_004a4470(uVar3);
          (&DAT_00bcd630)[iVar4] = 9;
          (&DAT_00bcba4c)[uVar3 * 0x24] = param_1;
          *(undefined2 *)(&DAT_00bcd62e + iVar4) = 0;
          return;
        }
        (&DAT_00bcd630)[iVar4] = 0xc;
        (&DAT_00bcd68a)[iVar4] = 0;
      }
      *(undefined2 *)(&DAT_00bcd62e + iVar4) = 0;
      return;
    }
  }
  else {
    if (cVar1 == '\t') {
      FUN_004b3250(param_1);
      return;
    }
    if (cVar1 == '\n') {
      FUN_004b3250(param_1);
      (&DAT_00bcd630)[iVar4] = 10;
    }
  }
  return;
}
