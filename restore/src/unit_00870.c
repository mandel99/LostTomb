#include "restore_types.h"
#define DAT_00ea5fc4 (*(undefined4 *)0x00ea5fc4u)
#define DAT_00ea5fc8 (*(undefined4 *)0x00ea5fc8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_0045a5a0(void)

{
  int iVar1;
  
  iVar1 = DAT_00eaac28 * 0x49d8;
  *(undefined4 *)(&DAT_00ea5fc4 + iVar1) = *(undefined4 *)(&DAT_00ea5fc8 + iVar1);
  *(undefined4 *)(&DAT_00ea5fc8 + iVar1) = 0;
  return;
}
