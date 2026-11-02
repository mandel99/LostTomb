#include "restore_types.h"
#define DAT_00bcbada (*(undefined4 *)0x00bcbadau)

void FUN_004d1840(void)

{
  undefined2 *puVar1;
  
  puVar1 = &DAT_00bcbada;
  do {
    *puVar1 = 0xffff;
    puVar1 = puVar1 + 0x48;
  } while ((int)puVar1 < 0xbcd66a);
  return;
}
