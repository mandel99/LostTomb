#include "restore_types.h"
#define DAT_00ea3e34 (*(undefined4 *)0x00ea3e34u)

void FUN_004e92f0(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = 0;
  iVar3 = 0;
  if (param_3 != 0) {
    do {
      iVar1 = param_1 * 0x24ec + param_2 * 10 + iVar2;
      if (*(short *)(0x00ea3e34u + (unsigned int)iVar1 * 2u) < 1) {
        iVar3 = iVar3 + 1;
      }
      else {
        param_3 = param_3 + -1;
        *(short *)(0x00ea3e34u + (unsigned int)iVar1 * 2u) =
             *(short *)(0x00ea3e34u + (unsigned int)iVar1 * 2u) + -1;
        iVar3 = 0;
      }
      iVar2 = iVar2 + 1;
      if (9 < iVar2) {
        iVar2 = 0;
      }
    } while ((iVar3 < 10) && (param_3 != 0));
  }
  return;
}
