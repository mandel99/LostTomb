#include "restore_types.h"
#define DAT_00ea44f0 (*(undefined4 *)0x00ea44f0u)
#define DAT_00ea44f4 (*(undefined4 *)0x00ea44f4u)
#define DAT_00ea5cfc (*(undefined4 *)0x00ea5cfcu)
#define DAT_00ea5ff4 (*(undefined4 *)0x00ea5ff4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_0045a560(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = DAT_00eaac28;
  iVar2 = DAT_00eaac28 * 0x49d8;
  (&DAT_00ea44f0)[DAT_00eaac28 * 0x1276] = (&DAT_00ea44f4)[DAT_00eaac28 * 0x1276];
  (&DAT_00ea44f4)[iVar1 * 0x1276] = 0;
  *(undefined2 *)((int)&DAT_00ea5cfc + iVar2 + 2) = *(undefined2 *)(&DAT_00ea5cfc + iVar1 * 0x1276);
  *(undefined2 *)(&DAT_00ea5cfc + iVar1 * 0x1276) = 0;
  *(undefined4 *)(&DAT_00ea5ff4 + iVar2) = 0;
  return;
}
