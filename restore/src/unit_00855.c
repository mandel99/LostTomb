#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)

undefined4 FUN_00459ec0(int param_1)

{
  if ((0x4c < (short)(&DAT_007999e8)[param_1 * 0x84]) &&
     ((short)(&DAT_007999e8)[param_1 * 0x84] < 0x50)) {
    return 1;
  }
  return 0;
}
