#include "restore_types.h"
#define DAT_00ea418c (*(undefined4 *)0x00ea418cu)

void FUN_004d1a70(void)

{
  short *psVar1;
  int *piVar2;
  int iVar3;
  short *psVar4;
  
  psVar4 = (short *)&DAT_00ea418c;
  do {
    iVar3 = 0x14;
    psVar1 = psVar4;
    piVar2 = (int *)(psVar4 + 0x9a4);
    do {
      piVar2[0x28] = (int)psVar1[0x14];
      *piVar2 = (int)*psVar1;
      psVar1 = psVar1 + 1;
      iVar3 = iVar3 + -1;
      piVar2 = piVar2 + 1;
    } while (iVar3 != 0);
    psVar1 = psVar4 + 0x9cc;
    iVar3 = 0x14;
    do {
      psVar1[0x50] = 0;
      psVar1[0x51] = 0;
      psVar1[0] = 0;
      psVar1[1] = 0;
      psVar1 = psVar1 + 2;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    psVar4 = psVar4 + 0x24ec;
  } while ((int)psVar4 < 0xead53c);
  return;
}
