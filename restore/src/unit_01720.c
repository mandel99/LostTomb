#include "restore_types.h"
#define DAT_00d3b4e8 (*(undefined *)0x00d3b4e8u)
#define DAT_00d3b4f0 (*(undefined *)0x00d3b4f0u)

void FUN_004cd850(void)

{
  DAT_00d3b4e8 = DAT_00d3b4e8 + 1;
  if (3 < DAT_00d3b4e8) {
    DAT_00d3b4e8 = 0;
    if (DAT_00d3b4f0 == 0xf800) {
      DAT_00d3b4f0 = 0;
      return;
    }
    DAT_00d3b4f0 = (-(uint)(DAT_00d3b4f0 != 0) & 0xfffff801) + 0xffff;
  }
  return;
}
