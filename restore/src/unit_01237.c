#include "restore_types.h"
#define DAT_00bc97c0 (*(undefined *)0x00bc97c0u)

void FUN_00484f20(void)

{
  uint *puVar1;
  
  puVar1 = &DAT_00bc97c0;
  do {
    *puVar1 = (uint)(*puVar1 != 0);
    puVar1 = puVar1 + 1;
  } while ((int)puVar1 < 0xbc97e0);
  return;
}
