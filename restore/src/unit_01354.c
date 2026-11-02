#include "restore_types.h"
#define DAT_009784dc (*(undefined4 *)0x009784dcu)
#define DAT_00bcd714 (*(undefined4 *)0x00bcd714u)

int FUN_004a34b0(int param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  
  iVar1 = 0;
  iVar3 = 4;
  piVar2 = &DAT_009784dc + *(int *)(&DAT_00bcd714 + param_1 * 0x184) * 0xb;
  do {
    if (0 < *piVar2) {
      iVar1 = iVar1 + 1;
    }
    piVar2 = piVar2 + 1;
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  return iVar1;
}
