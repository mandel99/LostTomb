#include "restore_types.h"
#define LAB_004f6670 (*(undefined1 *)0x004f6670u)
#define DAT_0097844c (*(undefined4 *)0x0097844cu)
extern void FUN_004f6690();
extern void FUN_004f66f0();
extern void FUN_00561441();

void FUN_00451a70(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  FUN_004f6690(0x14);
  FUN_004f6690(0x14);
  FUN_004f6690(0x14);
  FUN_004f6690(0x14);
  FUN_004f6690(0x14);
  puVar2 = &DAT_0097844c;
  for (iVar1 = 0x23; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  FUN_00561441(&DAT_0097844c,0x1c,5,&LAB_004f6670,FUN_004f66f0);
  return;
}
