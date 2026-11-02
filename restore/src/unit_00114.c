#include "restore_types.h"
#define DAT_00605a6c (*(undefined4 *)0x00605a6cu)
#define DAT_00605b38 (*(undefined4 *)0x00605b38u)

void FUN_0040f250(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = &DAT_00605a6c;
  for (iVar1 = 0x33; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  puVar2 = (undefined4 *)&DAT_00605b38;
  for (iVar1 = 0xc; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  *(undefined2 *)puVar2 = 0;
  *(undefined1 *)((int)puVar2 + 2) = 0;
  return;
}
