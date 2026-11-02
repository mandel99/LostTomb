#include "restore_types.h"
#define DAT_00f543b8 (*(undefined4 *)0x00f543b8u)
extern undefined4 FUN_00539c60();

void FUN_0053af70(int param_1,undefined4 param_2)

{
  undefined4 uVar1;
  
  uVar1 = FUN_00539c60(param_2);
  (&DAT_00f543b8)[param_1] = uVar1;
  return;
}
