#include "restore_types.h"
#define DAT_007090ec (*(undefined4 *)0x007090ecu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd728 (*(undefined4 *)0x00bcd728u)
#define _DAT_00784c8c (*(undefined4 *)0x00784c8cu)
#define _DAT_00784c8e (*(undefined4 *)0x00784c8eu)
#define _DAT_00784c90 (*(undefined4 *)0x00784c90u)
#define _DAT_00784c92 (*(undefined4 *)0x00784c92u)
extern void FUN_004a5da0();
extern void FUN_004b3250();

void FUN_004adc80(int param_1)

{
  char cVar1;
  short sVar2;
  short sVar3;
  int iVar4;
  
  iVar4 = param_1 * 0x184;
  FUN_004a5da0(param_1,*(undefined4 *)(&DAT_007090ec + (uint)(byte)(&DAT_00bcd602)[iVar4] * 0x38));
  sVar3 = _DAT_00784c92;
  sVar2 = _DAT_00784c8e;
  cVar1 = (&DAT_00bcd609)[iVar4];
  if (cVar1 == '\t') {
LAB_004add77: ;
    FUN_004b3250(param_1);
    return;
  }
  if (cVar1 != '\n') {
    if (cVar1 != '\b') {
      return;
    }
    if (((&DAT_00bcd60c)[param_1 * 0xc2] == _DAT_00784c8c) &&
       ((&DAT_00bcd60e)[param_1 * 0xc2] == _DAT_00784c8e)) {
      *(short *)(&DAT_00bcd61c + iVar4) = _DAT_00784c90;
      *(short *)(&DAT_00bcd61e + iVar4) = sVar3;
      (&DAT_00bcd630)[iVar4] = 0x11;
      FUN_004b3250(param_1);
      return;
    }
    if (((&DAT_00bcd60c)[param_1 * 0xc2] != _DAT_00784c90) ||
       ((&DAT_00bcd60e)[param_1 * 0xc2] != _DAT_00784c92)) {
      *(short *)(&DAT_00bcd61c + iVar4) = _DAT_00784c8c;
      *(short *)(&DAT_00bcd61e + iVar4) = sVar2;
      (&DAT_00bcd630)[iVar4] = 0x11;
      goto LAB_004add77;
    }
    if ((&DAT_00bcd728)[iVar4] != '\0') {
      FUN_004b3250(param_1);
      (&DAT_00bcd630)[iVar4] = 8;
      return;
    }
  }
  (&DAT_00bcd630)[iVar4] = 4;
  return;
}
