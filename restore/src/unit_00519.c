#include "restore_types.h"
#define DAT_0070e488 (*(undefined *)0x0070e488u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_00799ad4 (*(undefined4 *)0x00799ad4u)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
extern void FUN_00436180();

void FUN_004354b0(int param_1,undefined4 param_2,undefined4 param_3)

{
  DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) != '\0') {
    FUN_00436180(*(int *)(&DAT_00799ad4 + param_1 * 0x108) / 100,param_2,param_3,1);
  }
  return;
}
