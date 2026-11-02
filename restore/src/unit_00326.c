#include "restore_types.h"
#define DAT_00ea5ba8 (*(undefined4 *)0x00ea5ba8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define PTR_DAT_005c2750 (*(unsigned char * *)0x005c2750u)
extern void FUN_0053d820();

void FUN_004200b0(void)

{
  undefined **ppuVar1;
  
  ppuVar1 = &PTR_DAT_005c2750;
  while ((ppuVar1[-2] !=
          (undefined *)(int)*(short *)((int)&DAT_00ea5ba8 + DAT_00eaac28 * 0x49d8 + 2) ||
         (*(int *)(*ppuVar1 + 0xc) < 1))) {
    ppuVar1 = ppuVar1 + 4;
    if (0x5c28af < (int)ppuVar1) {
      FUN_0053d820();
      return;
    }
  }
  return;
}
