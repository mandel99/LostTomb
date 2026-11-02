#include "restore_types.h"
#define DAT_005c9348 (*(undefined *)0x005c9348u)
#define DAT_005c9370 (*(undefined4 *)0x005c9370u)
#define DAT_00705738 (*(undefined *)0x00705738u)
#define DAT_0078d5f0 (*(undefined *)0x0078d5f0u)

void FUN_00426a00(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  DAT_005c9348 = 100;
  iVar3 = 0;
  piVar1 = &DAT_005c9370;
  do {
    iVar2 = (&DAT_0078d5f0)[piVar1[-8]];
    if (*piVar1 < iVar2) {
      if (iVar2 < piVar1[1]) {
        iVar2 = piVar1[-6];
      }
      else if (iVar2 < piVar1[2]) {
        iVar2 = piVar1[-5];
      }
      else if (iVar2 < piVar1[3]) {
        iVar2 = piVar1[-4];
      }
      else if (iVar2 < piVar1[4]) {
        iVar2 = piVar1[-3];
      }
      else if (iVar2 < piVar1[5]) {
        iVar2 = piVar1[-2];
      }
      else {
        iVar2 = piVar1[-1];
      }
    }
    else {
      iVar2 = piVar1[-7];
    }
    if (iVar2 < DAT_005c9348) {
      DAT_005c9348 = iVar2;
      DAT_00705738 = iVar3;
    }
    piVar1 = piVar1 + 0xe;
    iVar3 = iVar3 + 1;
  } while ((int)piVar1 < 0x5c95d8);
  return;
}
