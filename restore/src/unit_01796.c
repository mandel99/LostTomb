#include "restore_types.h"
#define DAT_00e9523a (*(undefined4 *)0x00e9523au)

void FUN_004d1860(void)

{
  undefined2 *puVar1;
  
  puVar1 = &DAT_00e9523a;
  do {
    puVar1[-1] = 0xffff;
    *puVar1 = 0xffff;
    puVar1[1] = 0xffff;
    puVar1 = puVar1 + 0x18;
  } while ((int)puVar1 < 0xea0dba);
  return;
}
