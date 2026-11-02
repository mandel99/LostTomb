#include "restore_types.h"
#define DAT_005cd6c8 (*(undefined4 *)0x005cd6c8u)
#define DAT_005cd6d8 (*(undefined4 *)0x005cd6d8u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)

undefined4 FUN_00487e60(int param_1)

{
  if (((*(byte *)(&DAT_00af598c +
                 *(int *)(&DAT_005cd6c8 + (uint)*(byte *)(&DAT_00799a82 + param_1 * 0x84) * 4) +
                 *(int *)(&DAT_007999e4 + param_1 * 0x108)) & 4) == 0) &&
     ((*(byte *)(&DAT_00af598c +
                *(int *)(&DAT_005cd6d8 + (uint)*(byte *)(&DAT_00799a82 + param_1 * 0x84) * 4) +
                *(int *)(&DAT_007999e4 + param_1 * 0x108)) & 4) == 0)) {
    return 0;
  }
  return 1;
}
