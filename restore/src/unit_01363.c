#include "restore_types.h"
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
extern void FUN_004b27e0();

int FUN_004a3b00(int param_1,int param_2)

{
  short sVar1;
  int iVar2;
  
  if (param_2 < 2) {
    iVar2 = (FUN_004b27e0(param_1,1), restore_unresolved_return(0x004b27e0u));
  }
  else {
    *(short *)(&DAT_00bcd62e + param_1 * 0x184) = *(short *)(&DAT_00bcd62e + param_1 * 0x184) + 1;
    sVar1 = *(short *)(&DAT_00bcd62e + param_1 * 0x184);
    iVar2 = (int)sVar1 / param_2;
    if ((int)sVar1 % param_2 != 0) {
      iVar2 = (FUN_004b27e0(param_1,1), restore_unresolved_return(0x004b27e0u));
      return iVar2;
    }
    if (100 < sVar1) {
      *(undefined2 *)(&DAT_00bcd62e + param_1 * 0x184) = 0;
      return iVar2;
    }
  }
  return iVar2;
}
