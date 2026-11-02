#include "restore_types.h"
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd754 (*(undefined4 *)0x00bcd754u)
#define _DAT_00784c8c (*(undefined4 *)0x00784c8cu)
#define _DAT_00784c8e (*(undefined4 *)0x00784c8eu)
#define _DAT_00784c90 (*(undefined4 *)0x00784c90u)
#define _DAT_00784c92 (*(undefined4 *)0x00784c92u)
extern void FUN_004a5680();
extern undefined4 FUN_004aeab0();
extern void FUN_004b3250();
extern uint FUN_00562348();

void FUN_004aeb00(int param_1,undefined4 *param_2)

{
  short sVar1;
  undefined2 uVar2;
  int iVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  int local_4;
  
  iVar3 = param_1;
  FUN_004a5680(param_1,0);
  iVar6 = iVar3 * 0x184;
  *param_2 = 1;
  *(short *)(&DAT_00bcd62e + iVar6) = *(short *)(&DAT_00bcd62e + iVar6) + 1;
  iVar5 = FUN_00562348();
  if (iVar5 % 0x1e + 0x4b <= (int)*(short *)(&DAT_00bcd62e + iVar6)) {
    *(undefined2 *)(&DAT_00bcd62e + iVar6) = 0;
    (&DAT_00bcd630)[iVar6] = 10;
    *(undefined4 *)(&DAT_00bcd754 + iVar6) = 0x10;
  }
  cVar4 = (&DAT_00bcd609)[iVar6];
  if (cVar4 == '\t') {
    FUN_004b3250(iVar3);
    return;
  }
  if (cVar4 == '\n') {
    FUN_004b3250(iVar3);
    return;
  }
  if (cVar4 == '\b') {
    cVar4 = FUN_004aeab0(&param_1,&local_4);
    uVar2 = _DAT_00784c92;
    sVar1 = _DAT_00784c8e;
    if (((&DAT_00bcd60c)[iVar3 * 0xc2] == _DAT_00784c8c) &&
       ((&DAT_00bcd60e)[iVar3 * 0xc2] == _DAT_00784c8e)) {
      if (cVar4 == '\0') {
        *(undefined2 *)(&DAT_00bcd61c + iVar6) = _DAT_00784c90;
        *(undefined2 *)(&DAT_00bcd61e + iVar6) = uVar2;
      }
      else {
        *(undefined2 *)(&DAT_00bcd61c + iVar6) = (undefined2)param_1;
        *(undefined2 *)(&DAT_00bcd61e + iVar6) = (undefined2)local_4;
      }
    }
    else if ((cVar4 == '\0') ||
            (((short)(&DAT_00bcd60c)[iVar3 * 0xc2] != param_1 ||
             ((short)(&DAT_00bcd60e)[iVar3 * 0xc2] != local_4)))) {
      *(short *)(&DAT_00bcd61c + iVar6) = _DAT_00784c8c;
      *(short *)(&DAT_00bcd61e + iVar6) = sVar1;
    }
    else {
      *(undefined2 *)(&DAT_00bcd61c + iVar6) = _DAT_00784c90;
      *(undefined2 *)(&DAT_00bcd61e + iVar6) = uVar2;
    }
    (&DAT_00bcd630)[iVar6] = 0x11;
    FUN_004b3250(iVar3);
  }
  return;
}
