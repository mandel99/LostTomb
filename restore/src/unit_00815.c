#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)

bool FUN_00455bf0(int param_1)

{
  return (int)((-(uint)((&DAT_007999ea)[param_1 * 0x84] != 0) & 200) + 200) <=
         (int)(short)(&DAT_00799a2c)[param_1 * 0x84];
}
