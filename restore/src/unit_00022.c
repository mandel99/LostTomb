#include "restore_types.h"
#define DAT_006059f4 (*(int32_t *)0x006059f4u)
#define DAT_006059f8 (*(int32_t *)0x006059f8u)
#define DAT_00d3b198 (*(int32_t *)0x00d3b198u)
#define DAT_00d3b1b3 (*(char *)0x00d3b1b3u)

void FUN_00402980(void)

{
  if (0xc < DAT_006059f8) {
    if (DAT_00d3b198 == 0) {
      if (0 < DAT_006059f4) {
        DAT_00d3b1b3 = 0;
        DAT_006059f4 = DAT_006059f4 + -1;
        return;
      }
    }
    else {
      if (DAT_00d3b198 == 1) {
        if (DAT_006059f8 <= DAT_006059f4 + 0xc) {
          return;
        }
        DAT_006059f4 = DAT_006059f4 + 1;
      }
      DAT_00d3b1b3 = 0;
    }
  }
  return;
}
