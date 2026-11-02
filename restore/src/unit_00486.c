#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
extern int FUN_0045d510();
extern int FUN_00475b50();
extern longlong FUN_0056133c();

int FUN_00433ae0(int param_1,undefined4 param_2)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  
  switch((&DAT_007999e8)[param_1 * 0x84]) {
  case 0x46: ;
  case 0xbe: ;
    break;
  case 0x47: ;
  case 0xbf: ;
    break;
  case 0x48: ;
  case 0xc1: ;
    FUN_0045d510(param_1,param_2);
    break;
  case 0x49: ;
    uVar1 = FUN_00475b50(param_1,param_2);
    FUN_0045d510(uVar1);
    break;
default: ;
    if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) == '\0') {
      return 0;
    }
  }
  iVar2 = FUN_0056133c();
  iVar3 = 9;
  if (4 - iVar2 < 10) {
    iVar3 = 4 - iVar2;
  }
  return iVar3;
}
