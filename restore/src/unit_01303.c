#include "restore_types.h"
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd632 (*(undefined4 *)0x00bcd632u)
#define DAT_00d04e58 (*(undefined4 *)0x00d04e58u)

undefined4 FUN_00489970(int param_1,int *param_2)

{
  short *psVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  
  sVar2 = *(short *)(&DAT_00bcd632 + param_1 * 0x184);
  psVar1 = (short *)(&DAT_00bcd632 + param_1 * 0x184);
  if (sVar2 != 0) {
    if (*(short *)((int)&DAT_00d04e58 + sVar2 * 2) == param_1) {
      *param_2 = (int)sVar2;
      sVar2 = *psVar1;
      *psVar1 = 0;
      *(undefined2 *)((int)&DAT_00d04e58 + sVar2 * 2) = 0;
      return 1;
    }
    *psVar1 = 0;
  }
  *param_2 = 1;
  while( true ) {
    iVar3 = *param_2;
    iVar4 = (int)*(short *)((int)&DAT_00d04e58 + iVar3 * 2);
    if (iVar4 == 0) {
      return 1;
    }
    if ((iVar4 < 0) || (1999 < iVar4)) {
      *(undefined2 *)((int)&DAT_00d04e58 + *param_2 * 2) = 0;
      return 1;
    }
    if (*(short *)(&DAT_00bcd632 + iVar4 * 0x184) != iVar3) break;
    if ((&DAT_00bcd606)[iVar4 * 0x184] != '\x01') {
      *(undefined2 *)(&DAT_00bcd632 + iVar4 * 0x184) = 0;
      break;
    }
    *param_2 = iVar3 + 1;
    if (999 < iVar3 + 1) {
      *param_2 = 0;
      return 0;
    }
  }
  *(undefined2 *)((int)&DAT_00d04e58 + *param_2 * 2) = 0;
  return 1;
}
