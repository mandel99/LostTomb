#include "restore_types.h"
#define DAT_007057f4 (*(undefined *)0x007057f4u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a4e (*(undefined4 *)0x00799a4eu)

int FUN_00434690(int param_1)

{
  if ((&DAT_007999e8)[param_1 * 0x84] == 0x2e) {
    return 0;
  }
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) == '\0') {
    return 0;
  }
  if (DAT_007057f4 == 0) {
    return 0;
  }
  return 10 - (byte)(&DAT_00799a4e)[param_1 * 0x108] / 10;
}
