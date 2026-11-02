#include "restore_types.h"
#define DAT_00605980 (*(undefined1 *)0x00605980u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)

void FUN_00403770(void)

{
  DAT_00e38dca = 1;
  DAT_00e38dda = DAT_00605980;
  return;
}
