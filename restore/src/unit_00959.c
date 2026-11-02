#include "restore_types.h"
#define DAT_009783d8 (*(undefined4 *)0x009783d8u)

void FUN_0046a260(void)

{
  undefined4 *puVar1;
  
  puVar1 = &DAT_009783d8;
  do {
    *(undefined1 *)(puVar1 + -6) = 0;
    *puVar1 = 0;
    puVar1 = puVar1 + 7;
  } while ((int)puVar1 < 0x9784f0);
  return;
}
