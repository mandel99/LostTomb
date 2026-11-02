#include "restore_types.h"
#define DAT_00ea5ba8 (*(undefined4 *)0x00ea5ba8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define PTR_DAT_005c2750 (*(unsigned char * *)0x005c2750u)

undefined1 FUN_00420550(undefined *param_1)

{
  undefined **ppuVar1;
  
  ppuVar1 = &PTR_DAT_005c2750;
  while ((((*(short *)(*ppuVar1 + 0x12) != 0 ||
           (ppuVar1[-2] !=
            (undefined *)(int)*(short *)((int)&DAT_00ea5ba8 + DAT_00eaac28 * 0x49d8 + 2))) ||
          (ppuVar1[-1] != param_1)) || (*(int *)(*ppuVar1 + 0xc) < 1))) {
    ppuVar1 = ppuVar1 + 4;
    if (0x5c28af < (int)ppuVar1) {
      return 0;
    }
  }
  return 1;
}
