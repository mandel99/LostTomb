#include "restore_types.h"
#define DAT_007057b8 (*(undefined *)0x007057b8u)
#define DAT_007057cc (*(undefined *)0x007057ccu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a44 (*(undefined4 *)0x00799a44u)

int FUN_004340e0(int param_1)

{
  if ((&DAT_007999e8)[param_1 * 0x84] == 0x1f) {
    return 0;
  }
  if ((&DAT_007999e8)[param_1 * 0x84] == 0x24) {
    return 0;
  }
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) == '\0') {
    return 0;
  }
  if (DAT_007057cc + DAT_007057b8 == 0) {
    return 0;
  }
  return 10 - *(byte *)(&DAT_00799a44 + param_1 * 0x84) / 10;
}
