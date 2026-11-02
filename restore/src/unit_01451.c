#include "restore_types.h"
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bcd600 (*(undefined4 *)0x00bcd600u)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd682 (*(undefined4 *)0x00bcd682u)
extern void FUN_0051f780();

void FUN_004b52f0(int param_1)

{
  short sVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = param_1 * 0x184;
  if (-1 < *(int *)(&DAT_00bcd618 + iVar3)) {
    iVar2 = (int)*(short *)(&DAT_00b350dc + *(int *)(&DAT_00bcd618 + iVar3) * 2);
    (&DAT_00bcd682)[iVar3] = 0;
    if (iVar2 == 0) {
      *(short *)(&DAT_00b350dc + *(int *)(&DAT_00bcd618 + iVar3) * 2) = (short)param_1;
      FUN_0051f780(param_1);
      return;
    }
    (&DAT_00bcd682)[iVar3] = 1;
    sVar1 = *(short *)(&DAT_00bcd600 + iVar2 * 0x184);
    while (sVar1 != 0) {
      iVar2 = (int)sVar1;
      if (iVar2 == param_1) goto LAB_004b53a4;
      (&DAT_00bcd682)[iVar3] = (&DAT_00bcd682)[iVar3] + '\x01';
      sVar1 = *(short *)(&DAT_00bcd600 + iVar2 * 0x184);
    }
    if (0x14 < (byte)(&DAT_00bcd682)[iVar3]) {
      (&DAT_00bcd682)[iVar3] = 0x14;
    }
    *(short *)(&DAT_00bcd600 + iVar2 * 0x184) = (short)param_1;
  }
LAB_004b53a4: ;
  FUN_0051f780(param_1);
  return;
}
