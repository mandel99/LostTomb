#include "restore_types.h"
extern undefined4 FUN_004f6700();

void FUN_004f6790(undefined1 *param_1)

{
  undefined1 *in_ECX;
  uint uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  
  FUN_004f6700(*(undefined4 *)(param_1 + 0x10));
  *(undefined4 *)(in_ECX + 0xc) = *(undefined4 *)(param_1 + 0xc);
  *(undefined4 *)(in_ECX + 4) = *(undefined4 *)(param_1 + 4);
  *(undefined4 *)(in_ECX + 8) = *(undefined4 *)(param_1 + 8);
  *in_ECX = *param_1;
  puVar3 = *(undefined4 **)(param_1 + 0x14);
  puVar4 = *(undefined4 **)(in_ECX + 0x14);
  for (uVar1 = *(uint *)(in_ECX + 0x10) & 0x3fffffff; uVar1 != 0; uVar1 = uVar1 - 1) {
    *puVar4 = *puVar3;
    puVar3 = puVar3 + 1;
    puVar4 = puVar4 + 1;
  }
  for (iVar2 = 0; iVar2 != 0; iVar2 = iVar2 + -1) {
    *(undefined1 *)puVar4 = *(undefined1 *)puVar3;
    puVar3 = (undefined4 *)((int)puVar3 + 1);
    puVar4 = (undefined4 *)((int)puVar4 + 1);
  }
  return;
}
