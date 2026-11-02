#include "restore_types.h"
#define DAT_00ea44f8 (*(undefined4 *)0x00ea44f8u)
#define DAT_00ea44fc (*(undefined4 *)0x00ea44fcu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_0045a4d0(void)

{
  int iVar1;
  
  iVar1 = DAT_00eaac28;
  (&DAT_00ea44f8)[DAT_00eaac28 * 0x1276] = (&DAT_00ea44fc)[DAT_00eaac28 * 0x1276];
  (&DAT_00ea44fc)[iVar1 * 0x1276] = 0;
  return;
}
