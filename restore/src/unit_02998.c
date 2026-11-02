#include "restore_types.h"
#define DAT_01266710 (*(undefined *)0x01266710u)
#define DAT_01266720 (*(undefined4 *)0x01266720u)
#define DAT_01266724 (*(undefined4 *)0x01266724u)
#define DAT_01266728 (*(undefined4 *)0x01266728u)
#define DAT_0126672c (*(undefined *)0x0126672cu)
#define DAT_01266840 (*(undefined4 *)0x01266840u)
#define DAT_01266944 (*(undefined *)0x01266944u)

void FUN_00569a47(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = &DAT_01266840;
  for (iVar1 = 0x40; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  *(undefined1 *)puVar2 = 0;
  DAT_01266710 = 0;
  DAT_0126672c = 0;
  DAT_01266944 = 0;
  DAT_01266720 = 0;
  DAT_01266724 = 0;
  DAT_01266728 = 0;
  return;
}
