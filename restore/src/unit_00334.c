#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)

undefined1 FUN_00420500(uint param_1)

{
  byte *pbVar1;
  
  pbVar1 = &DAT_00bcd786;
  while (((pbVar1[4] != 1 || (*pbVar1 != param_1)) ||
         ((&DAT_007999e8)[*(short *)(&DAT_00b4e6fc + *(int *)(pbVar1 + 0x1e) * 2) * 0x84] != 0xd2)))
  {
    pbVar1 = pbVar1 + 0x184;
    if (0xc8ad41 < (int)pbVar1) {
      return 0;
    }
  }
  return 1;
}
