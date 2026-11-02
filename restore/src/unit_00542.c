#include "restore_types.h"
#define DAT_0070e45c (*(undefined *)0x0070e45cu)
#define DAT_0070e488 (*(undefined *)0x0070e488u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_00799a5f (*(undefined4 *)0x00799a5fu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
#define DAT_00f56724 (*(undefined *)0x00f56724u)
#define DAT_00f572e4 (*(undefined *)0x00f572e4u)
extern int FUN_0053e590();

void FUN_004360a0(int param_1,int param_2,undefined4 param_3)

{
  DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
  param_1 = param_1 * 0x108;
  if (*(char *)((int)&DAT_007999dc + param_1 + 1) != '\0') {
    if ((&DAT_00799a5f)[param_1] == '\x01') {
      FUN_0053e590(DAT_00f56724,((byte)(&DAT_007999db)[param_1] - 1) * DAT_0070e45c + 0x12 + param_2
                   ,param_3);
      return;
    }
    if ((&DAT_00799a5f)[param_1] == '\x02') {
      FUN_0053e590(DAT_00f572e4,((byte)(&DAT_007999db)[param_1] - 1) * DAT_0070e45c + 0x12 + param_2
                   ,param_3);
    }
  }
  return;
}
