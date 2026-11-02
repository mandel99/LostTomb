#include "restore_types.h"
#define DAT_0077de24 (*(undefined4 *)0x0077de24u)

void FUN_004d18f0(void)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 local_6c;
  undefined4 local_66 [7];
  undefined4 local_48;
  undefined2 local_44;
  undefined4 local_42 [7];
  undefined4 local_24;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_1c;
  undefined2 local_1a;
  undefined2 local_18;
  undefined2 local_16;
  undefined1 local_14;
  undefined1 local_13;
  undefined4 local_12;
  undefined2 local_e;
  undefined4 local_c;
  undefined4 local_8;
  undefined2 local_4;
  
  puVar2 = &DAT_0077de24;
  do {
    puVar4 = &local_6c;
    for (iVar1 = 0x1a; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar4 = 0;
      puVar4 = puVar4 + 1;
    }
    *(undefined2 *)puVar4 = 0;
    puVar4 = puVar2 + -9;
    puVar3 = &local_6c;
    for (iVar1 = 0x17; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar3 = *puVar4;
      puVar4 = puVar4 + 1;
      puVar3 = puVar3 + 1;
    }
    *(undefined2 *)puVar3 = *(undefined2 *)puVar4;
    puVar4 = (undefined4 *)((int)puVar2 + -0x1e);
    puVar3 = local_66;
    for (iVar1 = 7; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar3 = *puVar4;
      puVar4 = puVar4 + 1;
      puVar3 = puVar3 + 1;
    }
    *(undefined2 *)puVar3 = *(undefined2 *)puVar4;
    local_48 = 0;
    local_44 = 0;
    puVar4 = puVar2;
    puVar3 = local_42;
    for (iVar1 = 7; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar3 = *puVar4;
      puVar4 = puVar4 + 1;
      puVar3 = puVar3 + 1;
    }
    *(undefined2 *)puVar3 = *(undefined2 *)puVar4;
    local_24 = 0;
    local_20 = 0;
    local_1e = *(undefined2 *)((int)puVar2 + 0x1e);
    local_1c = *(undefined2 *)(puVar2 + 8);
    local_1a = *(undefined2 *)((int)puVar2 + 0x22);
    local_18 = *(undefined2 *)(puVar2 + 9);
    local_16 = *(undefined2 *)((int)puVar2 + 0x26);
    local_14 = *(undefined1 *)(puVar2 + 10);
    local_13 = *(undefined1 *)((int)puVar2 + 0x29);
    local_12 = *(undefined4 *)((int)puVar2 + 0x2a);
    local_e = *(undefined2 *)((int)puVar2 + 0x2e);
    puVar3 = (undefined4 *)((int)puVar2 + 0x6a);
    local_c = puVar2[0xc];
    local_8 = puVar2[0xd];
    local_4 = *(undefined2 *)(puVar2 + 0xe);
    puVar4 = &local_6c;
    puVar2 = puVar2 + -9;
    for (iVar1 = 0x1a; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar2 = *puVar4;
      puVar4 = puVar4 + 1;
      puVar2 = puVar2 + 1;
    }
    *(undefined2 *)puVar2 = *(undefined2 *)puVar4;
    puVar2 = puVar3;
  } while ((int)puVar3 < 0x77f766);
  return;
}
