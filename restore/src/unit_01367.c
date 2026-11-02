#include "restore_types.h"
#define DAT_00bcb9d0 (*(undefined4 *)0x00bcb9d0u)
#define DAT_00bcb9f0 (*(undefined4 *)0x00bcb9f0u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd68a (*(undefined4 *)0x00bcd68au)

uint FUN_004a43f0(int param_1)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  short *psVar6;
  
  uVar4 = 0;
  uVar1 = (uint)(byte)(&DAT_00bcd68a)[param_1 * 0x184];
  if (uVar1 != 0) {
    uVar3 = (uint)(byte)(&DAT_00bcb9f0)[uVar1 * 0x90];
    iVar5 = 0;
    if (uVar3 != 0) {
      psVar6 = (short *)(&DAT_00bcb9d0 + uVar1 * 0x90);
      do {
        if (*psVar6 == 0) break;
        iVar2 = *psVar6 * 0x184;
        if (((&DAT_00bcd630)[iVar2] != '\x04') && ((&DAT_00bcd609)[iVar2] == '\b')) {
          uVar4 = uVar4 + 1;
        }
        iVar5 = iVar5 + 1;
        psVar6 = psVar6 + 1;
      } while (iVar5 < (int)uVar3);
    }
    if (uVar4 == uVar3) {
      return uVar1;
    }
  }
  return 0;
}
