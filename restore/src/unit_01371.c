#include "restore_types.h"
#define DAT_00bcb9d0 (*(undefined4 *)0x00bcb9d0u)
#define DAT_00bcba4c (*(undefined4 *)0x00bcba4cu)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd68a (*(undefined4 *)0x00bcd68au)
#define DAT_00bcd728 (*(undefined4 *)0x00bcd728u)
extern void FUN_004b1d50();
extern void FUN_004b7c80();

void FUN_004a46f0(int param_1)

{
  byte bVar1;
  short sVar2;
  int iVar3;
  short *psVar4;
  
  FUN_004b1d50(param_1);
  param_1 = param_1 * 0x184;
  if (*(short *)(&DAT_00bcd62e + param_1) == 2) {
    if ((&DAT_00bcd728)[param_1] != '\0') {
      bVar1 = (&DAT_00bcd68a)[param_1];
      iVar3 = 0;
      (&DAT_00bcba4c)[(uint)bVar1 * 0x24] = 0;
      psVar4 = (short *)(&DAT_00bcb9d0 + (uint)bVar1 * 0x90);
      do {
        sVar2 = *psVar4;
        if (sVar2 == 0) break;
        psVar4 = psVar4 + 1;
        iVar3 = iVar3 + 1;
        (&DAT_00bcd606)[sVar2 * 0x184] = 2;
      } while (iVar3 < 0x10);
    }
    FUN_004b7c80();
  }
  return;
}
