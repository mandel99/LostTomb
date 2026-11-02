#include "restore_types.h"
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd624 (*(undefined4 *)0x00bcd624u)
#define DAT_00bcd626 (*(undefined4 *)0x00bcd626u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd65d (*(undefined4 *)0x00bcd65du)
#define _DAT_00784c8c (*(undefined4 *)0x00784c8cu)
#define _DAT_00784c8e (*(undefined4 *)0x00784c8eu)
extern void FUN_004b3250();

void FUN_004ae1f0(int param_1)

{
  int iVar1;
  
  iVar1 = param_1 * 0x184;
  *(short *)(&DAT_00bcd62e + iVar1) = *(short *)(&DAT_00bcd62e + iVar1) + -1;
  if (*(short *)(&DAT_00bcd62e + iVar1) < 1) {
    *(undefined2 *)(&DAT_00bcd62e + iVar1) = 0;
    (&DAT_00bcd65d)[iVar1] = 0;
    *(undefined2 *)(&DAT_00bcd624 + iVar1) = (&DAT_00bcd60c)[param_1 * 0xc2];
    *(undefined2 *)(&DAT_00bcd626 + iVar1) = (&DAT_00bcd60e)[param_1 * 0xc2];
    *(undefined2 *)(&DAT_00bcd61c + iVar1) = _DAT_00784c8c;
    *(undefined2 *)(&DAT_00bcd61e + iVar1) = _DAT_00784c8e;
    (&DAT_00bcd630)[iVar1] = 0x11;
    FUN_004b3250(param_1);
  }
  return;
}
