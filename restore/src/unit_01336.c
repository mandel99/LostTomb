#include "restore_types.h"
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
extern int FUN_0045d510();

uint FUN_004963b0(undefined4 param_1,undefined4 param_2,int param_3,undefined4 param_4,int param_5,
                 int param_6)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  
  param_5 = (400 - (short)(&DAT_00799a2e)[param_6 + param_3 * 0x84]) - param_5;
  if (param_5 < 1) {
    return 0;
  }
  uVar1 = param_5 / 100 + 1;
  iVar2 = FUN_0045d510(param_4,param_1);
  if (iVar2 < 400) {
    if (iVar2 < 300) {
      if (iVar2 < 200) {
        if (iVar2 < 100) {
          uVar3 = (uint)(0 < iVar2);
        }
        else {
          uVar3 = 1;
        }
      }
      else {
        uVar3 = 2;
      }
    }
    else {
      uVar3 = 3;
    }
  }
  else {
    uVar3 = 4;
  }
  if ((int)uVar1 < (int)uVar3) {
    uVar3 = uVar1;
  }
  return uVar3 & ((int)uVar3 < 1) - 1;
}
