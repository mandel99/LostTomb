#include "restore_types.h"
#define DAT_005f80fc (*(undefined4 *)0x005f80fcu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
extern void FUN_004b1210();

void FUN_004b1700(int param_1)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  
  piVar3 = &DAT_005f80fc;
  iVar2 = param_1 * 0x184;
  iVar1 = *(int *)(&DAT_00bcd618 + iVar2);
  do {
    if ((&DAT_00bcd630)[iVar2] == '\x05') {
      return;
    }
    switch((&DAT_00bcd602)[iVar2]) {
    case 0x19: ;
    case 0x4d: ;
    case 0x4e: ;
    case 0x5c: ;
    case 0x5d: ;
    case 100: ;
    case 0x65: ;
      break;
default: ;
      FUN_004b1210(param_1,(int)*(short *)(&DAT_00b350dc + (*piVar3 + iVar1) * 2));
    }
    piVar3 = piVar3 + 1;
  } while ((int)piVar3 < 0x5f811c);
  return;
}
