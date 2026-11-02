#include "restore_types.h"
#define DAT_00bcd60a (*(undefined4 *)0x00bcd60au)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
extern int FUN_004bcd60();
extern undefined1 FUN_0051cd80();

void FUN_004a4f60(int param_1)

{
  undefined1 uVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = param_1 * 0x184;
  *(short *)(&DAT_00bcd62e + iVar3) = *(short *)(&DAT_00bcd62e + iVar3) + 1;
  if (0x13 < *(short *)(&DAT_00bcd62e + iVar3)) {
    *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0;
    iVar2 = FUN_004bcd60(param_1,5);
    uVar1 = FUN_0051cd80((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60c)[iVar2 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[iVar2 * 0xc2]);
    (&DAT_00bcd60a)[iVar3] = uVar1;
  }
  return;
}
