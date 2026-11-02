#include "restore_types.h"
#define DAT_0070e124 (*(undefined *)0x0070e124u)
#define DAT_0070e128 (*(undefined *)0x0070e128u)
#define DAT_0070e12c (*(undefined *)0x0070e12cu)
#define DAT_0070e130 (*(undefined *)0x0070e130u)
#define DAT_0070e134 (*(undefined *)0x0070e134u)
#define DAT_0070e138 (*(undefined *)0x0070e138u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
extern void FUN_004e0cf0();

void FUN_004302b0(void)

{
  if (DAT_0070e138 != 0) {
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x147,(int)(short)(&DAT_007999e8)[DAT_0070e12c * 0x84],
                 *(undefined4 *)(&DAT_007999e4 + DAT_0070e12c * 0x108),0,0);
    DAT_0070e138 = 0;
  }
  if (DAT_0070e134 != 0) {
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x146,(int)(short)(&DAT_007999e8)[DAT_0070e128 * 0x84],
                 *(undefined4 *)(&DAT_007999e4 + DAT_0070e128 * 0x108),0,0);
    DAT_0070e134 = 0;
  }
  if (DAT_0070e130 != 0) {
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x148,(int)(short)(&DAT_007999e8)[DAT_0070e124 * 0x84],
                 *(undefined4 *)(&DAT_007999e4 + DAT_0070e124 * 0x108),0,0);
    DAT_0070e130 = 0;
  }
  return;
}
