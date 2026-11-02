#include "restore_types.h"
#define DAT_00f54670 (*(undefined4 *)0x00f54670u)
#define DAT_00f5467c (*(undefined4 *)0x00f5467cu)
#define DAT_00f54680 (*(undefined4 *)0x00f54680u)
extern undefined4 FUN_00539c60();

void FUN_00539cf0(int param_1,undefined4 param_2,int param_3)

{
  undefined4 uVar1;
  
  if (param_3 == 0) {
    uVar1 = FUN_00539c60(param_2);
    (&DAT_00f5467c)[param_1 * 5] = uVar1;
  }
  else if ((param_3 == 1) && ((int)(&DAT_00f54680)[param_1 * 5] < 3)) {
    uVar1 = FUN_00539c60(param_2);
    (&DAT_00f54670)[param_1 * 5 + (&DAT_00f54680)[param_1 * 5]] = uVar1;
    (&DAT_00f54680)[param_1 * 5] = (&DAT_00f54680)[param_1 * 5] + 1;
    return;
  }
  return;
}
