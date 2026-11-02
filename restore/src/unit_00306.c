#include "restore_types.h"
#define DAT_00704db0 (*(int32_t *)0x00704db0u)
#define DAT_00704db4 (*(int32_t *)0x00704db4u)
#define DAT_00704db8 (*(int32_t *)0x00704db8u)
#define DAT_00d3b198 (*(int32_t *)0x00d3b198u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)

void FUN_0041f460(void)

{
  if ((int)((-(uint)(DAT_00704db0 != 1) & 2) + 0xd) < DAT_00704db4) {
    if (DAT_00d3b198 == 0) {
      if (0 < DAT_00704db8) {
        DAT_00e38dca = 1;
        DAT_00704db8 = DAT_00704db8 + -1;
        return;
      }
    }
    else {
      if (DAT_00d3b198 == 1) {
        if (DAT_00704db4 <= (int)(DAT_00704db8 + (-(uint)(DAT_00704db0 != 1) & 2) + 0xd)) {
          return;
        }
        DAT_00704db8 = DAT_00704db8 + 1;
      }
      DAT_00e38dca = 1;
    }
  }
  return;
}
