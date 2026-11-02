#include "restore_types.h"
#define DAT_00bcd64a (*(undefined4 *)0x00bcd64au)
#define DAT_00bcd64c (*(undefined4 *)0x00bcd64cu)
#define DAT_00bcd64e (*(undefined4 *)0x00bcd64eu)
#define DAT_00bcd650 (*(undefined4 *)0x00bcd650u)

void FUN_004b4180(int param_1)

{
  short sVar1;
  
  sVar1 = (&DAT_00bcd64e)[param_1 * 0xc2];
  if ((&DAT_00bcd650)[param_1 * 0x184] != '\x01') {
    if (sVar1 < 0) {
      (&DAT_00bcd64e)[param_1 * 0xc2] = (&DAT_00bcd64a)[param_1 * 0xc2] * 2 + sVar1;
      (&DAT_00bcd64c)[param_1 * 0xc2] = (&DAT_00bcd64c)[param_1 * 0xc2] + -1;
      return;
    }
    (&DAT_00bcd64e)[param_1 * 0xc2] =
         sVar1 + ((&DAT_00bcd64a)[param_1 * 0xc2] - (&DAT_00bcd64c)[param_1 * 0xc2]) * 2;
    (&DAT_00bcd64c)[param_1 * 0xc2] = (&DAT_00bcd64c)[param_1 * 0xc2] + -1;
    return;
  }
  if (sVar1 < 0) {
    (&DAT_00bcd64e)[param_1 * 0xc2] = (&DAT_00bcd64c)[param_1 * 0xc2] * 2 + sVar1;
    (&DAT_00bcd64a)[param_1 * 0xc2] = (&DAT_00bcd64a)[param_1 * 0xc2] + -1;
    return;
  }
  (&DAT_00bcd64e)[param_1 * 0xc2] =
       sVar1 + ((&DAT_00bcd64c)[param_1 * 0xc2] - (&DAT_00bcd64a)[param_1 * 0xc2]) * 2;
  (&DAT_00bcd64a)[param_1 * 0xc2] = (&DAT_00bcd64a)[param_1 * 0xc2] + -1;
  return;
}
