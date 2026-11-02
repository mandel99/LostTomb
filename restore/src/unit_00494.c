#include "restore_types.h"
#define DAT_007057b4 (*(undefined *)0x007057b4u)
#define DAT_007057b8 (*(undefined *)0x007057b8u)
#define DAT_007057bc (*(undefined *)0x007057bcu)
#define DAT_007057c0 (*(undefined *)0x007057c0u)
#define DAT_007057c4 (*(undefined *)0x007057c4u)
#define DAT_007057c8 (*(undefined *)0x007057c8u)
#define DAT_007057cc (*(undefined *)0x007057ccu)
#define DAT_007057d0 (*(undefined *)0x007057d0u)
#define DAT_00705ac4 (*(undefined *)0x00705ac4u)
#define DAT_0070e488 (*(undefined *)0x0070e488u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a58 (*(undefined4 *)0x00799a58u)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)

int FUN_00433ed0(int param_1)

{
  DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
  switch((&DAT_007999e8)[param_1 * 0x84]) {
  case 0x1e: ;
  case 0x1f: ;
  case 0x20: ;
  case 0x21: ;
  case 0x22: ;
  case 0x23: ;
  case 0x24: ;
  case 0x25: ;
  case 0xe2: ;
    break;
default: ;
    if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) == '\0') {
      return 0;
    }
    if (DAT_007057d0 + DAT_007057cc + DAT_007057c8 + DAT_007057c4 + DAT_007057c0 + DAT_00705ac4 +
        DAT_007057bc + DAT_007057b8 + DAT_007057b4 != 0) {
      return 10 - (int)(char)(&DAT_00799a58)[param_1 * 0x108] / 10;
    }
  }
  return 0;
}
