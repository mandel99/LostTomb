#include "restore_types.h"
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00e38db0 (*(undefined *)0x00e38db0u)
#define DAT_00f5697c (*(undefined *)0x00f5697cu)
extern int FUN_0053e590();

void FUN_0052d630(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = DAT_00e38db0 / 0x46;
  iVar1 = DAT_00e38db0 + -0x46;
  if (0 < iVar2) {
    iVar3 = 0;
    do {
      FUN_0053e590(DAT_00f5697c + 3,iVar3,DAT_00e38dac + -0x78);
      FUN_0053e590(DAT_00f5697c + 3,iVar3,DAT_00e38dac + -0x50);
      FUN_0053e590(DAT_00f5697c + 3,iVar3,DAT_00e38dac + -0x28);
      iVar3 = iVar3 + 0x46;
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
  }
  FUN_0053e590(DAT_00f5697c + 3,iVar1,DAT_00e38dac + -0x78);
  FUN_0053e590(DAT_00f5697c + 3,iVar1,DAT_00e38dac + -0x50);
  FUN_0053e590(DAT_00f5697c + 3,iVar1,DAT_00e38dac + -0x28);
  iVar2 = DAT_00e38db0 / 0x56;
  iVar1 = DAT_00e38db0 + -0x56;
  if (0 < iVar2) {
    iVar3 = 0;
    do {
      FUN_0053e590(DAT_00f5697c + 1,iVar3,0);
      FUN_0053e590(DAT_00f5697c + 1,iVar3,DAT_00e38dac + -0x82);
      FUN_0053e590(DAT_00f5697c + 1,iVar3,DAT_00e38dac + -0x10);
      iVar3 = iVar3 + 0x56;
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
  }
  FUN_0053e590(DAT_00f5697c + 1,iVar1,0);
  FUN_0053e590(DAT_00f5697c + 1,iVar1,DAT_00e38dac + -0x82);
  FUN_0053e590(DAT_00f5697c + 1,iVar1,DAT_00e38dac + -0x10);
  iVar2 = DAT_00e38dac / 0x56;
  iVar1 = DAT_00e38dac + -0x56;
  if (0 < iVar2) {
    iVar3 = 0x10;
    do {
      FUN_0053e590(DAT_00f5697c,0,iVar3);
      FUN_0053e590(DAT_00f5697c,DAT_00e38db0 + -0x10,iVar3);
      iVar3 = iVar3 + 0x56;
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
  }
  FUN_0053e590(DAT_00f5697c,0,iVar1);
  FUN_0053e590(DAT_00f5697c,DAT_00e38db0 + -0x10,iVar1);
  FUN_0053e590(DAT_00f5697c + 2,0,0);
  FUN_0053e590(DAT_00f5697c + 2,0,DAT_00e38dac + -0x82);
  FUN_0053e590(DAT_00f5697c + 2,0,DAT_00e38dac + -0x10);
  FUN_0053e590(DAT_00f5697c + 2,DAT_00e38db0 + -0x10,0);
  FUN_0053e590(DAT_00f5697c + 2,DAT_00e38db0 + -0x10,DAT_00e38dac + -0x82);
  FUN_0053e590(DAT_00f5697c + 2,DAT_00e38db0 + -0x10,DAT_00e38dac + -0x10);
  return;
}
