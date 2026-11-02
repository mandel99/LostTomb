#include "restore_types.h"
#define DAT_00f54120 (*(undefined4 *)0x00f54120u)
extern undefined4 FUN_00539c60();

void FUN_0053b100(int param_1,undefined4 param_2,int param_3)

{
  undefined4 uVar1;
  
  uVar1 = FUN_00539c60(param_2);
  (&DAT_00f54120)[param_1 * 5 + param_3] = uVar1;
  return;
}
