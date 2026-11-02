#include "restore_types.h"
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bcd600 (*(undefined4 *)0x00bcd600u)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd682 (*(undefined4 *)0x00bcd682u)

void FUN_004b53b0(int param_1)

{
  short sVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = param_1 * 0x184;
  (&DAT_00bcd682)[iVar3] = 0;
  sVar1 = *(short *)(&DAT_00b350dc + *(int *)(&DAT_00bcd618 + iVar3) * 2);
  while( true ) {
    iVar2 = (int)sVar1;
    if (iVar2 == 0) {
      if (0x14 < (byte)(&DAT_00bcd682)[iVar3]) {
        (&DAT_00bcd682)[iVar3] = 0x14;
      }
      return;
    }
    if (iVar2 == param_1) break;
    (&DAT_00bcd682)[iVar3] = (&DAT_00bcd682)[iVar3] + '\x01';
    sVar1 = *(short *)(&DAT_00bcd600 + iVar2 * 0x184);
  }
  return;
}
