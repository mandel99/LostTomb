#include "restore_types.h"
#define DAT_00bcd8ae (*(undefined4 *)0x00bcd8aeu)

void FUN_004d32d0(void)

{
  undefined2 *puVar1;
  
  puVar1 = &DAT_00bcd8ae;
  do {
    *(undefined1 *)(puVar1 + -0x98) = 0;
    *puVar1 = 0xffff;
    puVar1 = puVar1 + 0xc2;
  } while ((int)puVar1 < 0xc8ae6a);
  return;
}
