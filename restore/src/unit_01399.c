#include "restore_types.h"
#define DAT_007090ec (*(undefined4 *)0x007090ecu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd628 (*(undefined4 *)0x00bcd628u)
#define DAT_00bcd62a (*(undefined4 *)0x00bcd62au)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd728 (*(undefined4 *)0x00bcd728u)
#define DAT_00bcd774 (*(undefined4 *)0x00bcd774u)
extern void FUN_004463c0();
extern void FUN_00469140();
extern void FUN_004a5da0();
extern void FUN_004ad4c0();
extern uint FUN_004ad880();
extern void FUN_004b3250();

void FUN_004adb40(int param_1,undefined4 *param_2)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  iVar3 = param_1;
  iVar7 = param_1 * 0x184;
  FUN_004a5da0(param_1,*(undefined4 *)(&DAT_007090ec + (uint)(byte)(&DAT_00bcd602)[iVar7] * 0x38));
  cVar1 = (&DAT_00bcd609)[iVar7];
  if (cVar1 == '\b') {
    *param_2 = 0;
    param_1 = (int)*(short *)(&DAT_00bcd628 + iVar7);
    param_2 = (undefined4 *)(int)*(short *)(&DAT_00bcd62a + iVar7);
    FUN_004ad880(&param_1,&param_2);
    iVar5 = (int)param_2;
    iVar4 = param_1;
    iVar6 = DAT_00bca2d8 + (int)param_2 * 0xe4 + param_1;
    uVar2 = (&DAT_00af598c)[iVar6];
    if ((uVar2 & 0x203) != 0) {
      *(undefined2 *)(&DAT_00bcd62e + iVar7) = 0x32;
      (&DAT_00bcd728)[iVar7] = 0;
      (&DAT_00bcd630)[iVar7] = 8;
    }
    if ((uVar2 & 4) == 0) {
      if ((uVar2 & 0xc108) != 0) {
        FUN_00469140(param_1,param_2,iVar6);
      }
    }
    else if ((uVar2 & 0x40) == 0) {
      *(undefined2 *)(&DAT_00bcd62e + iVar7) = 0x32;
      (&DAT_00bcd728)[iVar7] = 0;
      (&DAT_00bcd630)[iVar7] = 8;
    }
    FUN_004ad4c0(iVar3,iVar4,iVar5);
    FUN_004463c0(*(undefined4 *)(&DAT_00bcd774 + iVar7),*(undefined4 *)(&DAT_00bcd618 + iVar7));
    return;
  }
  if (cVar1 == '\t') {
    FUN_004b3250(iVar3);
    return;
  }
  if (cVar1 == '\n') {
    (&DAT_00bcd630)[iVar7] = 4;
  }
  return;
}
