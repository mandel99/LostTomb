#include "restore_types.h"
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bcd600 (*(undefined4 *)0x00bcd600u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)

int FUN_004a59c0(int param_1,int param_2)

{
  char cVar1;
  short sVar2;
  int iVar3;
  
  sVar2 = *(short *)(&DAT_00b350dc + param_1 * 2);
  while( true ) {
    while( true ) {
      iVar3 = (int)sVar2;
      if (iVar3 == 0) {
        return 0;
      }
      if (iVar3 != param_2) break;
      sVar2 = *(short *)(&DAT_00bcd600 + iVar3 * 0x184);
    }
    cVar1 = (&DAT_00bcd602)[iVar3 * 0x184];
    if (cVar1 == 'M') {
      return iVar3;
    }
    if (cVar1 == 'N') {
      return iVar3;
    }
    if (cVar1 == '\x19') break;
    sVar2 = *(short *)(&DAT_00bcd600 + iVar3 * 0x184);
  }
  return iVar3;
}
