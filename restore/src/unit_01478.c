#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a28 (*(undefined4 *)0x00799a28u)
#define DAT_00bcb9cb (*(undefined4 *)0x00bcb9cbu)
#define DAT_00bcb9ee (*(undefined4 *)0x00bcb9eeu)
#define DAT_00bcb9f0 (*(undefined4 *)0x00bcb9f0u)
#define DAT_00bcb9f1 (*(undefined4 *)0x00bcb9f1u)
#define DAT_00bcba0e (*(undefined4 *)0x00bcba0eu)
#define DAT_00bcba2c (*(undefined4 *)0x00bcba2cu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
extern void FUN_004b7c80();

void FUN_004b76b0(int param_1)

{
  byte bVar1;
  byte bVar2;
  short sVar3;
  int iVar4;
  int iVar5;
  short *psVar6;
  int iVar7;
  
  iVar4 = *(short *)(&DAT_00799a28 + param_1 * 0x108) * 0x90;
  *(undefined2 *)(&DAT_00bcba0e + iVar4) = 0;
  if (((&DAT_00bcb9cb)[iVar4] != '\0') && (*(short *)(&DAT_00bcba2c + iVar4) == 0)) {
    bVar1 = (&DAT_00bcb9f0)[iVar4];
    bVar2 = (&DAT_00bcb9f1)[iVar4];
    if (bVar1 != bVar2) {
      if (bVar1 < bVar2) {
        sVar3 = (&DAT_007999ea)[param_1 * 0x84];
        if (sVar3 == 0xd) {
          *(undefined2 *)(&DAT_00bcba0e + iVar4) = 3;
        }
        if (sVar3 == 0xb) {
          *(undefined2 *)(&DAT_00bcba0e + iVar4) = 1;
        }
        if (sVar3 == 0xc) {
          *(undefined2 *)(&DAT_00bcba0e + iVar4) = 2;
          return;
        }
      }
      else {
        iVar5 = (uint)bVar1 - (uint)bVar2;
        iVar7 = 0xf;
        psVar6 = (short *)(&DAT_00bcb9ee + iVar4);
        do {
          if (0 < *psVar6) {
            iVar5 = iVar5 + -1;
            (&DAT_00bcd630)[*psVar6 * 0x184] = 10;
            if (iVar5 < 1) break;
          }
          iVar7 = iVar7 + -1;
          psVar6 = psVar6 + -1;
        } while (-1 < iVar7);
        FUN_004b7c80();
      }
    }
  }
  return;
}
