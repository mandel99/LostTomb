#include "restore_types.h"
#define DAT_00ea44e4 (*(undefined4 *)0x00ea44e4u)
#define DAT_00ea44e8 (*(undefined4 *)0x00ea44e8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_0045a500(void)

{
  int iVar1;
  
  iVar1 = DAT_00eaac28;
  (&DAT_00ea44e8)[DAT_00eaac28 * 0x1276] = (&DAT_00ea44e4)[DAT_00eaac28 * 0x1276];
  (&DAT_00ea44e4)[iVar1 * 0x1276] = 0;
  return;
}
