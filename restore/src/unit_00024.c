#include "restore_types.h"
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)

void FUN_00402bb0(void)

{
  DAT_00e38dda = 0;
  DAT_00e38dca = 1;
  return;
}
