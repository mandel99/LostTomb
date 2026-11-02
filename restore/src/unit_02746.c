#include "restore_types.h"

uint FUN_005608cc(void)

{
  int iVar1;
  int in_ECX;
  byte *pbVar2;
  
  pbVar2 = *(byte **)(in_ECX + 0x28);
  if (*(byte **)(in_ECX + 0x2c) <= pbVar2) {
    if (*(byte **)(in_ECX + 0x2c) < *(byte **)(in_ECX + 0x1c)) {
      iVar1 = *(int *)(in_ECX + 0x24);
      *(int *)(in_ECX + 0x24) = *(int *)(in_ECX + 0x10);
      *(byte **)(in_ECX + 0x2c) = *(byte **)(in_ECX + 0x1c);
      *(undefined4 *)(in_ECX + 0xc) = 0xffffffff;
      *(byte **)(in_ECX + 0x28) = pbVar2 + (*(int *)(in_ECX + 0x10) - iVar1);
    }
    pbVar2 = *(byte **)(in_ECX + 0x28);
    if (*(byte **)(in_ECX + 0x2c) <= pbVar2) {
      return 0xffffffff;
    }
  }
  return (uint)*pbVar2;
}
