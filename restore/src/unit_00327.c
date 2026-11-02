#include "restore_types.h"
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define PTR_DAT_00596088 (*(unsigned char * *)0x00596088u)
extern void FUN_00542dd0();

void FUN_004200f0(int param_1)

{
  undefined **ppuVar1;
  
  ppuVar1 = &PTR_DAT_00596088;
  while ((ppuVar1[-2] != (undefined *)(&DAT_00799a30)[param_1 * 0x42] ||
         (*(int *)(*ppuVar1 + 0xc) < 1))) {
    ppuVar1 = ppuVar1 + 4;
    if (0x596157 < (int)ppuVar1) {
      FUN_00542dd0(param_1);
      return;
    }
  }
  return;
}
