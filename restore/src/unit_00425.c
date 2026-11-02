#include "restore_types.h"
#define DAT_005c9348 (*(undefined *)0x005c9348u)
#define DAT_005c9354 (*(undefined4 *)0x005c9354u)
#define DAT_00705734 (*(undefined *)0x00705734u)
#define DAT_0078d5f0 (*(undefined *)0x0078d5f0u)

void FUN_00426a80(void)

{
  int iVar1;
  int *piVar2;
  
  DAT_00705734 = 0;
  piVar2 = &DAT_005c9354;
  do {
    iVar1 = (&DAT_0078d5f0)[piVar2[-1]];
    if (iVar1 < 1) {
      iVar1 = *piVar2;
    }
    else if (iVar1 < 0x14) {
      iVar1 = piVar2[1];
    }
    else if (iVar1 < 0x28) {
      iVar1 = piVar2[2];
    }
    else if (iVar1 < 0x3c) {
      iVar1 = piVar2[3];
    }
    else if (iVar1 < 0x50) {
      iVar1 = piVar2[4];
    }
    else if (iVar1 < 100) {
      iVar1 = piVar2[5];
    }
    else {
      iVar1 = piVar2[6];
    }
    if (iVar1 + 0x1e < DAT_005c9348) {
      DAT_00705734 = DAT_00705734 + 5;
    }
    piVar2 = piVar2 + 0xe;
  } while ((int)piVar2 < 0x5c95bc);
  return;
}
