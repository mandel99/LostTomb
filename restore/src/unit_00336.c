#include "restore_types.h"
#define PTR_DAT_00596088 (*(unsigned char * *)0x00596088u)

undefined1 FUN_004205b0(undefined *param_1,undefined *param_2)

{
  undefined **ppuVar1;
  
  ppuVar1 = &PTR_DAT_00596088;
  while ((((*(short *)(*ppuVar1 + 0x12) != 0 || (ppuVar1[-2] != param_2)) ||
          (ppuVar1[-1] != param_1)) || (*(int *)(*ppuVar1 + 0xc) < 1))) {
    ppuVar1 = ppuVar1 + 4;
    if (0x596157 < (int)ppuVar1) {
      return 0;
    }
  }
  return 1;
}
