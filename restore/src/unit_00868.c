#include "restore_types.h"
#define DAT_00ea4500 (*(undefined4 *)0x00ea4500u)
#define DAT_00ea4510 (*(undefined4 *)0x00ea4510u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_0045a530(void)

{
  int iVar1;
  
  iVar1 = DAT_00eaac28;
  (&DAT_00ea4500)[DAT_00eaac28 * 0x1276] = (&DAT_00ea4510)[DAT_00eaac28 * 0x1276];
  (&DAT_00ea4510)[iVar1 * 0x1276] = 0;
  return;
}
