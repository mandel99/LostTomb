#include "restore_types.h"
#define DAT_00bcb9b4 (*(undefined *)0x00bcb9b4u)
#define DAT_00bcb9bc (*(undefined *)0x00bcb9bcu)
#define DAT_00bcb9c8 (*(undefined4 *)0x00bcb9c8u)
#define _DAT_00bcb9b8 (*(undefined4 *)0x00bcb9b8u)
extern void FUN_0040f8f0();

void FUN_004b8290(void)

{
  undefined1 *puVar1;
  
  puVar1 = &DAT_00bcb9c8;
  do {
    FUN_0040f8f0(puVar1,0x90,0);
    puVar1 = puVar1 + 0x90;
  } while ((int)puVar1 < 0xbcd5e8);
  DAT_00bcb9bc = 0;
  _DAT_00bcb9b8 = 0;
  DAT_00bcb9b4 = 0;
  return;
}
