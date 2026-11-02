#include "restore_types.h"
#define DAT_0070e488 (*(undefined *)0x0070e488u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_00799a4e (*(undefined4 *)0x00799a4eu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
extern void FUN_004361a0();

void FUN_00435c40(int param_1,undefined4 param_2,undefined4 param_3)

{
  DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) != '\0') {
    FUN_004361a0((byte)(&DAT_00799a4e)[param_1 * 0x108] / 10,param_2,param_3,0);
  }
  return;
}
