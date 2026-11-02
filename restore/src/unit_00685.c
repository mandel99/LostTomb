#include "restore_types.h"
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define _DAT_00784c60 (*(undefined4 *)0x00784c60u)
#define _DAT_00784c62 (*(undefined4 *)0x00784c62u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
extern uint FUN_004487f0();

void FUN_00448a90(void)

{
  undefined2 *puVar1;
  uint uVar2;
  int in_ECX;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int local_4;
  
  *(uint *)(in_ECX + 0x28) = *(uint *)(in_ECX + 0x28) & 0xfffffffb;
  FUN_004487f0();
  FUN_004487f0();
  FUN_004487f0();
  FUN_004487f0();
  FUN_004487f0();
  *(undefined1 *)(in_ECX + 0x57) = 0;
  *(undefined2 *)(in_ECX + 0x30) = 0;
  *(ushort *)(in_ECX + 0x2e) = (ushort)*(byte *)(in_ECX + 3) + *(short *)(in_ECX + 0x14);
  *(undefined2 *)(in_ECX + 0x34) = 0xffff;
  *(undefined2 *)(in_ECX + 100) = 0xffff;
  *(undefined1 *)(in_ECX + 0x32) = 0;
  *(undefined1 *)(in_ECX + 0x33) = 0;
  *(undefined1 *)(in_ECX + 99) = 0;
  if (*(char *)(in_ECX + 2) == '\x03') {
    puVar1 = (undefined2 *)(in_ECX + 0x42);
    iVar3 = 4;
    do {
      puVar1[-1] = _DAT_00784c60;
      *puVar1 = _DAT_00784c62;
      puVar1 = puVar1 + 2;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
  }
  (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
  DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
  if (99 < DAT_00d3b1c4) {
    DAT_00d3b1c4 = 0;
  }
  local_4 = 0x1f;
  do {
    uVar2 = DAT_00d3b384 >> 4;
    uVar4 = DAT_00d3b384 & 1;
    uVar5 = DAT_00d3b380 >> 4;
    uVar6 = DAT_00d3b380 & 1;
    DAT_00d3b384 = DAT_00d3b384 >> 1;
    DAT_00d3b380 = DAT_00d3b380 >> 1;
    if ((uVar2 & 1) != uVar4) {
      DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
    }
    if ((uVar5 & 1) != uVar6) {
      DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
    }
    local_4 = local_4 + -1;
  } while (local_4 != 0);
  DAT_00d3b374 = DAT_00d3b384 & 0x7f;
  *(char *)(in_ECX + 0x61) = (char)((ulonglong)(DAT_00d3b384 & 0x7fff) % 5) + '\x04';
  (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
  DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
  if (99 < DAT_00d3b1c4) {
    DAT_00d3b1c4 = 0;
  }
  iVar3 = 0x1f;
  do {
    uVar2 = DAT_00d3b384 >> 4;
    uVar5 = DAT_00d3b384 & 1;
    uVar6 = DAT_00d3b380 >> 4;
    uVar4 = DAT_00d3b380 & 1;
    DAT_00d3b384 = DAT_00d3b384 >> 1;
    DAT_00d3b380 = DAT_00d3b380 >> 1;
    if ((uVar2 & 1) != uVar5) {
      DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
    }
    if ((uVar6 & 1) != uVar4) {
      DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
    }
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  _DAT_00d3b378 = DAT_00d3b384 & 7;
  _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
  _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
  DAT_00d3b374 = DAT_00d3b384 & 0x7f;
  _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
  _DAT_00d3b36c = DAT_00d3b380 & 7;
  *(byte *)(in_ECX + 0x62) = (byte)DAT_00d3b384 & 1;
  return;
}
