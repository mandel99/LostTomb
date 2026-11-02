#include "restore_types.h"
#define DAT_00c8ad38 (*(undefined4 *)0x00c8ad38u)
#define DAT_00d04e58 (*(undefined4 *)0x00d04e58u)

void FUN_004b32a0(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = &DAT_00c8ad38;
  for (iVar1 = 0x1e848; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  puVar2 = &DAT_00d04e58;
  for (iVar1 = 500; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  return;
}
