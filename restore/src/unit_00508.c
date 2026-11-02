#include "restore_types.h"
#define DAT_007057f8 (*(undefined *)0x007057f8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a50 (*(undefined4 *)0x00799a50u)

int FUN_004346f0(int param_1)

{
  if ((&DAT_007999e8)[param_1 * 0x84] == 0x2f) {
    return 0;
  }
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) == '\0') {
    return 0;
  }
  if (DAT_007057f8 == 0) {
    return 0;
  }
  return 10 - (byte)(&DAT_00799a50)[param_1 * 0x108] / 10;
}
