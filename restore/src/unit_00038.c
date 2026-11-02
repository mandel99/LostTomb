#include "restore_types.h"
#define DAT_0070a8dc (*(undefined4 *)0x0070a8dcu)

void FUN_00404330(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = &DAT_0070a8dc;
  for (iVar1 = 0x474; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 1;
    puVar2 = puVar2 + 1;
  }
  return;
}
