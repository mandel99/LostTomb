#include "restore_types.h"
#define DAT_007090ec (*(undefined4 *)0x007090ecu)
#define DAT_00bcb9f6 (*(undefined4 *)0x00bcb9f6u)
#define DAT_00bcb9f8 (*(undefined4 *)0x00bcb9f8u)
#define DAT_00bcb9fa (*(undefined4 *)0x00bcb9fau)
#define DAT_00bcb9fc (*(undefined4 *)0x00bcb9fcu)
#define DAT_00bcba4c (*(undefined4 *)0x00bcba4cu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd628 (*(undefined4 *)0x00bcd628u)
#define DAT_00bcd62a (*(undefined4 *)0x00bcd62au)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd68a (*(undefined4 *)0x00bcd68au)
#define DAT_00bcd728 (*(undefined4 *)0x00bcd728u)
extern int FUN_0040bda0();
extern void FUN_004a44f0();
extern void FUN_004a5da0();
extern uint FUN_004ad880();
extern void FUN_004b3250();
extern undefined4 FUN_004b6400();

void FUN_004a4e10(int param_1,undefined4 *param_2)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  undefined4 local_4;
  
  iVar3 = param_1;
  iVar4 = param_1 * 0x184;
  FUN_004a5da0(param_1,*(undefined4 *)(&DAT_007090ec + (uint)(byte)(&DAT_00bcd602)[iVar4] * 0x38));
  uVar2 = (uint)(byte)(&DAT_00bcd68a)[iVar4];
  cVar1 = (&DAT_00bcd609)[iVar4];
  if (cVar1 == '\b') {
    *param_2 = 0;
    if ((*(short *)(&DAT_00bcd628 + iVar4) != 0) || (*(short *)(&DAT_00bcd62a + iVar4) != 0)) {
      FUN_0040bda0((int)*(short *)(&DAT_00bcd628 + iVar4),(int)*(short *)(&DAT_00bcd62a + iVar4),
                   (int)(short)(&DAT_00bcd60c)[iVar3 * 0xc2],
                   (int)(short)(&DAT_00bcd60e)[iVar3 * 0xc2],&param_1,&local_4);
      FUN_004ad880(&param_1,&local_4);
      iVar3 = uVar2 * 0x90;
      *(short *)(&DAT_00bcb9fa + iVar3) = (short)param_1;
      *(short *)(&DAT_00bcb9fc + iVar3) = (short)local_4;
      *(short *)(&DAT_00bcb9f6 + iVar3) = (short)param_1;
      *(short *)(&DAT_00bcb9f8 + iVar3) = (short)local_4;
      (&DAT_00bcba4c)[uVar2 * 0x24] = 0;
      FUN_004a44f0(uVar2,param_1,local_4);
      (&DAT_00bcd728)[iVar4] = 0;
      (&DAT_00bcd68a)[iVar4] = 0;
      FUN_004b6400((int)*(short *)(&DAT_00bcd628 + iVar4),(int)*(short *)(&DAT_00bcd62a + iVar4),
                   uVar2,0);
      (&DAT_00bcd630)[iVar4] = 9;
      *(undefined2 *)(&DAT_00bcd628 + iVar4) = 0;
      *(undefined2 *)(&DAT_00bcd62a + iVar4) = 0;
      return;
    }
  }
  else {
    if (cVar1 == '\t') {
      FUN_004b3250(iVar3);
      return;
    }
    if (cVar1 == '\n') {
      FUN_004b3250(iVar3);
      (&DAT_00bcd630)[iVar4] = 10;
    }
  }
  return;
}
