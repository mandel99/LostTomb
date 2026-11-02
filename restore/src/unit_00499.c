#include "restore_types.h"
#define DAT_007057bc (*(undefined *)0x007057bcu)
#define DAT_007057d0 (*(undefined *)0x007057d0u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a49 (*(undefined4 *)0x00799a49u)

int FUN_00434250(int param_1)

{
  if ((&DAT_007999e8)[param_1 * 0x84] == 0x20) {
    return 0;
  }
  if ((&DAT_007999e8)[param_1 * 0x84] == 0x25) {
    return 0;
  }
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) == '\0') {
    return 0;
  }
  if (DAT_007057d0 + DAT_007057bc == 0) {
    return 0;
  }
  return 10 - (byte)(&DAT_00799a49)[param_1 * 0x108] / 10;
}
