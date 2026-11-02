#include "restore_types.h"
#define DAT_00e20998 (*(undefined *)0x00e20998u)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define _DAT_007027e8 (*(undefined4 *)0x007027e8u)

void FUN_0041d3d0(void)

{
  DAT_00e20998 = 0;
  _DAT_007027e8 = (uint)DAT_00e38dda;
  DAT_00e38dda = 0x55;
  return;
}
