#include "restore_types.h"
#define DAT_00bcd6b4 (*(undefined4 *)0x00bcd6b4u)

uint FUN_00495d70(int param_1)

{
  int *piVar1;
  int iVar2;
  
  iVar2 = 0;
  piVar1 = (int *)(&DAT_00bcd6b4 + param_1 * 0x184);
  do {
    if (*piVar1 != 0) {
      return CONCAT31((int3)((uint)piVar1 >> 8),1);
    }
    iVar2 = iVar2 + 1;
    piVar1 = piVar1 + 1;
  } while (iVar2 < 4);
  return (uint)piVar1 & 0xffffff00;
}
