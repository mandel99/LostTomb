#include "restore_types.h"
#define DAT_00d04e58 (*(undefined4 *)0x00d04e58u)

int FUN_004b3320(void)

{
  int iVar1;
  short *psVar2;
  
  iVar1 = 0;
  psVar2 = (short *)((int)&DAT_00d04e58 + 2);
  do {
    if (*psVar2 == 0) {
      iVar1 = iVar1 + 1;
    }
    psVar2 = psVar2 + 1;
  } while ((int)psVar2 < 0xd05628);
  return iVar1;
}
