#include "restore_types.h"
#define DAT_00bcd603 (*(undefined4 *)0x00bcd603u)
extern void FUN_004b4760();
extern int FUN_004b4800();
extern void FUN_004b8c90();

void FUN_004bb090(void)

{
  char cVar1;
  int iVar2;
  int iVar3;
  
  FUN_004b4760();
  FUN_004b8c90();
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 1;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 2;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 3;
  FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0xc;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0xd;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0xe;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0xf;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x10;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x11;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x12;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x13;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x14;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x15;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x16;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x17;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x18;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x19;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x1a;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x1b;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x1c;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x1d;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x1e;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x1f;
  iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
  iVar3 = 0;
  (&DAT_00bcd603)[iVar2 * 0x184] = 0x20;
  do {
    cVar1 = (char)iVar3;
    iVar2 = FUN_004b4800(1,0x24,0xffffffff,0xffffffff,0);
    iVar3 = iVar3 + 1;
    (&DAT_00bcd603)[iVar2 * 0x184] = cVar1 + '2';
  } while (iVar3 < 0x10);
  return;
}
