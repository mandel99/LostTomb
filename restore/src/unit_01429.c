#include "restore_types.h"
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd632 (*(undefined4 *)0x00bcd632u)
#define DAT_00d04e58 (*(undefined4 *)0x00d04e58u)

void FUN_004b32c0(void)

{
  int iVar1;
  short *psVar2;
  int iVar3;
  
  iVar3 = 1;
  psVar2 = (short *)((int)&DAT_00d04e58 + 2);
  do {
    iVar1 = (int)*psVar2;
    if ((0 < iVar1) && (iVar1 < 2000)) {
      if ((&DAT_00bcd606)[iVar1 * 0x184] != '\x01') {
        *psVar2 = 0;
      }
      if (*(short *)(&DAT_00bcd632 + iVar1 * 0x184) != iVar3) {
        *psVar2 = 0;
      }
    }
    psVar2 = psVar2 + 1;
    iVar3 = iVar3 + 1;
  } while ((int)psVar2 < 0xd05628);
  return;
}
