#include "restore_types.h"
extern void FUN_00561875();

void FUN_005640a0(undefined4 param_1,undefined4 param_2,int param_3)

{
  code *pcVar1;
  
  pcVar1 = (code *)(FUN_00561875(param_3), restore_unresolved_return(0x00561875u));
  (*pcVar1)();
  if (param_3 == 0x100) {
    param_3 = 2;
  }
  FUN_00561875(param_3);
  return;
}
