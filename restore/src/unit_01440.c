#include "restore_types.h"
#define DAT_00bcd642 (*(undefined4 *)0x00bcd642u)
#define DAT_00bcd646 (*(undefined4 *)0x00bcd646u)

void FUN_004b43e0(int param_1)

{
  short sVar1;
  
  sVar1 = (&DAT_00bcd642)[param_1 * 0xc2];
  if (sVar1 < *(short *)(&DAT_00bcd646 + param_1 * 0x184)) {
    (&DAT_00bcd642)[param_1 * 0xc2] = sVar1 + 1;
    return;
  }
  if (*(short *)(&DAT_00bcd646 + param_1 * 0x184) < sVar1) {
    (&DAT_00bcd642)[param_1 * 0xc2] = sVar1 + -1;
  }
  return;
}
