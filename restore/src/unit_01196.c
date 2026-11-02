#include "restore_types.h"
extern void FUN_004818a0();

void FUN_00481830(undefined4 param_1,undefined4 param_2,int param_3,int param_4,int param_5,
                 int param_6,int param_7)

{
  int iVar1;
  int iVar2;
  
  if (0 < param_3) {
    if (6 < param_7) {
      param_7 = 6;
    }
    iVar1 = 0;
    iVar2 = 1;
    if (0 < param_7) {
      do {
        FUN_004818a0(param_1,param_2,param_3,iVar2,param_4);
        iVar2 = iVar2 + 1;
        iVar1 = iVar1 + 1;
        if (param_5 <= iVar1) {
          param_4 = param_4 + param_6;
          iVar1 = 0;
        }
        param_7 = param_7 + -1;
      } while (param_7 != 0);
    }
  }
  return;
}
