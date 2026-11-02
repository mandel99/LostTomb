#include "restore_types.h"

undefined4 FUN_004f6520(void)

{
  int iVar1;
  int in_ECX;
  undefined4 *puVar2;
  
  *(undefined4 *)(in_ECX + 4) = 0;
  *(undefined4 *)(in_ECX + 8) = 0;
  *(undefined4 *)(in_ECX + 0xc) = 0x32;
  iVar1 = 0;
  puVar2 = (undefined4 *)(in_ECX + 0x14);
  do {
    *puVar2 = 0;
    iVar1 = iVar1 + 1;
    puVar2 = puVar2 + 1;
  } while (iVar1 < *(int *)(in_ECX + 0xc));
  return 1;
}
