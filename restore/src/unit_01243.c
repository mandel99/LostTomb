#include "restore_types.h"
#define DAT_00bc9688 (*(undefined *)0x00bc9688u)
#define DAT_00bc968c (*(undefined *)0x00bc968cu)
#define DAT_00bc9728 (*(undefined *)0x00bc9728u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define _DAT_00bc9684 (*(undefined4 *)0x00bc9684u)

void FUN_00485200(void)

{
  DAT_00bc968c = 1;
  DAT_00bc9688 = 500;
  _DAT_00bc9684 = DAT_00bc9728;
  DAT_00e38dca = 1;
  return;
}
