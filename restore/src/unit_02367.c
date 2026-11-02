#include "restore_types.h"
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)

void FUN_00521060(void)

{
  if (DAT_00f53f48 < 6) {
    DAT_00f53f48 = 6;
    DAT_00e38dca = 1;
  }
  return;
}
