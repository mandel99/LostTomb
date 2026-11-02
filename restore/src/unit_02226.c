#include "restore_types.h"
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
extern int FUN_0053e590();

void FUN_004fbf00(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  uVar1 = DAT_00e20778;
  FUN_0053e590(DAT_00f571fc + param_1,param_2,param_3);
  DAT_00e20778 = uVar1;
  return;
}
