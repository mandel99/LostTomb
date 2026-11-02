#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
extern longlong FUN_0056133c();

void FUN_00401540(int param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *(int *)(&DAT_00799a2c + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84);
  if (iVar1 == 0x38) {
    iVar2 = 100;
  }
  else {
    iVar2 = FUN_0056133c();
  }
  if (param_2 != (int *)0x0) {
    *param_2 = iVar1;
    param_2[3] = iVar2;
  }
  return;
}
