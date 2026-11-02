#include "restore_types.h"
#define DAT_00ee0c50 (*(undefined4 *)0x00ee0c50u)

void FUN_00517a20(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = &DAT_00ee0c50;
  for (iVar1 = 0x32c4; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0xffffffff;
    puVar2 = puVar2 + 1;
  }
  return;
}
