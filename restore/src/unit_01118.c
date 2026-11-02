#include "restore_types.h"
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00f565dc (*(undefined *)0x00f565dcu)

void FUN_0047c260(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(&DAT_007999e4 + param_1 * 0x108);
  (&DAT_00799a2c)[param_1 * 0x84] = (&DAT_00799a2c)[param_1 * 0x84] + 1;
  if (4 < (short)(&DAT_00799a2c)[param_1 * 0x84]) {
    (&DAT_00799a2c)[param_1 * 0x84] = 0;
  }
  (&DAT_00b7482c)[iVar1] = (short)(&DAT_00799a2c)[param_1 * 0x84] + DAT_00f565dc;
  return;
}
