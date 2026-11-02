#include "restore_types.h"
#define DAT_00ea4006 (*(undefined4 *)0x00ea4006u)

void FUN_004d18b0(void)

{
  short *psVar1;
  int *piVar2;
  int iVar3;
  short *psVar4;
  
  psVar4 = (short *)&DAT_00ea4006;
  do {
    iVar3 = 0x1e;
    psVar1 = psVar4;
    piVar2 = (int *)(psVar4 + 0x1065);
    do {
      piVar2[0x24] = (int)psVar1[0x24];
      *piVar2 = (int)*psVar1;
      psVar1 = psVar1 + 1;
      iVar3 = iVar3 + -1;
      piVar2 = piVar2 + 1;
    } while (iVar3 != 0);
    psVar4 = psVar4 + 0x24ec;
  } while ((int)psVar4 < 0xead3b6);
  return;
}
