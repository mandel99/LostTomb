#include "restore_types.h"
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bcd600 (*(undefined4 *)0x00bcd600u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)

int FUN_004b57e0(int param_1,uint param_2)

{
  short sVar1;
  int iVar2;
  
  sVar1 = *(short *)(0x00b350dcu +
       (unsigned int)*(int *)(0x00bcd618u + (unsigned int)param_1 * 0x184u) * 2u);
  while( true ) {
    iVar2 = (int)sVar1;
    if (iVar2 < 1) {
      return 0;
    }
    if (*(byte *)(0x00bcd602u + (unsigned int)iVar2 * 0x184u) == param_2) break;
    sVar1 = *(short *)(0x00bcd600u + (unsigned int)iVar2 * 0x184u);
  }
  return iVar2;
}
