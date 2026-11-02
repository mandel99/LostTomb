#include "restore_types.h"
#define DAT_00705a74 (*(undefined *)0x00705a74u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a46 (*(undefined4 *)0x00799a46u)

int FUN_00434630(int param_1)

{
  if ((&DAT_007999e8)[param_1 * 0x84] == 0x31) {
    return 0;
  }
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) == '\0') {
    return 0;
  }
  if (DAT_00705a74 == 0) {
    return 0;
  }
  return 10 - (byte)(&DAT_00799a46)[param_1 * 0x108] / 10;
}
