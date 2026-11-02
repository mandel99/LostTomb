#include "restore_types.h"
#define DAT_00790136 (*(undefined4 *)0x00790136u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)

void FUN_004d5ad0(void)

{
  short sVar1;
  short *psVar2;
  int iVar3;
  int iVar4;
  short *psVar5;
  
  psVar5 = &DAT_007999e8;
  do {
    if (((char)psVar5[-8] == '\x01') && (*psVar5 == 0x47)) {
      iVar3 = 0;
      psVar2 = psVar5 + 0x24;
      iVar4 = 0x23;
      do {
        iVar3 = iVar3 + *psVar2;
        psVar2 = psVar2 + 1;
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
      iVar4 = 0x23;
      psVar5[0x23] = (ushort)(0xc80 - iVar3) & (0xc80 - iVar3 < 0) - 1;
      psVar2 = (short *)(&DAT_00790136 + (uint)*(byte *)((int)psVar5 + 0xb1) * 0xc4);
      do {
        sVar1 = psVar2[-0x24];
        if (sVar1 < 0x321) {
          psVar2[-0x24] = 800;
        }
        else if (sVar1 < 0x641) {
          psVar2[-0x24] = 0x640;
        }
        else if (sVar1 < 0x961) {
          psVar2[-0x24] = 0x960;
        }
        else if (sVar1 < 0xc81) {
          psVar2[-0x24] = 0xc80;
        }
        sVar1 = *psVar2;
        if (sVar1 < 0x321) {
          *psVar2 = 800;
        }
        else if (sVar1 < 0x641) {
          *psVar2 = 0x640;
        }
        else if (sVar1 < 0x961) {
          *psVar2 = 0x960;
        }
        else if (sVar1 < 0xc81) {
          *psVar2 = 0xc80;
        }
        psVar2 = psVar2 + 1;
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
    }
    psVar5 = psVar5 + 0x84;
  } while ((int)psVar5 < 0x89b6e8);
  return;
}
