#include "restore_types.h"
#define DAT_00d3b198 (*(int32_t *)0x00d3b198u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00eadfb0 (*(int32_t *)0x00eadfb0u)
#define DAT_00eadfc8 (*(int32_t *)0x00eadfc8u)

void FUN_00516890(void)

{
  if (0xc < DAT_00eadfb0) {
    if (DAT_00d3b198 == 0) {
      if (0 < DAT_00eadfc8) {
        DAT_00e38dca = 2;
        DAT_00eadfc8 = DAT_00eadfc8 + -1;
        return;
      }
    }
    else {
      if (DAT_00d3b198 == 1) {
        if (DAT_00eadfb0 <= DAT_00eadfc8 + 0xc) {
          return;
        }
        DAT_00eadfc8 = DAT_00eadfc8 + 1;
      }
      DAT_00e38dca = 2;
    }
  }
  return;
}
