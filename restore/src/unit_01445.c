#include "restore_types.h"
#define DAT_00bcd5f0 (*(undefined *)0x00bcd5f0u)
#define DAT_00bcd5f8 (*(undefined4 *)0x00bcd5f8u)
#define _DAT_00bcd5f4 (*(undefined4 *)0x00bcd5f4u)
extern void FUN_0040f8f0();

void FUN_004b4760(void)

{
  undefined *puVar1;
  
  puVar1 = &DAT_00bcd5f8;
  do {
    FUN_0040f8f0(puVar1,0x184,0);
    puVar1 = puVar1 + 0x184;
  } while ((int)puVar1 < 0xc8ad38);
  DAT_00bcd5f0 = 0;
  _DAT_00bcd5f4 = 0;
  return;
}
