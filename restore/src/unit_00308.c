#include "restore_types.h"
#define DAT_00605a3c (*(int32_t *)0x00605a3cu)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
extern void FUN_004cb6a0();

void FUN_0041f650(void)

{
  if (DAT_00605a3c != 0) {
    DAT_00e38dda = 0;
    DAT_00e38dca = 1;
    return;
  }
  FUN_004cb6a0();
  return;
}
