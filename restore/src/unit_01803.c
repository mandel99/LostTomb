#include "restore_types.h"
#define DAT_00799a6a (*(undefined4 *)0x00799a6au)

void FUN_004d2fa0(void)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 local_108 [4];
  short local_f8;
  undefined1 local_b4 [2];
  undefined4 local_b2 [15];
  undefined4 local_76;
  undefined4 local_72;
  undefined4 local_6e;
  undefined4 local_6a;
  undefined4 local_66;
  undefined1 local_62;
  undefined1 local_61;
  undefined1 local_60;
  undefined1 local_5f;
  undefined1 local_5e;
  undefined1 local_5c;
  undefined1 local_5b;
  undefined4 local_5a;
  undefined2 local_56;
  undefined2 local_54;
  undefined2 local_52;
  undefined4 local_50;
  undefined1 local_4c;
  undefined1 local_4b;
  undefined1 local_4a;
  undefined1 local_49;
  undefined1 local_48;
  undefined1 local_47;
  undefined1 local_46;
  undefined1 local_45;
  undefined4 local_44;
  undefined2 local_40;
  undefined2 local_3c;
  undefined2 local_3a;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  
  puVar2 = &DAT_00799a6a;
  do {
    puVar5 = (undefined4 *)((int)puVar2 + -0x92);
    puVar3 = local_108;
    for (iVar1 = 0x42; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar3 = 0;
      puVar3 = puVar3 + 1;
    }
    puVar3 = puVar5;
    puVar4 = local_108;
    for (iVar1 = 0x3d; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar4 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar4 = puVar4 + 1;
    }
    if ((local_f8 < 10) || (0x1d < local_f8)) {
      if ((local_f8 == 0xb7) || (local_f8 == 0xd2)) {
        puVar3 = (undefined4 *)((int)puVar2 + -0x3e);
        iVar1 = 0xc;
        puVar4 = (undefined4 *)local_b4;
        goto LAB_004d30c6;
      }
      puVar3 = puVar2 + -0xf;
      puVar4 = (undefined4 *)(local_b4 + 2);
      for (iVar1 = 0xf; iVar1 != 0; iVar1 = iVar1 + -1) {
        *puVar4 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar4 = puVar4 + 1;
      }
      local_76 = 0;
      local_72 = 0;
      local_6e = 0;
      local_6a = *puVar2;
      local_62 = *(undefined1 *)(puVar2 + 2);
      local_66 = puVar2[1];
      local_60 = *(undefined1 *)((int)puVar2 + 10);
      local_61 = *(undefined1 *)((int)puVar2 + 9);
      local_5e = *(undefined1 *)(puVar2 + 3);
      local_5f = *(undefined1 *)((int)puVar2 + 0xb);
      local_5b = *(undefined1 *)((int)puVar2 + 0xf);
      local_5c = *(undefined1 *)((int)puVar2 + 0xe);
      local_5a = puVar2[4];
      local_54 = *(undefined2 *)((int)puVar2 + 0x16);
      local_56 = *(undefined2 *)(puVar2 + 5);
      local_52 = *(undefined2 *)(puVar2 + 6);
    }
    else {
      iVar1 = 0xf;
      puVar3 = (undefined4 *)((int)puVar2 + -0x3e);
      puVar4 = (undefined4 *)local_b4;
LAB_004d30c6: ;
      for (; iVar1 != 0; iVar1 = iVar1 + -1) {
        *puVar4 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar4 = puVar4 + 1;
      }
    }
    local_50 = *(undefined4 *)((int)puVar2 + 0x1a);
    local_4c = *(undefined1 *)((int)puVar2 + 0x1e);
    local_4b = *(undefined1 *)((int)puVar2 + 0x1f);
    local_4a = *(undefined1 *)(puVar2 + 8);
    local_49 = *(undefined1 *)((int)puVar2 + 0x21);
    local_48 = *(undefined1 *)((int)puVar2 + 0x22);
    local_47 = *(undefined1 *)((int)puVar2 + 0x23);
    local_46 = *(undefined1 *)(puVar2 + 9);
    local_45 = *(undefined1 *)((int)puVar2 + 0x25);
    local_44 = *(undefined4 *)((int)puVar2 + 0x26);
    local_40 = *(undefined2 *)((int)puVar2 + 0x2a);
    local_3c = *(undefined2 *)((int)puVar2 + 0x2e);
    local_3a = *(undefined2 *)(puVar2 + 0xc);
    local_38 = *(undefined4 *)((int)puVar2 + 0x32);
    local_34 = *(undefined4 *)((int)puVar2 + 0x36);
    local_30 = *(undefined4 *)((int)puVar2 + 0x3a);
    local_2c = *(undefined4 *)((int)puVar2 + 0x3e);
    local_28 = *(undefined4 *)((int)puVar2 + 0x42);
    local_24 = *(undefined4 *)((int)puVar2 + 0x46);
    local_20 = *(undefined4 *)((int)puVar2 + 0x4a);
    local_1c = *(undefined4 *)((int)puVar2 + 0x4e);
    local_18 = *(undefined4 *)((int)puVar2 + 0x52);
    local_14 = *(undefined4 *)((int)puVar2 + 0x56);
    puVar3 = (undefined4 *)((int)puVar2 + 0x5e);
    local_10 = *(undefined4 *)((int)puVar2 + 0x5a);
    puVar2 = puVar2 + 0x42;
    local_c = *puVar3;
    puVar3 = local_108;
    for (iVar1 = 0x42; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    if (0x89b769 < (int)puVar2) {
      return;
    }
  } while( true );
}
