#include "restore_types.h"
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd624 (*(undefined4 *)0x00bcd624u)
#define DAT_00bcd626 (*(undefined4 *)0x00bcd626u)
#define DAT_00bcd628 (*(undefined4 *)0x00bcd628u)
#define DAT_00bcd62a (*(undefined4 *)0x00bcd62au)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd65d (*(undefined4 *)0x00bcd65du)
#define DAT_00bcd728 (*(undefined4 *)0x00bcd728u)
#define _DAT_00784c90 (*(undefined4 *)0x00784c90u)
#define _DAT_00784c92 (*(undefined4 *)0x00784c92u)
extern int FUN_0040c180();
extern void FUN_004b3250();

void FUN_004ad350(int param_1,undefined4 *param_2)

{
  undefined2 uVar1;
  undefined2 uVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = param_1;
  iVar4 = param_1 * 0x184;
  (&DAT_00bcd65d)[iVar4] = 0;
  *param_2 = 0;
  *(short *)(&DAT_00bcd62e + iVar4) = *(short *)(&DAT_00bcd62e + iVar4) + -1;
  if (*(short *)(&DAT_00bcd62e + iVar4) < 1) {
    *(undefined2 *)(&DAT_00bcd62e + iVar4) = 0;
    uVar1 = _DAT_00784c90;
    if ((&DAT_00bcd728)[iVar4] != '\0') {
      *(undefined2 *)(&DAT_00bcd624 + iVar4) = (&DAT_00bcd60c)[param_1 * 0xc2];
      *(undefined2 *)(&DAT_00bcd626 + iVar4) = (&DAT_00bcd60e)[param_1 * 0xc2];
      FUN_0040c180((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                   (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],
                   (int)*(short *)(&DAT_00bcd628 + iVar4),(int)*(short *)(&DAT_00bcd62a + iVar4),
                   &param_1,&param_2);
      *(undefined2 *)(&DAT_00bcd61c + iVar4) = (undefined2)param_1;
      *(undefined2 *)(&DAT_00bcd61e + iVar4) = RESTORE_SLICE2(param_2,0);
      (&DAT_00bcd630)[iVar4] = 0xd;
      FUN_004b3250(iVar3);
      return;
    }
    *(undefined2 *)(&DAT_00bcd624 + iVar4) = (&DAT_00bcd60c)[param_1 * 0xc2];
    uVar2 = _DAT_00784c92;
    *(undefined2 *)(&DAT_00bcd626 + iVar4) = (&DAT_00bcd60e)[param_1 * 0xc2];
    *(undefined2 *)(&DAT_00bcd61c + iVar4) = uVar1;
    *(undefined2 *)(&DAT_00bcd61e + iVar4) = uVar2;
    (&DAT_00bcd630)[iVar4] = 0x11;
  }
  return;
}
