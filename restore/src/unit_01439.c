#include "restore_types.h"
#define DAT_00bcd644 (*(undefined4 *)0x00bcd644u)
#define DAT_00bcd648 (*(undefined4 *)0x00bcd648u)

void FUN_004b43a0(int param_1)

{
  short sVar1;
  
  sVar1 = (&DAT_00bcd644)[param_1 * 0xc2];
  if (sVar1 < *(short *)(&DAT_00bcd648 + param_1 * 0x184)) {
    (&DAT_00bcd644)[param_1 * 0xc2] = sVar1 + 1;
    return;
  }
  if (*(short *)(&DAT_00bcd648 + param_1 * 0x184) < sVar1) {
    (&DAT_00bcd644)[param_1 * 0xc2] = sVar1 + -1;
  }
  return;
}
