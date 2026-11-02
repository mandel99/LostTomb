#include "restore_types.h"
#define DAT_00ea60ac (*(undefined4 *)0x00ea60acu)

void FUN_004d1880(void)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 *puVar4;
  
  puVar1 = &DAT_00ea60ac;
  do {
    puVar2 = puVar1 + 0x1276;
    puVar4 = puVar1 + -1999;
    for (iVar3 = 7; iVar3 != 0; iVar3 = iVar3 + -1) {
      *puVar1 = *puVar4;
      puVar4 = puVar4 + 1;
      puVar1 = puVar1 + 1;
    }
    puVar1 = puVar2;
  } while ((int)puVar2 < 0xeaf45c);
  return;
}
