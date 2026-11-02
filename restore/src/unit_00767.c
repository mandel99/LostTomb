#include "restore_types.h"
extern int FUN_004449d0();
extern undefined2 * FUN_0044c0d0();
extern int FUN_0044d750();

void FUN_0044e6f0(int param_1)

{
  undefined1 uVar1;
  undefined2 uVar2;
  int iVar3;
  int in_ECX;
  
  uVar2 = FUN_004449d0((int)*(short *)(in_ECX + 0x1c));
  *(undefined2 *)(param_1 + 6) = uVar2;
  *(undefined2 *)(param_1 + 0x12) = uVar2;
  *(undefined2 *)(param_1 + 0x18) = *(undefined2 *)(in_ECX + 0xc);
  *(undefined2 *)(param_1 + 0x1a) = *(undefined2 *)(in_ECX + 4);
  *(undefined1 *)(param_1 + 0x1d) = *(undefined1 *)(in_ECX + 0x36);
  iVar3 = FUN_0044c0d0((int)*(short *)(in_ECX + 0x34));
  if (iVar3 != 0) {
    if (-1 < *(short *)(in_ECX + 100)) {
      *(short *)(param_1 + 0x10) = *(short *)(in_ECX + 100);
    }
    if (*(char *)(in_ECX + 99) != '\0') {
      uVar2 = FUN_004449d0(*(char *)(in_ECX + 99));
      *(undefined2 *)(param_1 + 0x12) = uVar2;
    }
    *(undefined2 *)(param_1 + 0x18) = *(undefined2 *)(in_ECX + 0x68);
    *(undefined2 *)(param_1 + 0x1a) = *(undefined2 *)(in_ECX + 0x66);
    *(undefined1 *)(param_1 + 0x1d) = *(undefined1 *)(in_ECX + 0x37);
  }
  *(undefined2 *)(param_1 + 4) = 0xffff;
  *(undefined2 *)(param_1 + 8) = *(undefined2 *)(in_ECX + 0xc);
  *(undefined2 *)(param_1 + 10) = *(undefined2 *)(in_ECX + 4);
  *(undefined2 *)(param_1 + 0xc) = *(undefined2 *)(in_ECX + 0x2c);
  uVar2 = FUN_0044d750();
  *(undefined2 *)(param_1 + 0xe) = uVar2;
  *(undefined2 *)(param_1 + 0x14) = 0;
  uVar1 = *(undefined1 *)(in_ECX + 0x36);
  *(undefined2 *)(param_1 + 0x16) = 0;
  *(undefined1 *)(param_1 + 0x1c) = uVar1;
  return;
}
