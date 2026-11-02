#include "restore_types.h"
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bcd600 (*(undefined4 *)0x00bcd600u)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
extern void FUN_0051f780();

void FUN_004b5420(int param_1)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = param_1 * 0x184;
  iVar2 = *(int *)(&DAT_00bcd618 + iVar3);
  if (iVar2 < 0) {
    FUN_0051f780(param_1);
    return;
  }
  iVar4 = (int)*(short *)(&DAT_00b350dc + iVar2 * 2);
  if (iVar4 != 0) {
    if (iVar4 == param_1) {
      *(undefined2 *)(&DAT_00b350dc + iVar2 * 2) = *(undefined2 *)(&DAT_00bcd600 + iVar3);
      *(undefined2 *)(&DAT_00bcd600 + iVar3) = 0;
      FUN_0051f780(param_1);
      return;
    }
    sVar1 = *(short *)(&DAT_00bcd600 + iVar4 * 0x184);
    while( true ) {
      iVar2 = (int)sVar1;
      if (iVar2 == param_1) {
        *(undefined2 *)(&DAT_00bcd600 + iVar4 * 0x184) = *(undefined2 *)(&DAT_00bcd600 + iVar3);
        *(undefined2 *)(&DAT_00bcd600 + iVar3) = 0;
        FUN_0051f780(param_1);
        FUN_0051f780(iVar4);
        return;
      }
      if (iVar4 == 0) break;
      sVar1 = *(short *)(&DAT_00bcd600 + iVar2 * 0x184);
      iVar4 = iVar2;
    }
    return;
  }
  FUN_0051f780(param_1);
  return;
}
