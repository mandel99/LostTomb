#include "restore_types.h"
#define DAT_00f54550 (*(undefined4 *)0x00f54550u)
#define DAT_00f54578 (*(undefined4 *)0x00f54578u)
#define DAT_00f5457c (*(undefined4 *)0x00f5457cu)
extern undefined4 FUN_00539c60();

void FUN_0053bc80(int param_1,undefined4 param_2,int param_3)

{
  int *piVar1;
  undefined4 uVar2;
  
  if (param_3 == 0) {
    uVar2 = FUN_00539c60(param_2);
    (&DAT_00f54578)[param_1 * 0xc] = uVar2;
  }
  else if (param_3 == 1) {
    piVar1 = &DAT_00f5457c + param_1 * 0xc;
    if ((int)(&DAT_00f5457c)[param_1 * 0xc] < 10) {
      uVar2 = FUN_00539c60(param_2);
      (&DAT_00f54550)[*piVar1 + param_1 * 0xc] = uVar2;
      *piVar1 = *piVar1 + 1;
      return;
    }
  }
  return;
}
