#include "restore_types.h"
#define DAT_00784de8 (*(undefined4 *)0x00784de8u)

int FUN_0040cc40(int param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  
  iVar1 = 0;
  iVar3 = 0;
  piVar2 = &DAT_00784de8;
  do {
    if ((iVar3 != param_1) && (0 < *piVar2)) {
      iVar1 = iVar1 + 1;
    }
    piVar2 = piVar2 + 1;
    iVar3 = iVar3 + 1;
  } while ((int)piVar2 < 0x784e78);
  return iVar1;
}
