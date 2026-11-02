#include "restore_types.h"
#define DAT_00ea5b40 (*(undefined4 *)0x00ea5b40u)
#define DAT_00ea5ca8 (*(undefined4 *)0x00ea5ca8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_0046e840(void)

{
  int iVar1;
  int iVar2;
  
  iVar2 = DAT_00eaac28;
  iVar1 = (&DAT_00ea5b40)[DAT_00eaac28 * 0x1276];
  (&DAT_00ea5b40)[DAT_00eaac28 * 0x1276] = iVar1 + -3;
  if (iVar1 + -3 < 0) {
    (&DAT_00ea5b40)[iVar2 * 0x1276] = 0;
  }
  (&DAT_00ea5ca8)[iVar2 * 0x1276] = 8;
  return;
}
