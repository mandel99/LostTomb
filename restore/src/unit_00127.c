#include "restore_types.h"
#define DAT_00d3b4f0 (*(undefined *)0x00d3b4f0u)
#define DAT_00e38d94 (*(undefined *)0x00e38d94u)
#define DAT_00e38d98 (*(undefined *)0x00e38d98u)
#define DAT_00e38d9c (*(undefined *)0x00e38d9cu)
#define DAT_00e92e18 (*(undefined *)0x00e92e18u)

void FUN_0040fb10(int param_1,int param_2)

{
  if ((((-1 < param_1) && (param_1 < DAT_00e38d9c)) && (-1 < param_2)) &&
     (param_2 < DAT_00e38d98 + -0x20)) {
    *(short *)(DAT_00e92e18 + param_1 * 2 + param_2 * DAT_00e38d94) = (short)DAT_00d3b4f0;
  }
  return;
}
