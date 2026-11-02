#include "restore_types.h"
#define DAT_00705a1c (*(undefined *)0x00705a1cu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a48 (*(undefined4 *)0x00799a48u)

int FUN_00434750(int param_1)

{
  if ((&DAT_007999e8)[param_1 * 0x84] == 0xb8) {
    return 0;
  }
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) == '\0') {
    return 0;
  }
  if (DAT_00705a1c == 0) {
    return 0;
  }
  return 10 - (byte)(&DAT_00799a48)[param_1 * 0x108] / 10;
}
