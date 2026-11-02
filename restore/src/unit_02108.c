#include "restore_types.h"
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
extern int FUN_004efe30();

undefined4 FUN_004f0190(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  if (DAT_005e4ef4 == 5) {
    return 0;
  }
  uVar1 = FUN_004efe30(param_1,param_2,param_3,1);
  return uVar1;
}
