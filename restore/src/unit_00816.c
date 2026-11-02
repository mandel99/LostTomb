#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799aa4 (*(undefined4 *)0x00799aa4u)

void FUN_00455c30(int param_1)

{
  short sVar1;
  
  (&DAT_00799a2c)[param_1 * 0x84] = 0;
  if (((&DAT_007999ea)[param_1 * 0x84] != 0) && ((&DAT_00799a14)[param_1 * 0x84] != 0)) {
    (&DAT_00799a14)[param_1 * 0x84] = (&DAT_00799a14)[param_1 * 0x84] + -100;
  }
  if ((*(short *)((int)&DAT_00799aa4 + param_1 * 0x108 + 2) != 0) &&
     (sVar1 = *(short *)(&DAT_00799aa4 + param_1 * 0x42), sVar1 != 0)) {
    if ((&DAT_007999e8)[param_1 * 0x84] == 0xcc) {
      *(short *)(&DAT_00799aa4 + param_1 * 0x42) = sVar1 + -0x19;
      return;
    }
    *(short *)(&DAT_00799aa4 + param_1 * 0x42) = sVar1 + -100;
  }
  return;
}
