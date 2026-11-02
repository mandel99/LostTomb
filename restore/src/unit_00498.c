#include "restore_types.h"
#define DAT_007057c0 (*(undefined *)0x007057c0u)
#define DAT_007057c4 (*(undefined *)0x007057c4u)
#define DAT_007057c8 (*(undefined *)0x007057c8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a47 (*(undefined4 *)0x00799a47u)

int FUN_004341d0(int param_1)

{
  short sVar1;
  
  sVar1 = (&DAT_007999e8)[param_1 * 0x84];
  if (sVar1 == 0x21) {
    return 0;
  }
  if (sVar1 == 0x22) {
    return 0;
  }
  if (sVar1 == 0x23) {
    return 0;
  }
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) == '\0') {
    return 0;
  }
  if (DAT_007057c8 + DAT_007057c4 + DAT_007057c0 == 0) {
    return 0;
  }
  return 10 - (byte)(&DAT_00799a47)[param_1 * 0x108] / 10;
}
