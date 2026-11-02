#include "restore_types.h"
#define DAT_00e92dfc (*(uint32_t *)0x00e92dfcu)

void FUN_004cdf20(ushort *param_1,int param_2)

{
  ushort uVar1;
  
  if (DAT_00e92dfc == 2) {
    for (; 0 < param_2; param_2 = param_2 + -2) {
      uVar1 = *param_1;
      if (uVar1 != 0xf81f) {
        *param_1 = (uVar1 & 0xffe0) << 1 | uVar1 & 0x1f;
      }
      param_1 = param_1 + 1;
    }
  }
  return;
}
