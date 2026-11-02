#include "restore_types.h"
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bcd600 (*(undefined4 *)0x00bcd600u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)

int FUN_004b5770(int param_1)

{
  byte bVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  
  sVar2 = *(short *)(0x00b350dcu +
       (unsigned int)*(int *)(0x00bcd618u + (unsigned int)param_1 * 0x184u) * 2u);
  while( true ) {
    iVar3 = (int)sVar2;
    if (iVar3 < 1) {
      return 0;
    }
    iVar4 = iVar3 * 0x184;
    if ((((((*(byte *)(0x00bcd630u + (unsigned int)iVar4) != '\x04') &&
          (bVar1 = *(byte *)(0x00bcd602u + (unsigned int)iVar4), bVar1 != 0)) &&
          (bVar1 != 6)) && ((bVar1 != 0xe && (bVar1 != 0x24)))) &&
        ((bVar1 != 0x25 && ((bVar1 < 0x29 || (0x4c < bVar1)))))) && (bVar1 < 0x4f)) break;
    sVar2 = *(short *)(0x00bcd600u + (unsigned int)iVar4);
  }
  return iVar3;
}
