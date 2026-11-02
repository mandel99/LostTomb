#include "restore_types.h"
#define DAT_00d3b4f4 (*(undefined *)0x00d3b4f4u)
#define DAT_00e36c84 (*(undefined *)0x00e36c84u)

void FUN_004cd810(void)

{
  DAT_00e36c84 = DAT_00e36c84 + 1;
  if (0x13 < DAT_00e36c84) {
    DAT_00e36c84 = 0;
    if (DAT_00d3b4f4 == 0xf800) {
      DAT_00d3b4f4 = 0;
      return;
    }
    DAT_00d3b4f4 = (-(uint)(DAT_00d3b4f4 != 0) & 0xfffff801) + 0xffff;
  }
  return;
}
