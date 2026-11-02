#include "restore_types.h"
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e94d98 (*(undefined *)0x00e94d98u)

void FUN_004d6070(void)

{
  if (DAT_00e94d98 != 0) {
    DAT_00e38dca = 1;
  }
  return;
}
