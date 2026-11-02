#include "restore_types.h"
#define DAT_00ea44d4 (*(undefined4 *)0x00ea44d4u)
#define DAT_00ea44d8 (*(undefined4 *)0x00ea44d8u)
#define DAT_00ea44dc (*(undefined4 *)0x00ea44dcu)
#define DAT_00ea44e0 (*(undefined4 *)0x00ea44e0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_0045a490(void)

{
  int iVar1;
  
  iVar1 = DAT_00eaac28;
  (&DAT_00ea44d8)[DAT_00eaac28 * 0x1276] = (&DAT_00ea44d4)[DAT_00eaac28 * 0x1276];
  (&DAT_00ea44d4)[iVar1 * 0x1276] = 0;
  (&DAT_00ea44e0)[iVar1 * 0x1276] = (&DAT_00ea44dc)[iVar1 * 0x1276];
  (&DAT_00ea44dc)[iVar1 * 0x1276] = 0;
  return;
}
