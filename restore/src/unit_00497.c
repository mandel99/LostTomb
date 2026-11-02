#include "restore_types.h"
#define DAT_007057b4 (*(undefined *)0x007057b4u)
#define DAT_007057c4 (*(undefined *)0x007057c4u)
#define DAT_007057cc (*(undefined *)0x007057ccu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a44 (*(undefined4 *)0x00799a44u)

int FUN_00434150(int param_1)

{
  short sVar1;
  
  sVar1 = (&DAT_007999e8)[param_1 * 0x84];
  if (sVar1 == 0x1e) {
    return 0;
  }
  if (sVar1 == 0x22) {
    return 0;
  }
  if (sVar1 == 0x24) {
    return 0;
  }
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) == '\0') {
    return 0;
  }
  if (DAT_007057cc + DAT_007057c4 + DAT_007057b4 == 0) {
    return 0;
  }
  return 10 - *(byte *)((int)&DAT_00799a44 + param_1 * 0x108 + 1) / 10;
}
