#include "restore_types.h"
extern undefined4 FUN_0044f170();
extern undefined4 FUN_0044f360();
extern undefined4 FUN_0044f3d0();
extern undefined4 FUN_0044f440();

undefined4 FUN_0044f120(undefined4 param_1)

{
  undefined4 uVar1;
  
  switch((int)(param_1)) {
  case 0: ;
    uVar1 = FUN_0044f170();
    return uVar1;
  case 1: ;
    uVar1 = FUN_0044f360();
    return uVar1;
  case 2: ;
    uVar1 = FUN_0044f3d0();
    return uVar1;
  case 3: ;
    uVar1 = FUN_0044f440();
    return uVar1;
default: ;
    return 0xffffffff;
  }
}
