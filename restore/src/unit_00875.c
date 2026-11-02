#include "restore_types.h"
#define DAT_00ea4504 (*(undefined4 *)0x00ea4504u)
#define DAT_00ea4510 (*(undefined4 *)0x00ea4510u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_0045a970(void)

{
  (&DAT_00ea4504)[DAT_00eaac28 * 0x1276] = (&DAT_00ea4510)[DAT_00eaac28 * 0x1276];
  return;
}
