#include "restore_types.h"

void FUN_0041bae0(void)

{
  int iVar1;
  int in_ECX;
  undefined1 *puVar2;
  
  iVar1 = 0;
  if (0 < *(int *)(in_ECX + 0x1900)) {
    puVar2 = (undefined1 *)(in_ECX + 0x3c);
    do {
      *puVar2 = 1;
      iVar1 = iVar1 + 1;
      puVar2 = puVar2 + 0x40;
    } while (iVar1 < *(int *)(in_ECX + 0x1900));
  }
  return;
}
