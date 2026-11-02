#include "restore_types.h"
#define DAT_01266424 (*(undefined4 *)0x01266424u)
extern undefined1 * FUN_00564263();

void FUN_005644e0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  DAT_01266424 = 1;
  FUN_00564263(param_1,param_2,param_3,param_4);
  DAT_01266424 = 0;
  return;
}
